#include "common.h"
#include "ps4_renderer.h"
#include "ps4_video.h"
#include <video_out.h>
int PS4_Video::videoOutHandle;

bool PS4_Video::Init() { return true; }

bool PS4_Video::Resize() { return false; }

bool PS4_Video::Swap() {
  PS4_Renderer::Flip();
  return true;
}

bool PS4_Video::Shutdown() { return false; }
