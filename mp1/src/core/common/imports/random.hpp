#pragma once
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>
#include <stdexcept>
#include "casts.hpp"
#include "containers.hpp"
#include "misc.hpp"

namespace csopesy {
  // === Random Type Aliases ===
  using mt19937 = std::mt19937;
  using random_device = std::random_device;
  using uniform_int = std::uniform_int_distribution<>;
  using uniform_real = std::uniform_real_distribution<float>;

  // === Random Utilities ===
  struct Random {
    mt19937 rng;

    Random() : rng(random_device{}()) {}

    int num(int min, int max) {
      uniform_int dist(min, max);
      return dist(rng);
    }

    uint num(uint min, uint max) {
      uniform_int dist(min, max);
      return dist(rng);
    }

    float real(float min, float max) {
      uniform_real dist(min, max);
      return dist(rng);
    }

    template <typename Type>
    Type pick(const vector<Type>& list) {
      if (list.empty()) throw std::runtime_error("Cannot pick from empty list.");
      return list[num(0, cast_int(list.size()) - 1)];
    }

    template <typename Type>
    vector<Type>& shuffle(vector<Type>& list) {
      std::shuffle(list.begin(), list.end(), rng);
      return list;
    }
  };


  // Global instance of the Random class (since seeding needs to be initialized)
  inline Random random; 
}
