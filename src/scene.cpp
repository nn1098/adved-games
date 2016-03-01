#include "entity.h"
#include "scene.h"
#include "renderer.h"
#include "component_camera.h"
std::vector<Entity *> Scene::scene_list;
Components::CmCamera *Scene::activeCamera_;
Components::FollowCamera *Scene::playerCamera_;

void Scene::SetActiveCamera(Components::FollowCamera *fc) { playerCamera_ = fc; }

void Scene::SetActiveCamera(Components::CmCamera *c) { activeCamera_ = c; }

void Scene::Update(double delta) {
  for (auto &e : scene_list) {
    e->Update(delta);
  }
}

void Scene::Render(double delta) {

  // should really be doing sorting and culling
  // Calulate visibility
  auto cc = playerCamera_->getViewMatrix();
  Renderer::SetViewMatrix(playerCamera_->getViewMatrix());

  for (auto &e : scene_list) {
    e->Render();
  }
}