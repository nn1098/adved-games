#include "component_shipdriver.h"
#include "Entity.h"
using namespace glm;

/*
bool player_accelerate(const std::vector<std::string> &params) {
  // what direction is player facing?
  vec3 direction = normalize(GetForwardVector(player.GetRotation()));

  playerAcceleration += (0.1f * direction);
  // vec3 direction = {0, 0, 1.0f};
  // player.SetPosition(player.GetPosition() + (direction * 0.1f));
  return true;
}
bool player_brake(const std::vector<std::string> &params) {
  // playerVelocity += {0, 0, -1.0f};
  // player.SetPosition(player.GetPosition() + (direction * 0.1f));
  return true;
}
bool player_steer_left(const std::vector<std::string> &params) {
  // vec3 direction = {1.0f, 0, 0};
  player.SetRotation(player.GetRotation() * glm::normalize(glm::angleAxis(0.04f, vec3(0, 1.0, 0))));
  // player.SetPosition(player.GetPosition() + (direction * 0.1f));
  return true;
}
bool player_steer_right(const std::vector<std::string> &params) {
  player.SetRotation(player.GetRotation() *
                     glm::normalize(glm::angleAxis(-0.04f, vec3(0, 1.0, 0))));
  // vec3 direction = {-1.0f, 0, 0};
  // player.SetPosition(player.GetPosition() + (direction * 0.1f));
  return true;
}
*/

namespace Components {

CmShipdriver::CmShipdriver() : Component("ShipDriver") {

  velocity_ = vec3(0.0f, 0.0f, 0.0f);
  acceleration_ = vec3(0.0f, 0.0f, 0.0f);
}

CmShipdriver::~CmShipdriver() {}

void CmShipdriver::Update(double delta) {
  const auto playerspeed = glm::length(velocity_);
  // const auto playerspeedReal = glm::length(velocity_) / delta;
 
  // air resistance
  if (playerspeed != 0.0f) {
    acceleration_ += -0.5f * playerspeed * normalize(velocity_);
    if (abs(velocity_.y) < 0.0001f) {
      velocity_.y = 0.0f;
    }
    if (abs(velocity_.x) < 0.0001f) {
      velocity_.x = 0.0f;
    }
    if (abs(velocity_.z) < 0.001f) {
      velocity_.z = 0.0f;
    }
  }
  //velocity integration
  velocity_ += acceleration_ * (float)delta;
  acceleration_ = vec3(0.0f);
  Ent_->SetPosition(Ent_->GetPosition() + velocity_);
}

void CmShipdriver::Accelerate(const float &f) {
  vec3 direction = normalize(GetForwardVector(Ent_->GetRotation()));
  acceleration_ += (0.1f * direction * f);
}
void CmShipdriver::Brake(const float &f) {
  if (glm::length(velocity_) > 0) {
    acceleration_ += (-0.1f * velocity_ * f);
  }
}
void CmShipdriver::Steer(const float &f) {
  Ent_->SetRotation(Ent_->GetRotation() *
                    glm::normalize(glm::angleAxis(0.04f * f, vec3(0, 1.0, 0))));
}
}
