#pragma once

#include "common.h"
#include "component.h"
#include "resource.h"
#include "mesh.h"

struct Mesh;

struct plane {
  vec2 p1;
  vec2 p2;
  bool sphereCollide(const mat4 &trn, const vec3 &pos, const float &radius);
  plane(vec2 p1, vec2 p2);
};

namespace Components {

  class CmTrack : public Component {
  protected:
    // RenderObject* _ro;
    Mesh *mesh_;

  public:
    CmTrack();
    ~CmTrack();
    static std::vector<plane> planes;
    Mesh *getMesh();
    void SetMesh(Mesh *msh);
    void SetMesh(const std::string &meshname);


    void Render();
  };
}
