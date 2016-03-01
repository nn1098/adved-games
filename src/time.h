#pragma once
#include <chrono>
#include <string>

using namespace std;

struct Timer {
  chrono::high_resolution_clock::time_point start;
  chrono::high_resolution_clock::time_point end;
  Timer();
  void Start();
  void Stop();
  const chrono::high_resolution_clock::duration Duration();
  unsigned long long Duration_NS();
};

chrono::high_resolution_clock::time_point Now();

std::string NowIsoString();
std::string NowDateTimeString();
std::string NowTimeString();