#include "groundplane.h"
#include "mesh.h"
#include "renderer.h"
#include "pc_renderer.h"
#include <glm/gtx/transform.hpp>
static Mesh plane_mesh;

void GroundPlane::Init() {
  plane_mesh = Mesh();
  plane_mesh.hasUvs = true;
  plane_mesh.hasNormals = true;
  plane_mesh.loadedLocal = false;
  plane_mesh.hastexture = false;
  plane_mesh.loadedMain = true;
  plane_mesh.hasColours = false;
  plane_mesh.hasIndicies = true;
  plane_mesh.numVerts = 4;
  plane_mesh.vertexData = {
      {-1.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, -1.0f}, {1.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 1.0f}};
  plane_mesh.indices = {0, 2, 1, 0, 3, 2};
 // plane_mesh.colours = { 1, 1, 0, 0 };
  plane_mesh.uvs = {vec2{0,0}, vec2{1,0}, vec2{0,1}, vec2{1,1}};
  plane_mesh.shaderPref = "basic";
	//auto floortex =  PC_Renderer::loadTexture("..\res\textures\floor.bmp");
	//glBindTexture(GL_TEXTURE_2D, floortex);
  Renderer::LoadMesh(&plane_mesh);
}

void GroundPlane::Shutdown() {}
void GroundPlane::Update() {}

void GroundPlane::Render(const float &x, const float &z) {

  glm::mat4 mm;
  mm = glm::translate(vec3(x, 0, z)) * glm::scale(vec3(50.0f));

  Renderer::RenderMesh(plane_mesh, mm);
}