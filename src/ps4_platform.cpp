#include "ps4_memory.h"
#include "ps4_platform.h"

/*
LinearAllocator PS4_Platform::onionAllocator;
LinearAllocator PS4_Platform::garlicAllocator;
*/
bool PS4_Platform::Init() {
  /*
  int ret = 0;

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
  if (ret != SCE_OK) {
    return false;
  }
  return true;
  */
  return true;
}

bool PS4_Platform::Shutdown() { return false; }
