
#pragma once
#include "common.h"
#include "component.h"
#include <glm\glm.hpp>

namespace Components {

class CmShipdriver : public Component {
public:
  glm::vec3 velocity_;
  glm::vec3 acceleration_;
  void Accelerate(const float &f);
  void Brake(const float &f);
  void Steer(const float &f);

  CmShipdriver();
  ~CmShipdriver();

  void Update(double delta);
};
}
