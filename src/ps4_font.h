#include <glm/glm.hpp>

class PS4_Font {
public:
  static bool Init();
  static bool Shutdown();
  static void Draw(const uint16_t &fontsizes, const char *text, const glm::vec2 &position,
    const glm::vec4 &color);
  static void Render();
  static void Clear();

protected:
private:
  PS4_Font() = delete;
  ~PS4_Font() = delete;
  // delete copy/move constructors & assign operators
  PS4_Font(PS4_Font const &) = delete;
  PS4_Font(PS4_Font &&) = delete;
  PS4_Font &operator=(PS4_Font const &) = delete;
  PS4_Font &operator=(PS4_Font &&) = delete;
};
