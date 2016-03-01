#pragma once
#include "common.h"
#include "ps4_memory.h"

class PS4_Platform {
public:
  static bool Init();
  static bool Shutdown();
  static const size_t kOnionMemorySize = 16 * 1024 * 1024;
  static const size_t kGarlicMemorySize = 64 * 1024 * 1024;
  static const uint32_t kCueRingEntries = 64;
  static const size_t kDcbSizeInBytes = 2 * 1024 * 1024;
  static const size_t kCcbSizeInBytes = 256 * 1024;
  static const size_t sceLibcHeapSize = 64 * 1024 * 1024;

  /*
  static LinearAllocator onionAllocator;
  static LinearAllocator garlicAllocator;
  */
private:
  PS4_Platform() = delete;
  ~PS4_Platform() = delete;
  // delete copy/move constructors & assign operators
  PS4_Platform(PS4_Platform const &) = delete;
  PS4_Platform(PS4_Platform &&) = delete;
  PS4_Platform &operator=(PS4_Platform const &) = delete;
  PS4_Platform &operator=(PS4_Platform &&) = delete;
};