#pragma once

class PS4_Video {
public:
  static bool Init();
  static bool Resize();
  static bool Swap();
  static bool Shutdown();
  static int videoOutHandle;
protected:
private:
  PS4_Video() = delete;
  ~PS4_Video() = delete;
  // delete copy/move constructors & assign operators
  PS4_Video(PS4_Video const &) = delete;
  PS4_Video(PS4_Video &&) = delete;
  PS4_Video &operator=(PS4_Video const &) = delete;
  PS4_Video &operator=(PS4_Video &&) = delete;
};
