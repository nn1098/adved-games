#include "common.h"
#include <gnmx.h>
#include <gnmx/shader_parser.h>
#include <kernel.h>
#include <scebase.h>
#include <video_out.h>

#include "component_camera.h"
#include "filesystem.h"
#include "mesh.h"
#include "resource.h"
#include <vectormath.h>

#include "ps4_memory.h"
#include "ps4_platform.h"
#include "ps4_renderer.h"
#include "ps4_shaderprogram.h"
#include "ps4_video.h"

#include "toolkit/toolkit.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

using namespace sce;
using namespace sce::Gnmx;
/* SCE CONFIDENTIAL
PlayStation(R)4 Programmer Tool Runtime Library Release 03.008.041
* Copyright (C) 2015 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#include <gnmx.h>
#include <kernel.h>
#include <scebase.h>
#include <stdio.h>
#include <stdlib.h>
#include <video_out.h>

#include "toolkit/common/allocator.h"
#include "toolkit/common/shader_loader.h"
#include "toolkit/toolkit.h"

#include "toolkit/std_cbuffer.h"

using namespace sce;
using namespace sce::Gnmx;
typedef struct RenderContext {
  Gnmx::GnmxGfxContext gfxContext;
  void *cueHeap;
  void *dcbBuffer;
  void *ccbBuffer;
  volatile uint32_t *eopLabel;
  volatile uint32_t *contextLabel;
} RenderContext;

typedef struct DisplayBuffer {
  Gnm::RenderTarget renderTarget;
  int displayIndex;
} DisplayBuffer;

enum EopState {
  kEopStateNotYet = 0,
  kEopStateFinished,
};

enum RenderContextState {
  kRenderContextFree = 0,
  kRenderContextInUse,
};
/*
static const Vertex kVertexData[] = {
    //   POSITION                COLOR               UV
    {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
    {0.5f, -0.5f, -0.5f, 0.7f, 0.7f, 1.0f, 1.0f, 1.0f},
    {-0.5f, 0.5f, 0.0f, 0.7f, 1.0f, 1.0f, 0.0f, 0.0f},
    {0.5f, 0.5f, 0.2f, 1.0f, 0.7f, 1.0f, 1.0f, 0.0f},
};
*/

static const Vertex kVertexData[] = {
  { -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
  { 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f },
  { 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
  { -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
  { -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
  { 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f },
  { 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
  { -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
};

//static const uint16_t kIndexData[] = {0, 1, 2, 1, 3, 2};
static const uint16_t kIndexData[] = {0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1, 7, 6, 5, 5, 4, 7,
                                      4, 0, 3, 3, 7, 4, 4, 5, 1, 1, 0, 4, 3, 2, 6, 6, 7, 3};
size_t sceLibcHeapSize = 64 * 1024 * 1024;
LinearAllocator garlicAllocator;
LinearAllocator onionAllocator;

static const uint32_t kRenderContextCount = 2;
RenderContext renderContexts[kRenderContextCount];
RenderContext *renderContext = renderContexts;
SceKernelEqueue eopEventQueue;
int videoOutHandle;
static const uint32_t kDisplayBufferCount = 3;
DisplayBuffer displayBuffers[kDisplayBufferCount];
DisplayBuffer *backBuffer = displayBuffers;
uint32_t backBufferIndex;
Gnm::DepthRenderTarget depthTarget;
Vector4 kClearColor = Vector4(1.0f, 0.0f, 0.0f, 0.0f);

VsShader *vsShader;
PsShader *psShader;
uint32_t shaderModifier;
void *fsMem;
Gnm::Buffer vertexBuffers[kVertexElemCount];

Gnm::Texture texture;
Gnm::Sampler sampler;

uint16_t *indexData;

static const uint32_t kIndexCount = sizeof(kIndexData) / sizeof(kIndexData[0]);

static const uint32_t kDisplayBufferWidth = 1920;
static const uint32_t kDisplayBufferHeight = 1080;
const float kAspectRatio = float(kDisplayBufferWidth) / float(kDisplayBufferHeight);
uint32_t renderContextIndex;

bool readRawTextureData(const char *path, void *address, size_t size) {
  bool success = false;

  FILE *fp = fopen(path, "rb");
  if (fp != NULL) {
    success = readFileContents(address, size, fp);
    fclose(fp);
  }

  return success;
}

void shutdown() {
  int ret;
  // Wait for the GPU to be idle before deallocating its resources
  for (uint32_t i = 0; i < kRenderContextCount; ++i) {
    if (renderContexts[i].contextLabel) {
      while (renderContexts[i].contextLabel[0] != kRenderContextFree) {
        sceKernelUsleep(1000);
      }
    }
  }

  // Unregister the EOP event queue
  ret = Gnm::deleteEqEvent(eopEventQueue, Gnm::kEqEventGfxEop);
  if (ret != SCE_OK) {
    printf("Gnm::deleteEqEvent failed: 0x%08X\n", ret);
  }

  // Destroy the EOP event queue
  ret = sceKernelDeleteEqueue(eopEventQueue);
  if (ret != SCE_OK) {
    printf("sceKernelDeleteEqueue failed: 0x%08X\n", ret);
  }

  // Terminate the video output
  ret = sceVideoOutClose(videoOutHandle);
  if (ret != SCE_OK) {
    printf("sceVideoOutClose failed: 0x%08X\n", ret);
  }

  // Releasing manually each allocated resource is not necessary as we are
  // terminating the linear allocators for ONION and GARLIC here.
  onionAllocator.terminate();
  garlicAllocator.terminate();
}

bool PS4_Renderer::Init() {
  static const uint32_t kRenderContextCount = 2;
  static const Gnm::ZFormat kZFormat = Gnm::kZFormat32Float;
  static const Gnm::StencilFormat kStencilFormat = Gnm::kStencil8;
  static const bool kHtileEnabled = true;

  static const size_t kOnionMemorySize = 16 * 1024 * 1024;
  static const size_t kGarlicMemorySize = 64 * 1024 * 1024;

  static const uint32_t kCueRingEntries = 64;
  static const size_t kDcbSizeInBytes = 2 * 1024 * 1024;
  static const size_t kCcbSizeInBytes = 256 * 1024;

  int ret;

  // Initialize the WB_ONION memory allocator
  ret = onionAllocator.initialize(kOnionMemorySize, SCE_KERNEL_WB_ONION,
                                  SCE_KERNEL_PROT_CPU_RW | SCE_KERNEL_PROT_GPU_ALL);
  if (ret != SCE_OK)
    return ret;

  // Initialize the WC_GARLIC memory allocator
  // NOTE: CPU reads from GARLIC write-combined memory have a very low
  //       bandwidth so they are disabled for safety in this sample
  ret = garlicAllocator.initialize(kGarlicMemorySize, SCE_KERNEL_WC_GARLIC,
                                   SCE_KERNEL_PROT_CPU_WRITE | SCE_KERNEL_PROT_GPU_ALL);
  if (ret != SCE_OK)
    return ret;

  // Open the video output port
  videoOutHandle = sceVideoOutOpen(0, SCE_VIDEO_OUT_BUS_TYPE_MAIN, 0, NULL);
  if (videoOutHandle < 0) {
    printf("sceVideoOutOpen failed: 0x%08X\n", videoOutHandle);
    return videoOutHandle;
  }

  // Initialize the flip rate: 0: 60Hz, 1: 30Hz or 2: 20Hz
  ret = sceVideoOutSetFlipRate(videoOutHandle, 0);
  if (ret != SCE_OK) {
    printf("sceVideoOutSetFlipRate failed: 0x%08X\n", ret);
    return ret;
  }

  // Create the event queue for used to synchronize with end-of-pipe interrupts

  ret = sceKernelCreateEqueue(&eopEventQueue, "EOP QUEUE");
  if (ret != SCE_OK) {
    printf("sceKernelCreateEqueue failed: 0x%08X\n", ret);
    return ret;
  }

  // Register for the end-of-pipe events
  ret = Gnm::addEqEvent(eopEventQueue, Gnm::kEqEventGfxEop, NULL);
  if (ret != SCE_OK) {
    printf("Gnm::addEqEvent failed: 0x%08X\n", ret);
    return ret;
  }

  // Initialize the Toolkit module
  sce::Gnmx::Toolkit::Allocators toolkitAllocators;
  onionAllocator.getIAllocator(toolkitAllocators.m_onion);
  garlicAllocator.getIAllocator(toolkitAllocators.m_garlic);
  Toolkit::initializeWithAllocators(&toolkitAllocators);

  InitShaders();
  printf("");
  vsShader = vs_basic.m_shader;
  psShader = ps_basic.m_shader;
  if (!vsShader || !psShader) {
    printf("Cannot load the shaders\n");
    return SCE_KERNEL_ERROR_EIO;
  }

  // Allocate the memory for the fetch shader
  fsMem = garlicAllocator.allocate(Gnmx::computeVsFetchShaderSize(vsShader),
                                   Gnm::kAlignmentOfFetchShaderInBytes);
  if (!fsMem) {
    printf("Cannot allocate the fetch shader memory\n");
    return SCE_KERNEL_ERROR_ENOMEM;
  }

  // Generate the fetch shader for the VS stage
  Gnmx::generateVsFetchShader(fsMem, &shaderModifier, vsShader, NULL);

  // For simplicity reasons the sample uses a single GfxContext for each
  // frame. Implementing more complex schemes where multipleGfxContext-s
  // are submitted in each frame is possible as well, but it is out of the
  // scope for this basic sample.

  renderContextIndex = 0;

  // Initialize all the render contexts
  for (uint32_t i = 0; i < kRenderContextCount; ++i) {
    // Allocate the CUE heap memory
    renderContexts[i].cueHeap =
        garlicAllocator.allocate(Gnmx::ConstantUpdateEngine::computeHeapSize(kCueRingEntries),
                                 Gnm::kAlignmentOfBufferInBytes);

    if (!renderContexts[i].cueHeap) {
      printf("Cannot allocate the CUE heap memory\n");
      return SCE_KERNEL_ERROR_ENOMEM;
    }

    // Allocate the draw command buffer
    renderContexts[i].dcbBuffer =
        onionAllocator.allocate(kDcbSizeInBytes, Gnm::kAlignmentOfBufferInBytes);

    if (!renderContexts[i].dcbBuffer) {
      printf("Cannot allocate the draw command buffer memory\n");
      return SCE_KERNEL_ERROR_ENOMEM;
    }

    // Allocate the constants command buffer
    renderContexts[i].ccbBuffer =
        onionAllocator.allocate(kCcbSizeInBytes, Gnm::kAlignmentOfBufferInBytes);

    if (!renderContexts[i].ccbBuffer) {
      printf("Cannot allocate the constants command buffer memory\n");
      return SCE_KERNEL_ERROR_ENOMEM;
    }

    // Initialize the GfxContext used by this rendering context
    renderContexts[i].gfxContext.init(renderContexts[i].cueHeap, kCueRingEntries,
                                      renderContexts[i].dcbBuffer, kDcbSizeInBytes,
                                      renderContexts[i].ccbBuffer, kCcbSizeInBytes);

    renderContexts[i].eopLabel = (volatile uint32_t *)onionAllocator.allocate(4, 8);
    renderContexts[i].contextLabel = (volatile uint32_t *)onionAllocator.allocate(4, 8);
    if (!renderContexts[i].eopLabel || !renderContexts[i].contextLabel) {
      printf("Cannot allocate a GPU label\n");
      return SCE_KERNEL_ERROR_ENOMEM;
    }

    renderContexts[i].eopLabel[0] = kEopStateFinished;
    renderContexts[i].contextLabel[0] = kRenderContextFree;
  }

  backBufferIndex = 0;

  // Convenience array used by sceVideoOutRegisterBuffers()
  void *surfaceAddresses[kDisplayBufferCount];

  // Initialize all the display buffers
  for (uint32_t i = 0; i < kDisplayBufferCount; ++i) {
    // Compute the tiling mode for the render target
    Gnm::TileMode tileMode;
    Gnm::DataFormat format = Gnm::kDataFormatB8G8R8A8UnormSrgb;
    ret = GpuAddress::computeSurfaceTileMode(
        &tileMode,                                      // Tile mode pointer
        GpuAddress::kSurfaceTypeColorTargetDisplayable, // Surface type
        format,                                         // Surface format
        1);                                             // Elements per pixel
    if (ret != SCE_OK) {
      printf("GpuAddress::computeSurfaceTileMode: 0x%08X\n", ret);
      return ret;
    }

    // Initialize the render target descriptor
    Gnm::SizeAlign sizeAlign = displayBuffers[i].renderTarget.init(
        kDisplayBufferWidth, kDisplayBufferHeight, 1, format, tileMode, Gnm::kNumSamples1,
        Gnm::kNumFragments1, NULL, NULL);

    // Allocate the render target memory
    surfaceAddresses[i] = garlicAllocator.allocate(sizeAlign);
    if (!surfaceAddresses[i]) {
      printf("Cannot allocate the render target memory\n");
      return SCE_KERNEL_ERROR_ENOMEM;
    }
    displayBuffers[i].renderTarget.setAddresses(surfaceAddresses[i], 0, 0);

    displayBuffers[i].displayIndex = i;
  }

  // Initialization the VideoOut buffer descriptor. The pixel format must
  // match with the render target data format, which in this case is
  // Gnm::kDataFormatB8G8R8A8UnormSrgb
  SceVideoOutBufferAttribute videoOutBufferAttribute;
  sceVideoOutSetBufferAttribute(
      &videoOutBufferAttribute, SCE_VIDEO_OUT_PIXEL_FORMAT_B8_G8_R8_A8_SRGB,
      SCE_VIDEO_OUT_TILING_MODE_TILE, SCE_VIDEO_OUT_ASPECT_RATIO_16_9,
      backBuffer->renderTarget.getWidth(), backBuffer->renderTarget.getHeight(),
      backBuffer->renderTarget.getPitch());

  // Register the buffers to the slot: [0..kDisplayBufferCount-1]
  ret = sceVideoOutRegisterBuffers(videoOutHandle,
                                   0, // Start index
                                   surfaceAddresses, kDisplayBufferCount, &videoOutBufferAttribute);
  if (ret != SCE_OK) {
    printf("sceVideoOutRegisterBuffers failed: 0x%08X\n", ret);
    return ret;
  }

  // Compute the tiling mode for the depth buffer
  Gnm::DataFormat depthFormat = Gnm::DataFormat::build(kZFormat);
  Gnm::TileMode depthTileMode;
  ret = GpuAddress::computeSurfaceTileMode(&depthTileMode, // Tile mode pointer
                                           GpuAddress::kSurfaceTypeDepthOnlyTarget, // Surface type
                                           depthFormat, // Surface format
                                           1);          // Elements per pixel
  if (ret != SCE_OK) {
    printf("GpuAddress::computeSurfaceTileMode: 0x%08X\n", ret);
    return ret;
  }

  // Initialize the depth buffer descriptor
  Gnm::SizeAlign stencilSizeAlign;
  Gnm::SizeAlign htileSizeAlign;
  Gnm::SizeAlign depthTargetSizeAlign =
      depthTarget.init(kDisplayBufferWidth, kDisplayBufferHeight, depthFormat.getZFormat(),
                       kStencilFormat, depthTileMode, Gnm::kNumFragments1,
                       kStencilFormat != Gnm::kStencilInvalid ? &stencilSizeAlign : NULL,
                       kHtileEnabled ? &htileSizeAlign : NULL);

  // Initialize the HTILE buffer, if enabled
  if (kHtileEnabled) {
    void *htileMemory = garlicAllocator.allocate(htileSizeAlign);
    if (!htileMemory) {
      printf("Cannot allocate the HTILE buffer\n");
      return SCE_KERNEL_ERROR_ENOMEM;
    }

    depthTarget.setHtileAddress(htileMemory);
  }

  // Initialize the stencil buffer, if enabled
  void *stencilMemory = NULL;
  if (kStencilFormat != Gnm::kStencilInvalid) {
    stencilMemory = garlicAllocator.allocate(stencilSizeAlign);
    if (!stencilMemory) {
      printf("Cannot allocate the stencil buffer\n");
      return SCE_KERNEL_ERROR_ENOMEM;
    }
  }

  // Allocate the depth buffer
  void *depthMemory = garlicAllocator.allocate(depthTargetSizeAlign);
  if (!depthMemory) {
    printf("Cannot allocate the depth buffer\n");
    return SCE_KERNEL_ERROR_ENOMEM;
  }
  depthTarget.setAddresses(depthMemory, stencilMemory);

  // Initialize a Gnm::Texture object

  Gnm::SizeAlign textureSizeAlign =
      texture.initAs2d(512, 512, 1, Gnm::kDataFormatR8G8B8A8UnormSrgb,
                       Gnm::kTileModeDisplay_LinearAligned, Gnm::kNumFragments1);

  // Allocate the texture data using the alignment returned by initAs2d
  void *textureData = garlicAllocator.allocate(textureSizeAlign);
  if (!textureData) {
    printf("Cannot allocate the texture data\n");
    return SCE_KERNEL_ERROR_ENOMEM;
  }

  // Read the texture data
  if (!readRawTextureData("/app0/texture.raw", textureData, textureSizeAlign.m_size)) {
    printf("Cannot load the texture data\n");
    return SCE_KERNEL_ERROR_EIO;
  }

  // Set the base data address in the texture object
  texture.setBaseAddress(textureData);

  // Initialize the texture sampler

  sampler.init();
  sampler.setMipFilterMode(Gnm::kMipFilterModeNone);
  sampler.setXyFilterMode(Gnm::kFilterModeBilinear, Gnm::kFilterModeBilinear);

  // Allocate the vertex buffer memory
  Vertex *vertexData = static_cast<Vertex *>(
      garlicAllocator.allocate(sizeof(kVertexData), Gnm::kAlignmentOfBufferInBytes));
  if (!vertexData) {
    printf("Cannot allocate vertex data\n");
    return SCE_KERNEL_ERROR_ENOMEM;
  }

  // Allocate the index buffer memory
  indexData = static_cast<uint16_t *>(
      garlicAllocator.allocate(sizeof(kIndexData), Gnm::kAlignmentOfBufferInBytes));
  if (!indexData) {
    printf("Cannot allocate index data\n");
    return SCE_KERNEL_ERROR_ENOMEM;
  }

  // Copy the vertex/index data onto the GPU mapped memory
  memcpy(vertexData, kVertexData, sizeof(kVertexData));
  memcpy(indexData, kIndexData, sizeof(kIndexData));

  // Initialize the vertex buffers pointing to each vertex element

  vertexBuffers[kVertexPosition].initAsVertexBuffer(&vertexData->x, Gnm::kDataFormatR32G32B32Float,
                                                    sizeof(Vertex),
                                                    sizeof(kVertexData) / sizeof(Vertex));

  vertexBuffers[kVertexColor].initAsVertexBuffer(&vertexData->r, Gnm::kDataFormatR32G32B32Float,
                                                 sizeof(Vertex),
                                                 sizeof(kVertexData) / sizeof(Vertex));

  vertexBuffers[kVertexUv].initAsVertexBuffer(&vertexData->u, Gnm::kDataFormatR32G32Float,
                                              sizeof(Vertex), sizeof(kVertexData) / sizeof(Vertex));

  return 0;
  return false;
}

bool PS4_Renderer::Shutdown() { return false; }
void PS4_Renderer::setCamera(Components::CmCamera *cm) {}
void PS4_Renderer::LoadMesh(Mesh *msh) {}

void PS4_Renderer::RenderMesh(const Mesh& m, const glm::mat4& modelMatrix) {
  if (!m.loadedLocal) {
   // LoadMesh(m);
    assert(false);
  }
  Gnmx::GnmxGfxContext &gfxc = renderContext->gfxContext;

  // Activate the VS and PS shader stages

  gfxc.setActiveShaderStages(Gnm::kActiveShaderStagesVsPs);
  gfxc.setVsShader(vsShader, shaderModifier, fsMem, &vs_basic.m_offsetsTable);
  gfxc.setPsShader(psShader, &ps_basic.m_offsetsTable);

  // Setup the vertex buffer used by the ES stage (vertex shader)
  // Note that the setXxx methods of GfxContext which are used to set
  // shader resources (e.g.: V#, T#, S#, ...) map directly on the
  // Constants Update Engine. These methods do not directly produce PM4
  // packets in the command buffer. The CUE gathers all the resource
  // definitions and creates a set of PM4 packets later on in the
  // gfxc.drawXxx method.
  gfxc.setVertexBuffers(Gnm::kShaderStageVs, 0, kVertexElemCount, vertexBuffers);

  // Setup the texture and its sampler on the PS stage
  gfxc.setTextures(Gnm::kShaderStagePs, 0, 1, &texture);
  gfxc.setSamplers(Gnm::kShaderStagePs, 0, 1, &sampler);

  // Allocate the vertex shader constants from the command buffer
  ShaderConstants *constants = static_cast<ShaderConstants *>(
      gfxc.allocateFromCommandBuffer(sizeof(ShaderConstants), Gnm::kEmbeddedDataAlignment4));

  // Initialize the vertex shader constants
  if (constants) {
    static float angle = 0.0f;
    angle += 1.0f / 120.0f;

   // const Matrix4 rotationMatrix = Matrix4::rotationZ(angle);
   // const Matrix4 trnMatrix = Matrix4::translation(Vector3(0.0f, 0.0f, -3.0f));
    //const Matrix4 rotationMatrix = Matrix4::rotation(angle, Vector3(0.3f, 0.9f, 0.5f));
    const Matrix4 rotationMatrix = Matrix4::identity();
    const Matrix4 scaleMatrix = Matrix4::scale(Vector3(1, kAspectRatio, 1));
    //Matrix4  Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    const float aspect = (float)backBuffer->renderTarget.getWidth() / (float)backBuffer->renderTarget.getHeight();
      
    Matrix4  Projection = Matrix4::perspective(glm::radians(60.0f), aspect, 0.1f, 100.0f);
    Matrix4 view = Matrix4::lookAt(Point3(4, 3, -5), Point3(0, 0, 0), Vector3(0, 1, 0));

    Matrix4 VP = Projection * view * Matrix4::rotation(angle, Vector3(0.3f, 0.9f, 0.5f));

    Gnm::Buffer constBuffer;
    constants->m_WorldViewProj = ToMatrix4Unaligned(VP);
    constBuffer.initAsConstantBuffer(constants, sizeof(ShaderConstants));
    gfxc.setConstantBuffers(Gnm::kShaderStageVs, 0, 1, &constBuffer);
  } else {
    printf("Cannot allocate vertex shader constants\n");
  }

  // Submit a draw call
  gfxc.setPrimitiveType(Gnm::kPrimitiveTypeTriList);
  gfxc.setIndexSize(Gnm::kIndexSize16);
  gfxc.drawIndex(kIndexCount, indexData);
}

void PS4_Renderer::ClearFrame() {
  Gnmx::GnmxGfxContext &gfxc = renderContext->gfxContext;
  // Clear the color and the depth target
  Toolkit::SurfaceUtil::clearRenderTarget(gfxc, &backBuffer->renderTarget, kClearColor);
  Toolkit::SurfaceUtil::clearDepthTarget(gfxc, &depthTarget, 1.f);
}

void PS4_Renderer::PreRender() {

  Gnmx::GnmxGfxContext &gfxc = renderContext->gfxContext;
  int ret;
  // Wait until the context label has been written to make sure that the
  // GPU finished parsing the command buffers before overwriting them
  while (renderContext->eopLabel[0] != kEopStateFinished) {
    // Wait for the EOP event
    SceKernelEvent eopEvent;
    int count;
    ret = sceKernelWaitEqueue(eopEventQueue, &eopEvent, 1, &count, NULL);
    if (ret != SCE_OK) {
      printf("sceKernelWaitEqueue failed: 0x%08X\n", ret);
    }
  }

  // Safety check
  volatile uint32_t spinCount = 0;
  while (renderContext->contextLabel[0] != kRenderContextFree) {
    ++spinCount;
  }

  // Reset the EOP and flip GPU labels
  renderContext->eopLabel[0] = kEopStateNotYet;
  renderContext->contextLabel[0] = kRenderContextInUse;

  // Reset the graphical context and initialize the hardware state
  gfxc.reset();
  gfxc.initializeDefaultHardwareState();

  // In a real-world scenario, any rendering of off-screen buffers or
  // other compute related processing would go here

  // The waitUntilSafeForRendering stalls the GPU until the scan-out
  // operations on the current display buffer have been completed.
  // This command is not blocking for the CPU.
  //
  // NOTE
  // This command should be used right before writing the display buffer.
  //
  gfxc.waitUntilSafeForRendering(videoOutHandle, backBuffer->displayIndex);

  // Setup the viewport to match the entire screen.
  // The z-scale and z-offset values are used to specify the transformation
  // from clip-space to screen-space
  gfxc.setupScreenViewport(0, // Left
                           0, // Top
                           backBuffer->renderTarget.getWidth(),
                           backBuffer->renderTarget.getHeight(),
                           0.5f,  // Z-scale
                           0.5f); // Z-offset

  // Bind the render & depth targets to the context
  gfxc.setRenderTarget(0, &backBuffer->renderTarget);
  gfxc.setDepthRenderTarget(&depthTarget);

  static float count = 0.0f;
  count += 0.1f;

  kClearColor = Vector4((sin(0.1f * count) * 0.5f) + 0.5f, (sin((0.1f * count) + 2) * 0.5f) + 0.5f,
          (sin((0.1f * count) + 4) * 0.5f) + 0.5f, 0.0f);

  ClearFrame();
  // Enable z-writes using a less comparison function
  Gnm::DepthStencilControl dsc;
  dsc.init();
  dsc.setDepthControl(Gnm::kDepthControlZWriteEnable, Gnm::kCompareFuncLess);
  dsc.setDepthEnable(true);
  gfxc.setDepthStencilControl(dsc);

  // Cull clock-wise backfaces
  Gnm::PrimitiveSetup primSetupReg;
  primSetupReg.init();
  primSetupReg.setCullFace(Gnm::kPrimitiveSetupCullFaceBack);
  primSetupReg.setFrontFace(Gnm::kPrimitiveSetupFrontFaceCcw);
  gfxc.setPrimitiveSetup(primSetupReg);

  // Setup an additive blending mode
  Gnm::BlendControl blendControl;
  blendControl.init();
  blendControl.setBlendEnable(true);
  blendControl.setColorEquation(Gnm::kBlendMultiplierSrcAlpha, Gnm::kBlendFuncAdd,
                                Gnm::kBlendMultiplierOneMinusSrcAlpha);
  gfxc.setBlendControl(0, blendControl);

  // Setup the output color mask
  gfxc.setRenderTargetMask(0xF);
}
void PS4_Renderer::PostRender() {}

void PS4_Renderer::Flip() {
  Gnmx::GnmxGfxContext &gfxc = renderContext->gfxContext;
  // Write the label that indicates that the GPU finished working on this frame
  // and trigger a software interrupt to signal the EOP event queue
  gfxc.writeAtEndOfPipeWithInterrupt(Gnm::kEopFlushCbDbCaches, Gnm::kEventWriteDestMemory,
                                     const_cast<uint32_t *>(renderContext->eopLabel),
                                     Gnm::kEventWriteSource32BitsImmediate, kEopStateFinished,
                                     Gnm::kCacheActionNone, Gnm::kCachePolicyLru);

  // Submit the command buffers, request a flip of the display buffer and
  // write the GPU label that determines the render context state (free)
  //
  // NOTE: for this basic sample we are submitting a single GfxContext
  // per frame. Submitting multiple GfxContext-s per frame is allowed.
  // Multiple contexts are processed in order, i.e.: they start in
  // submission order and end in submission order.
  int ret =
      gfxc.submitAndFlip(videoOutHandle, backBuffer->displayIndex, SCE_VIDEO_OUT_FLIP_MODE_VSYNC, 0,
                         const_cast<uint32_t *>(renderContext->contextLabel), kRenderContextFree);
  if (ret != sce::Gnm::kSubmissionSuccess) {
    // Analyze the error code to determine whether the command buffers
    // have been submitted to the GPU or not
    if (ret & sce::Gnm::kStatusMaskError) {
      // Error codes in the kStatusMaskError family block submissions
      // so we need to mark this render context as not-in-flight
      renderContext->eopLabel[0] = kEopStateFinished;
      renderContext->contextLabel[0] = kRenderContextFree;
    }

    printf("GfxContext::submitAndFlip failed: 0x%08X\n", ret);
  }

  // Signal the system that every draw for this frame has been submitted.
  // This function gives permission to the OS to hibernate when all the
  // currently running GPU tasks (graphics and compute) are done.
  ret = Gnm::submitDone();
  if (ret != SCE_OK) {
    printf("Gnm::submitDone failed: 0x%08X\n", ret);
  }

  // Rotate the display buffers
  backBufferIndex = (backBufferIndex + 1) % kDisplayBufferCount;
  backBuffer = displayBuffers + backBufferIndex;

  // Rotate the render contexts
  renderContextIndex = (renderContextIndex + 1) % kRenderContextCount;
  renderContext = renderContexts + renderContextIndex;
}