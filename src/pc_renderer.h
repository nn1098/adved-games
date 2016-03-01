#pragma once
#include <GL\glew.h>

struct Mesh;
namespace Components {
class CmCamera;
}

class PC_Renderer {
public:
  static bool Init();
  static bool Shutdown();
  static void ClearFrame();
  static void SetViewMatrix(const glm::mat4 &vm);
  static void RenderMesh(const Mesh &m, const glm::mat4 &modelMatrix);
  static GLuint loadTexture(const char * imagepath);
  static void DrawBoundingBox();
  static void LoadMesh(Mesh *msh);
  static void PreRender() { ClearFrame(); };
  static void PostRender(){};

protected:
  static glm::mat4 vp_;

private:
  PC_Renderer() = delete;
  ~PC_Renderer() = delete;
  // delete copy/move constructors & assign operators
  PC_Renderer(PC_Renderer const &) = delete;
  PC_Renderer(PC_Renderer &&) = delete;
  PC_Renderer &operator=(PC_Renderer const &) = delete;
  PC_Renderer &operator=(PC_Renderer &&) = delete;
};
