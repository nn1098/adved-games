#pragma once
#include "component_track.h"
#include "entity.h"
#include "renderer.h"
#include "resource.h"

const vec2 colp[] = {
    vec2(-2.071618, -1.226097), vec2(-2.303969, -0.839075), vec2(-2.496487, -0.689960),
    vec2(-2.300414, -0.729397), vec2(-2.425100, -0.518676), vec2(-2.247029, -0.609730),
    vec2(-2.326826, -0.371876), vec2(-2.212482, -0.243004), vec2(-2.070809, -0.384173),
    vec2(-2.167996, -0.493420), vec2(-1.711310, -0.118559), vec2(-1.803928, 0.058703),
    vec2(-1.306556, -0.003862), vec2(-1.298821, 0.195988),  vec2(-1.060996, -0.086372),
    vec2(-1.181897, -0.026097), vec2(-0.960307, 0.086433),  vec2(-0.827677, -0.006110),
    vec2(-0.932377, -0.176515), vec2(-0.777771, -0.262854), vec2(-0.705436, -0.076393),
    vec2(-0.602528, -0.305806), vec2(-0.427835, -0.315133), vec2(-0.258071, -0.303464),
    vec2(-0.093687, -0.279053), vec2(0.066354, -0.247937),  vec2(0.223470, -0.214447),
    vec2(0.182374, -0.018714),  vec2(0.499947, 0.044235),   vec2(0.661511, 0.069187),
    vec2(0.995615, -0.099576),  vec2(1.291509, -0.117339),  vec2(1.431055, -0.155128),
    vec2(1.497481, 0.033519),   vec2(1.656146, -0.036844),  vec2(1.563212, -0.213941),
    vec2(1.801492, -0.382700),  vec2(1.688090, -0.289498),  vec2(1.936916, -0.235526),
    vec2(1.902271, -0.490242),  vec2(2.054495, -0.360521),  vec2(1.990559, -0.607647),
    vec2(2.157625, -0.497698),  vec2(2.064698, -0.733494),  vec2(2.244804, -0.646535),
    vec2(2.309601, -0.808819),  vec2(2.146165, -1.002659),  vec2(2.345268, -0.983738),
    vec2(2.339964, -1.167169),  vec2(2.104417, -1.247025),  vec2(2.279173, -1.344289),
    vec2(2.030826, -1.338259),  vec2(2.160188, -1.490790),  vec2(1.923827, -1.401345),
    vec2(1.999555, -1.586454),  vec2(1.826209, -1.631925),  vec2(1.791269, -1.435000),
    vec2(1.655742, -1.645298),  vec2(1.494612, -1.648492),  vec2(1.642754, -1.445721),
    vec2(1.339896, -1.658111),  vec2(1.484066, -1.448770),  vec2(1.319809, -1.459122),
    vec2(1.189173, -1.678838),  vec2(0.990164, -1.517636),  vec2(1.154106, -1.481937),
    vec2(1.040992, -1.711069),  vec2(0.894738, -1.754816),  vec2(0.749443, -1.809338),
    vec2(0.527331, -1.684459),  vec2(0.450851, -1.929641),  vec2(0.234695, -1.793384),
    vec2(-0.062233, -1.868385), vec2(-0.027113, -2.065277), vec2(-0.367170, -1.904773),
    vec2(-0.522362, -2.106280), vec2(-0.823936, -1.885480), vec2(-0.856947, -2.082737),
    vec2(-0.964164, -1.854927), vec2(-1.193038, -1.973915), vec2(-1.189877, -1.723773),
    vec2(-1.438494, -1.712288), vec2(-1.522798, -1.583308), vec2(-1.270553, -1.603679),
    vec2(-1.474522, -1.332232), vec2(-1.361379, -1.465225), vec2(-1.615384, -1.474210),
    vec2(-1.610175, -1.220169), vec2(-1.724058, -1.384579), vec2(-1.760107, -1.133268),
    vec2(-1.849639, -1.312109), vec2(-1.914676, -1.067296), vec2(-1.998721, -1.039855),
    vec2(-1.988818, -1.253046), vec2(-2.230279, -1.164456), vec2(-2.386486, -1.062967),
    vec2(-2.138351, -0.986835), vec2(-2.496174, -0.894369), vec2(-2.244348, -0.922266),
    vec2(-2.086550, -0.128530), vec2(-1.950384, -0.027647), vec2(-1.960602, -0.283892),
    vec2(-1.840161, -0.194534), vec2(-1.647509, 0.129611),  vec2(-1.479096, 0.178727),
    vec2(-1.577037, -0.057562), vec2(-1.440078, -0.017430), vec2(-1.119474, 0.163912),
    vec2(-0.570868, -0.108327), vec2(-0.427612, -0.115133), vec2(-0.279182, -0.104581),
    vec2(-0.127454, -0.081924), vec2(0.026530, -0.051942),  vec2(0.340072, 0.014481),
    vec2(0.378901, -0.181713),  vec2(0.533571, -0.152918),  vec2(0.824811, 0.088382),
    vec2(0.688120, -0.129035),  vec2(0.990586, 0.100361),   vec2(0.842440, -0.110840),
    vec2(1.159411, 0.100009),   vec2(1.330373, 0.078849),   vec2(1.145954, -0.099538),
    vec2(1.804018, -0.126524),  vec2(2.118215, -0.866884),  vec2(2.142950, -1.132740),
    vec2(0.830127, -1.565540),  vec2(0.602051, -1.869977),  vec2(0.675862, -1.623366),
    vec2(0.295272, -1.983989),  vec2(0.380941, -1.742258),  vec2(0.135630, -2.029800),
    vec2(0.087216, -1.835748),  vec2(-0.191705, -2.090155), vec2(-0.357080, -2.104518),
    vec2(-0.213754, -1.891374), vec2(-0.688720, -2.100309), vec2(-0.521750, -1.906281),
    vec2(-0.674851, -1.900790), vec2(-1.026814, -2.044861), vec2(-1.339609, -1.856364),
    vec2(-1.088644, -1.803322)};
const int coli[] = {
    28,  114, 78,  140, 50,  126, 12,  104, 32,  123, 74,  134, 30,  117, 9,   10,  73,  133, 77,
    139, 65,  66,  85,  86,  37,  38,  11,  103, 21,  109, 69,  128, 75,  136, 92,  93,  80,  141,
    71,  130, 1,   94,  1,   95,  7,   8,   58,  59,  72,  131, 34,  35,  91,  94,  31,  120, 76,
    137, 70,  129, 15,  16,  17,  18,  82,  83,  13,  107, 14,  108, 47,  125, 45,  46,  27,  26,
    134, 135, 56,  58,  138, 75,  61,  64,  95,  96,  46,  48,  121, 122, 20,  19,  68,  69,  123,
    31,  124, 39,  81,  142, 52,  50,  84,  81,  127, 65,  90,  88,  115, 27,  55,  56,  129, 127,
    53,  55,  79,  77,  6,   4,   114, 29,  109, 110, 120, 118, 125, 44,  102, 9,   105, 14,  54,
    52,  43,  45,  110, 111, 87,  89,  100, 101, 117, 119, 26,  25,  10,  6,   137, 78,  139, 138,
    118, 116, 92,  90,  130, 132, 131, 70,  140, 80,  8,   100, 4,   2,   23,  22,  88,  85,  132,
    74,  116, 115, 63,  62,  35,  124, 96,  98,  42,  40,  24,  23,  126, 47,  29,  30,  62,  60,
    133, 72,  36,  33,  108, 17,  19,  15,  103, 102, 59,  61,  135, 76,  66,  63,  39,  41,  119,
    121, 112, 113, 101, 12,  83,  87,  128, 71,  141, 82,  25,  24,  5,   7,   142, 79,  64,  67,
    113, 28,  67,  68,  48,  49,  107, 106, 40,  37,  97,  93,  57,  54,  38,  36,  3,   5,   49,
    51,  89,  91,  98,  3,   22,  20,  136, 73,  104, 105, 99,  97,  2,   99,  60,  57,  86,  84,
    18,  21,  16,  13,  41,  43,  33,  32,  122, 34,  111, 112, 51,  53,  106, 11,  44,  42
};

Components::CmTrack::CmTrack() : Component("CmTrack") {}

Components::CmTrack::~CmTrack() {}
/*
bool plane::sphereCollide(const vec3 &pos, const float &radius) {
  vec2 LineP1 = p1;
  vec2 LineP2 = p2;
  vec2 CircleCentre = vec2(pos.x, pos.z);
  float Radius = radius;

  // Input
  // LineP1	Point	First point describing the line
  // LineP2	Point	Second point describing the line
  // CircleCentre	Point	The centre of the circle
  //  Radius	Floating - point	The circle's radius
  // Output
  //  The point(s) of the collision, or null if no collision exists.
  //  Method

  // Transform to local coordinates
  vec2 LocalP1 = LineP1 - CircleCentre;
  vec2 LocalP2 = LineP2 - CircleCentre;
  // Precalculate this value. We use it often
  vec2 P2MinusP1 = LocalP2 - LocalP1;

  float a = (P2MinusP1.x) * (P2MinusP1.x) + (P2MinusP1.y) * (P2MinusP1.y);
  float b = 2.0f * ((P2MinusP1.x * LocalP1.x) + (P2MinusP1.y * LocalP1.y));
  float c = (LocalP1.x * LocalP1.x) + (LocalP1.y * LocalP1.y) - (Radius * Radius);

  float delta = b * b - (4.0f * a * c);

  float SquareRootDelta = 0;

  if (delta < 0) { // No intersection
    return false;
  } else if (delta == 0) { // One intersection
    auto u = -b / (2.0f * a);
    vec2 ret = LineP1 + (u * P2MinusP1);
    auto gg =  glm::length(ret);
    return (!isnan(gg) > 0);
  } else if (delta > 0) {
    SquareRootDelta = sqrt(delta);
  }

  float u1 = ((-b) + SquareRootDelta) / (2.0f * a);
  float u2 = ((-b) - SquareRootDelta) / (2.0f * a);

  if (glm::length(LineP1 + (u1 * P2MinusP1)) > 0) {
    return true;
  } else if (glm::length(LineP1 + (u2 * P2MinusP1)) > 0) {
    return true;
  }
  return false;
}
*/
bool plane::sphereCollide(const mat4 &trn, const vec3 &pos, const float &radius) {
  vec4 pt1 = (trn * vec4(p1.x, 0, p1.y, 1.0f));
  vec4 pt2 = (trn * vec4(p2.x, 0, p2.y, 1.0f));

  float x1 = pt1.x;
  float y1 = pt1.z;
  float x2 = pt2.x;
  float y2 = pt2.z;
  float cx = pos.x;
  float cy = pos.z;
  float cr = radius;

  float dx = x2 - x1;
  float dy = y2 - y1;
  float a = dx * dx + dy * dy;
  float b = 2 * (dx * (x1 - cx) + dy * (y1 - cy));
  float c = cx * cx + cy * cy;
  c += x1 * x1 + y1 * y1;
  c -= 2 * (cx * x1 + cy * y1);
  c -= cr * cr;
  float bb4ac = b * b - 4 * a * c;

  if (bb4ac < 0) {
    return false; // No collision
  } else {
    // return true; // Collision

    float mu = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
    float ix1 = x1 + mu * (dx);
    float iy1 = y1 + mu * (dy);
    mu = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
    float ix2 = x1 + mu * (dx);
    float iy2 = y1 + mu * (dy);

    // The intersection points
    // ellipse(ix1, iy1, 10, 10);
    // ellipse(ix2, iy2, 10, 10);

    float testX;
    float testY;
    // Figure out which point is closer to the circle
    if (glm::length(vec2(x1, y1) - vec2(cx, cy)) < glm::length(vec2(x2, y2) - vec2(cx, cy))) {
      testX = x2;
      testY = y2;
    } else {
      testX = x1;
      testY = y1;
    }

    if (glm::length(vec2(testX, testY) - vec2(ix1, iy1)) <
            glm::length(vec2(x1, y1) - vec2(x2, y2)) ||
        glm::length(vec2(testX, testY) - vec2(ix2, iy2)) <
            glm::length(vec2(x1, y1) - vec2(x2, y2))) {
      return true;
    } else {
      return false;
    }
  }
}

plane::plane(vec2 ip1, vec2 ip2) {
  if (ip1.x == ip2.x && ip1.y == ip2.y) {
    assert(false);
  }
  p1 = ip1;
  p2 = ip2;
}

std::vector<plane> Components::CmTrack::planes;

void Components::CmTrack::SetMesh(Mesh *msh) {
  mesh_ = msh;
  Mesh *edge = nullptr;
  /*
  if (mesh_->objGroupName == "trackEdgePlane") {
    edge = mesh_;
  } else {
    for (auto &m : mesh_->subMeshes) {
      if (m->objGroupName == "trackEdgePlane") {
        edge = m;
        break;
      }
    }
  }
  if (edge == nullptr) {
    LOG(logERROR) << "Track has no colliisons";
    return;
  }
  if (!edge->hasLines) {
    LOG(logERROR) << "Track collision mesh has no lines!";
    return;
  }
  std::vector<uint32_t> *lns = &edge->lines;
  const auto lnsCount = lns->size() / 2;
  LOG(logINFO) << "Parsing Track Collision: " << lnsCount << " planes";
  planes.clear();

  for (size_t i = 0; i < lnsCount; i++) {

    vec2 ip1 = vec2(edge->vertexData[i * 2].x, edge->vertexData[i * 2].z);
    vec2 ip2 = vec2(edge->vertexData[(i * 2) + 1].x, edge->vertexData[(i * 2) + 1].z);
    if (ip1.x == ip2.x && ip1.y == ip2.y) {

    } else {
      plane p = plane(ip1, ip2);
      planes.push_back(p);
    }
  }
  */
  for (size_t i = 0; i < 142; i++) {

    // vec2 ip1 = vec2(edge->vertexData[i * 2].x, edge->vertexData[i * 2].z);
    // vec2 ip2 = vec2(edge->vertexData[(i * 2) + 1].x, edge->vertexData[(i * 2) + 1].z);
    vec2 ip1 = colp[i * 2];
    vec2 ip2 = colp[(i * 2) + 1];

    if (ip1.x == ip2.x && ip1.y == ip2.y) {

    } else {
      plane p = plane(ip1, ip2);
      planes.push_back(p);
    }
  }

  LOG(logINFO) << "Parsing Track Collision: ";
  return;
}





void Components::CmTrack::SetMesh(const std::string &meshname) {
  SetMesh(Storage<Mesh>::Get(meshname));
}

void Components::CmTrack::Render() {
  if (!mesh_->loadedLocal) {
    Renderer::LoadMesh(mesh_);
  }
  Renderer::RenderMesh(*mesh_, Ent_->GetTranform());
  for (auto &m : mesh_->subMeshes) {
    if (!m->loadedLocal) {
      Renderer::LoadMesh(m);
    }
    Renderer::RenderMesh(*m, Ent_->GetTranform());
  }
}
