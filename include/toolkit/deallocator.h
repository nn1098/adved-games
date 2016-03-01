﻿/* SCE CONFIDENTIAL
PlayStation(R)4 Programmer Tool Runtime Library Release 03.008.041
* Copyright (C) 2015 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _SCE_GNM_TOOLKIT_DEALLOCATOR_H_
#define _SCE_GNM_TOOLKIT_DEALLOCATOR_H_

#include "allocator.h"
#include <sys/dmem.h>

namespace sce
{
	namespace Gnmx
	{
		namespace Toolkit
		{
			struct Deallocator
			{
			private:
				IAllocator m_iallocator;
				enum {kMaximumAllocations = 8192};
				void *m_allocation[kMaximumAllocations];
				uint32_t m_allocations;
			public:
				void deallocate();
				Deallocator(IAllocator iallocator);
				~Deallocator();
				void *allocate(uint32_t size, uint32_t alignment);
				void *allocate(Gnm::SizeAlign sizeAlign);
			};

			IAllocator GetInterface(Deallocator *stackAllocator);
		}
	}
}

#endif /* _SCE_GNM_TOOLKIT_STACK_ALLOCATOR_H_ */
