#pragma once
#include "common.h"
#include <string>

class Entity;
class Component {
protected:
  // Parent Entity
  Entity *Ent_;
  bool active_;

public:
  Component(const std::string &token);
  virtual ~Component();
  virtual void Update(double delta){};
  virtual void Render(){};
  bool IsActive();
  void SetActive(bool b);
  void SetParent(Entity *p);
  Entity *GetParent();
};