#include "ps4_platform.h"
#include "ps4_shaderprogram.h"
#include "toolkit/common/allocator.h"
EmbeddedCsShader cs_setint = {cs_setint_bin};
EmbeddedVsShader vs_basic = {vs_basic_bin, "Basic vs"};
EmbeddedPsShader ps_basic = {ps_basic_bin, "Basic ps"};

extern LinearAllocator garlicAllocator;
extern LinearAllocator onionAllocator;

void Loadin(const uint32_t *m_source, void *&shaderBinary, void *&shaderHeader) {
  /*
  Gnmx::ShaderInfo shaderInfo;
  Gnmx::parseShader(&shaderInfo, m_source);
  shaderBinary = garlicAllocator.allocate(shaderInfo.m_gpuShaderCodeSize,
                                                        Gnm::kAlignmentOfShaderInBytes);
  shaderHeader = onionAllocator.allocate(shaderInfo.m_csShader->computeSize(),
                                                       Gnm::kAlignmentOfBufferInBytes);
  memcpy(shaderBinary, shaderInfo.m_gpuShaderCode, shaderInfo.m_gpuShaderCodeSize);
  memcpy(shaderHeader, shaderInfo.m_csShader, shaderInfo.m_csShader->computeSize());
  */
}

void EmbeddedCsShader::Init() {
  void *shaderBinary;
  void *shaderHeader;
  Gnmx::ShaderInfo shaderInfo;
  Gnmx::parseShader(&shaderInfo, m_source);
  shaderBinary =
      garlicAllocator.allocate(shaderInfo.m_gpuShaderCodeSize, Gnm::kAlignmentOfShaderInBytes);
  shaderHeader =
      onionAllocator.allocate(shaderInfo.m_csShader->computeSize(), Gnm::kAlignmentOfBufferInBytes);
  memcpy(shaderBinary, shaderInfo.m_gpuShaderCode, shaderInfo.m_gpuShaderCodeSize);
  memcpy(shaderHeader, shaderInfo.m_csShader, shaderInfo.m_csShader->computeSize());

  m_shader = static_cast<Gnmx::CsShader *>(shaderHeader);
  m_shader->patchShaderGpuAddress(shaderBinary);
  Gnmx::generateInputOffsetsCache(&m_offsetsTable, Gnm::kShaderStageCs, m_shader);
}

void EmbeddedPsShader::Init() {
  void *shaderBinary;
  void *shaderHeader;
  Gnmx::ShaderInfo shaderInfo;
  Gnmx::parseShader(&shaderInfo, m_source);
  shaderBinary =
      garlicAllocator.allocate(shaderInfo.m_gpuShaderCodeSize, Gnm::kAlignmentOfShaderInBytes);
  shaderHeader =
      onionAllocator.allocate(shaderInfo.m_psShader->computeSize(), Gnm::kAlignmentOfBufferInBytes);
  memcpy(shaderBinary, shaderInfo.m_gpuShaderCode, shaderInfo.m_gpuShaderCodeSize);
  memcpy(shaderHeader, shaderInfo.m_psShader, shaderInfo.m_psShader->computeSize());

  m_shader = static_cast<Gnmx::PsShader *>(shaderHeader);
  m_shader->patchShaderGpuAddress(shaderBinary);

  Gnmx::generateInputOffsetsCache(&m_offsetsTable, Gnm::kShaderStagePs, m_shader);


}
void EmbeddedVsShader::Init() {
  void *shaderBinary;
  void *shaderHeader;
  Gnmx::ShaderInfo shaderInfo;
  Gnmx::parseShader(&shaderInfo, m_source);

  shaderBinary =
      garlicAllocator.allocate(shaderInfo.m_gpuShaderCodeSize, Gnm::kAlignmentOfShaderInBytes);
  shaderHeader =
      onionAllocator.allocate(shaderInfo.m_vsShader->computeSize(), Gnm::kAlignmentOfBufferInBytes);

  memcpy(shaderBinary, shaderInfo.m_gpuShaderCode, shaderInfo.m_gpuShaderCodeSize);
  memcpy(shaderHeader, shaderInfo.m_vsShader, shaderInfo.m_vsShader->computeSize());

  auto as = shaderInfo.m_vsShader->computeSize();
  m_shader = static_cast<Gnmx::VsShader *>(shaderHeader);
  m_shader->patchShaderGpuAddress(shaderBinary);

  Gnmx::generateInputOffsetsCache(&m_offsetsTable, Gnm::kShaderStageVs, m_shader);
}

void InitShaders() {
  cs_setint.Init();
  vs_basic.Init();
  ps_basic.Init();
}
