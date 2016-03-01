#pragma once
#include <glm/glm.hpp>

struct Mesh;
namespace Components{
  class CmCamera;
}


class PS4_Renderer {
public:
  static bool Init();
  static bool Shutdown();
  static void ClearFrame();
  static void setCamera(Components::CmCamera* cm);
  static void SetViewMatrix(const glm::mat4& vm);
  static void RenderMesh(const Mesh& m, const glm::mat4& modelMatrix);
  static void LoadMesh(Mesh* msh);
  static void PreRender();
  static void PostRender();
  static void Flip();
protected:
private:
  PS4_Renderer() = delete;
  ~PS4_Renderer() = delete;
  // delete copy/move constructors & assign operators
  PS4_Renderer(PS4_Renderer const &) = delete;
  PS4_Renderer(PS4_Renderer &&) = delete;
  PS4_Renderer &operator=(PS4_Renderer const &) = delete;
  PS4_Renderer &operator=(PS4_Renderer &&) = delete;
};
