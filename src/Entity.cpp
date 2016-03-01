#include "Entity.h"
#include <algorithm>
#include <glm/gtx/transform.hpp>
Entity::Entity() {
  visible_ = true;
  scale_ = vec3(1.0f, 1.0f, 1.0f);
  position_ = vec3(0.0f, 0.0f, 0.0f);
  rotation_ = quat();
}

Entity::~Entity() {}

const vec3 Entity::GetScale() const { return scale_; }

const vec3 Entity::GetPosition() const { return position_; }

const quat Entity::GetRotation() const { return rotation_; }

const mat4 Entity::GetTranform() {
  mat4 scl = glm::scale(scale_);
  mat4 rot = glm::mat4_cast(rotation_);
  mat4 trn = translate(position_);
  transform_ = (trn * rot * scl);
  return transform_;
}

const bool Entity::IsVisible() const { return false; }

const std::string Entity::GetName() const { return std::string(); }

void Entity::SetTransform(const mat4 m4) {}

void Entity::SetScale(const vec3 &v3) { scale_ = v3; }

void Entity::SetPosition(const vec3 &v3) { position_ = v3; }

void Entity::SetRotation(const vec3 &v3) {}

void Entity::SetRotation(const quat &q) { rotation_ = q; }

void Entity::SetVisibility(const bool b) {}

void Entity::SetName(std::string const &name) {}

void Entity::Update(const double delta) {
  for (auto c : components_) {
    c->Update(delta);
  }
}

void Entity::Render() {
  for (auto c : components_) {
    c->Render();
  }
}

void Entity::AddComponent(Component &c) {
  components_.push_back(&c);
  c.SetParent(this);
}

void Entity::RemoveComponent(Component &c) {
  const auto position = std::find(components_.begin(), components_.end(), &c);
  if (position != components_.end()) {
    components_.erase(position);
  }
}

std::vector<Component *> Entity::GetComponents(std::string const &name) const {
  assert(false);
  std::vector<Component *> list;
  if (components_.size() < 1) {
    return list;
  }
  for (auto c : components_) {
    list.push_back(c);
  }
  return list;
}
