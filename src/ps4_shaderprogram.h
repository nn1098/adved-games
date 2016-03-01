#include "ps4_memory.h"
#include <gnmx.h>
#include <gnmx/shader_parser.h>
using namespace sce;

struct EmbeddedCsShader {
  const uint32_t *m_source;
  const char *m_name;
  sce::Gnmx::CsShader *m_shader;
  sce::Gnmx::InputOffsetsCache m_offsetsTable;

  void Init();
};

struct EmbeddedPsShader {
  const uint32_t *m_source;
  const char *m_name;
  Gnmx::PsShader *m_shader;
  Gnmx::InputOffsetsCache m_offsetsTable;
  void Init();
};

struct EmbeddedVsShader {
  const uint32_t *m_source;
  const char *m_name;
  Gnmx::VsShader *m_shader;
  void *m_fetchShader;
  Gnmx::InputOffsetsCache m_offsetsTable;
  void Init();
};

static const uint32_t cs_setint_bin[] = {
#include "compiled_shaders/ps4_shader_setint_c.h"
};
static const uint32_t vs_basic_bin[] = {
#include "ps4_shader_basic_vv.h"
};
static const uint32_t ps_basic_bin[] = {
#include "ps4_shader_basic_p.h"
};

extern EmbeddedCsShader cs_setint;
extern EmbeddedVsShader vs_basic ;
extern EmbeddedPsShader ps_basic ;

void InitShaders();
/*
template<typename tShader> tShader* loadShaderFromFile(const char *path,
sce::Gnmx::Toolkit::Allocators &alloc)
{
        BindEmbeddedShader<tShader> binder;
        memset(&binder, 0, sizeof(binder));

        void *binary;
        uint32_t size;
        if (acquireFileContents(binary, size, path))
        {
                binder.embedded.m_source = static_cast<uint32_t*>(binary);
                binder.embedded.initializeWithAllocators(&alloc);
                free(binary);
        }

        return binder.embedded.m_shader;
        }*/