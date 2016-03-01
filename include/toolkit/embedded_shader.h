/* SCE CONFIDENTIAL
PlayStation(R)4 Programmer Tool Runtime Library Release 03.008.041
* Copyright (C) 2015 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _SCE_GNM_TOOLKIT_EMBEDDED_SHADER_H
#define _SCE_GNM_TOOLKIT_EMBEDDED_SHADER_H

#include <gnmx.h>
#include <gnm/gpumem.h>
#include <string.h>
#include <gnmx/shader_parser.h>
#include <algorithm>
#include "allocators.h"

namespace sce
{
	namespace Gnmx
	{
		namespace Toolkit
		{
			struct MemoryRequests;

			struct EmbeddedPsShader
			{
				const uint32_t *m_source;
				const char *m_name;
				Gnmx::PsShader *m_shader;
				Gnmx::InputOffsetsCache m_offsetsTable;

				SCE_GNM_API_DEPRECATED_MSG_NOINLINE("initializeWithAllocators")
				void addToMemoryRequests(Gnmx::Toolkit::MemoryRequests *memoryRequests) const;
				SCE_GNM_API_DEPRECATED_MSG_NOINLINE("initializeWithAllocators")
				void initializeWithMemoryRequests(Gnmx::Toolkit::MemoryRequests *memoryRequests);
				void initializeWithAllocators(Gnmx::Toolkit::Allocators *allocators);
			};

			struct EmbeddedCsShader
			{
				const uint32_t *m_source;
				const char *m_name;
				Gnmx::CsShader *m_shader;
				Gnmx::InputOffsetsCache m_offsetsTable;

				SCE_GNM_API_DEPRECATED_MSG_NOINLINE("initializeWithAllocators")
				void addToMemoryRequests(Gnmx::Toolkit::MemoryRequests *memoryRequests) const;
				SCE_GNM_API_DEPRECATED_MSG_NOINLINE("initializeWithAllocators")
				void initializeWithMemoryRequests(Gnmx::Toolkit::MemoryRequests *memoryRequests);
				void initializeWithAllocators(Gnmx::Toolkit::Allocators *allocators);
			};

			struct EmbeddedVsShader
			{
				const uint32_t *m_source;
				const char *m_name;
				Gnmx::VsShader *m_shader;
				void *m_fetchShader;
				Gnmx::InputOffsetsCache m_offsetsTable;

				SCE_GNM_API_DEPRECATED_MSG_NOINLINE("initializeWithAllocators")
				void addToMemoryRequests(Gnmx::Toolkit::MemoryRequests *memoryRequests) const;
				SCE_GNM_API_DEPRECATED_MSG_NOINLINE("initializeWithAllocators")
				void initializeWithMemoryRequests(Gnmx::Toolkit::MemoryRequests *memoryRequests);
				void initializeWithAllocators(Gnmx::Toolkit::Allocators *allocators);
			};

			struct EmbeddedEsShader
			{
				const uint32_t *m_source;
				const char *m_name;
				Gnmx::EsShader *m_shader;
				void *m_fetchShader;
				Gnmx::InputOffsetsCache m_offsetsTable;

				SCE_GNM_API_DEPRECATED_MSG_NOINLINE("initializeWithAllocators")
				void addToMemoryRequests(Gnmx::Toolkit::MemoryRequests *memoryRequests) const;
				SCE_GNM_API_DEPRECATED_MSG_NOINLINE("initializeWithAllocators")
				void initializeWithMemoryRequests(Gnmx::Toolkit::MemoryRequests *memoryRequests);
				void initializeWithAllocators(Gnmx::Toolkit::Allocators *allocators);
			};

			struct EmbeddedGsShader
			{
				const uint32_t *m_source;
				const char *m_gsName;
				const char *m_vsName;
				Gnmx::GsShader *m_shader;
				Gnmx::InputOffsetsCache m_offsetsTable;

				SCE_GNM_API_DEPRECATED_MSG_NOINLINE("initializeWithAllocators")
				void addToMemoryRequests(Gnmx::Toolkit::MemoryRequests *memoryRequests) const;
				SCE_GNM_API_DEPRECATED_MSG_NOINLINE("initializeWithAllocators")
				void initializeWithMemoryRequests(Gnmx::Toolkit::MemoryRequests *memoryRequests);
				void initializeWithAllocators(Gnmx::Toolkit::Allocators *allocators);
			};

			struct EmbeddedLsShader
			{
				const uint32_t *m_source;
				const char *m_name;
				Gnmx::LsShader *m_shader;
				void *m_fetchShader;
				Gnmx::InputOffsetsCache m_offsetsTable;

				SCE_GNM_API_DEPRECATED_MSG_NOINLINE("initializeWithAllocators")
				void addToMemoryRequests(Gnmx::Toolkit::MemoryRequests *memoryRequests) const;
				SCE_GNM_API_DEPRECATED_MSG_NOINLINE("initializeWithAllocators")
				void initializeWithMemoryRequests(Gnmx::Toolkit::MemoryRequests *memoryRequests);
				void initializeWithAllocators(Gnmx::Toolkit::Allocators *allocators);
			};

			struct EmbeddedHsShader
			{
				const uint32_t *m_source;
				const char *m_name;
				Gnmx::HsShader *m_shader;
				Gnmx::InputOffsetsCache m_offsetsTable;

				SCE_GNM_API_DEPRECATED_MSG_NOINLINE("initializeWithAllocators")
				void addToMemoryRequests(Gnmx::Toolkit::MemoryRequests *memoryRequests) const;
				SCE_GNM_API_DEPRECATED_MSG_NOINLINE("initializeWithAllocators")
				void initializeWithMemoryRequests(Gnmx::Toolkit::MemoryRequests *memoryRequests);
				void initializeWithAllocators(Gnmx::Toolkit::Allocators *allocators);
			};

			struct EmbeddedCsVsShader
			{
				const uint32_t *m_source;
				const char *m_csName;
				const char *m_vsName;
				Gnmx::CsVsShader *m_shader;
				void *m_fetchShaderVs;
				void *m_fetchShaderCs;
				Gnmx::InputOffsetsCache m_offsetsTableVs;
				Gnmx::InputOffsetsCache m_offsetsTableCs;

				SCE_GNM_API_DEPRECATED_MSG_NOINLINE("initializeWithAllocators")
				void addToMemoryRequests(Gnmx::Toolkit::MemoryRequests *memoryRequests) const;
				SCE_GNM_API_DEPRECATED_MSG_NOINLINE("initializeWithAllocators")
				void initializeWithMemoryRequests(Gnmx::Toolkit::MemoryRequests *memoryRequests);
				void initializeWithAllocators(Gnmx::Toolkit::Allocators *allocators);
			};

			struct EmbeddedShaders
			{
				EmbeddedCsShader **m_embeddedCsShader;
				uint32_t           m_embeddedCsShaders;
				EmbeddedPsShader **m_embeddedPsShader;
				uint32_t           m_embeddedPsShaders;
				EmbeddedVsShader **m_embeddedVsShader;
				uint32_t           m_embeddedVsShaders;
				EmbeddedEsShader **m_embeddedEsShader;
				uint32_t           m_embeddedEsShaders;
				EmbeddedGsShader **m_embeddedGsShader;
				uint32_t           m_embeddedGsShaders;
				EmbeddedLsShader **m_embeddedLsShader;
				uint32_t           m_embeddedLsShaders;
				EmbeddedHsShader **m_embeddedHsShader;
				uint32_t           m_embeddedHsShaders;
				EmbeddedCsVsShader **m_embeddedCsVsShader;
				uint32_t           m_embeddedCsVsShaders;
				SCE_GNM_API_DEPRECATED_MSG_NOINLINE("initializeWithAllocators")
				void addToMemoryRequests(Gnmx::Toolkit::MemoryRequests *memoryRequests) const;
				SCE_GNM_API_DEPRECATED_MSG_NOINLINE("initializeWithAllocators")
				void initializeWithMemoryRequests(Gnmx::Toolkit::MemoryRequests *memoryRequests);
				void initializeWithAllocators(Gnmx::Toolkit::Allocators *allocators);
			};
		}
	}
}
#endif // _SCE_GNM_TOOLKIT_EMBEDDED_SHADER_H
