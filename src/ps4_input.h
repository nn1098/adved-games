#pragma once

class PS4_Input {
public:
  static bool Init();
  static bool Update();
  static bool Shutdown();

protected:
private:
  PS4_Input() = delete;
  ~PS4_Input() = delete;
  // delete copy/move constructors & assign operators
  PS4_Input(PS4_Input const &) = delete;
  PS4_Input(PS4_Input &&) = delete;
  PS4_Input &operator=(PS4_Input const &) = delete;
  PS4_Input &operator=(PS4_Input &&) = delete;
};
