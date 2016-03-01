#pragma once
#include "assert.h"
#include "log.h"
#include <stdint.h>

#if defined(_PLATFORM_X64) || defined(_PLATFORM_WIN32)

#define _RENDER_LIB_OGL
#define _INPUT_LIB_GLFW
#define _WINDOW_LIB_GLFW
#define _MATH_LIB_GLM
#define DEFAULT_RESOLUTION_X 1280
#define DEFAULT_RESOLUTION_Y 720
#define DEFAULT_RESOLUTION DEFAULT_RESOLUTION_X, DEFAULT_RESOLUTION_Y
#elif defined(_PLATFORM_PS4)
#define _RENDER_LIB_PS4
#define _INPUT_LIB_PS4
#define _WINDOW_LIB_PS4
#define _WINDOW_LIB_PS4
#define _MATH_LIB_PS4
// typedef unsigned long size_t;
#else

#endif

// TODO: const exp this
#define ALIGN_VALUE(val, alignment) ((val + alignment - 1) & ~(alignment - 1))
#define stackalloc(_size) alloca(ALIGN_VALUE(_size, 16))

template <typename T> inline T alignToPow2(const T size, const T alignment) {
  return (size + alignment - T(1)) & (~(alignment - T(1)));
}

template <typename T> inline T alignTo(const T size, const T alignment) {
  return ((size + alignment - T(1)) / alignment) * alignment;
}
/*

template<class InputIt, class T>
InputIt find(InputIt first, InputIt last, const T& value)
{
  for (; first != last; ++first) {
    if (*first == value) {
      return first;
    }
  }
  return last;
}

template<class InputIt, class UnaryPredicate>
InputIt find_if(InputIt first, InputIt last, UnaryPredicate p)
{
  for (; first != last; ++first) {
    if (p(*first)) {
      return first;
    }
  }
  return last;
}*/