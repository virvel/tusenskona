#include "th.h"
#include <cmath>
#include <tuple>

TrillHandler::TrillHandler() {};

int TrillHandler::init(Trill trill) {
  m_trill = trill;
}

void TrillHandler::poll() {
  m_trill.readI2C();
  int i = 0;
  for (; i < 9; i++)  A[i]    = m_trill.rawData[i];
  for (; i < 17; i++) B[i-8]  = m_trill.rawData[i];
  for (; i < 26; i++) C[i-16] = m_trill.rawData[i];
  for (; i < 36; i++) D[i-25] = m_trill.rawData[i];
}

// Sum of field NORMALIZED to number of tongues on each.
void TrillHandler::calcSum() {
  float z = 0.f;
  for (auto &x: A) {
    z += x;
  };
  A_sum = z / 9.f;
  z = 0.f;

  for (auto &x: B) {
    z += x;
  }
  B_sum = z / 8.f;
  z = 0.f;

  for (auto &x: C) {
    z += x; 
  }
  C_sum = z / 9.f;
  z = 0.f;

  for (auto &x: D) {
    z += x; 
  }
  D_sum = z / 10.f;
};

// Get current active 'tongue' from the top
int TrillHandler::getTop(TrillCtrl field) {
  switch (field) {
    case TrillCtrl::A :
      for (int i = 8; i >= 0; i--) 
        if (A[i] != 0.f) 
          return i;

    case TrillCtrl::B :
      for (int i = 7; i >= 0; i--) 
        if (B[i] != 0.f)  return i;

    case TrillCtrl::C :
      for (int i = 8; i >= 0; i--) 
        if (C[i] != 0.f) return i;

    case TrillCtrl::D :
      for (int i = 9; i >= 0; i--) 
        if (D[i] != 0.f)  return i;

    default:
      break;
  }
}

// Get current active 'tongue' from the bottom
int TrillHandler::getBottom(TrillCtrl field) {
  switch (field) {
    case TrillCtrl::A :
      for (int i = 0; i < 9; i++) 
        if (A[i] != 0.f) return i;

    case TrillCtrl::B :
      for (int i = 0; i < 8; i++) 
        if (B[i] != 0.f)  return i;

    case TrillCtrl::C :
      for (int i = 0; i < 9; i++) 
        if (C[i] != 0.f) return i;

    case TrillCtrl::D :
      for (int i = 0; i < 10; i++) 
        if (D[i] != 0.f) return i;

    default:
      break;
  }
}

// Gets a fields highest value
float TrillHandler::max(TrillCtrl field) {
  float max = 0.f;
  switch (field) {
    case TrillCtrl::A :
      for (auto &x: A) 
        if (x > max) max = x;
      break;

    case TrillCtrl::B :
      for (auto &x: B) 
        if (x > max) max = x;
      break;

    case TrillCtrl::C :
      for (auto &x: C) 
        if (x > max) max = x;
      break;

    case TrillCtrl::D :
      for (auto &x: D) 
        if (x > max) max = x;
      break;

    default:
      break;
  }
  return max;
}

// Gets a fields smallest value (probably 0)
float TrillHandler::min(TrillCtrl field) {
  float min = INFINITY;
  switch (field) {
    case TrillCtrl::A :
      for (auto &x: A) 
        if (x < min) min = x;
      break;

    case TrillCtrl::B :
      for (auto &x: B) 
        if (x < min) min = x;
      break;

    case TrillCtrl::C :
      for (auto &x: C) 
        if (x < min) min = x;
      break;

    case TrillCtrl::D :
      for (auto &x: D) 
        if (x < min) min = x;
      break;

    default:
      break;
  }
  return min;
}

// Gets the maximum value and which index on entire board
std::tuple<float, int> TrillHandler::globalMax() {
  float max = 0.f;
  int idx = 0;
  int i = 0;
  for (float &x: m_trill.rawData) {
    if (x > max) {
      max = m_trill.rawData[i];
      idx = i;
    }
    i++;
  }

  return std::make_tuple(max, idx);
}

// Gets the minimum value and its index on entire board
std::tuple<float, int> TrillHandler::globalMin() {
  float min = INFINITY;
  int idx = 0;
  int i = 0;
  for (float &x: m_trill.rawData) {
    if (x < min) {
      min = m_trill.rawData[i];
      idx = i;
    }
    i++;
  }

  return std::make_tuple(min, idx);
}

// Gets which field is most active
int TrillHandler::getVertical() {
  float max = 0.f;
  int idx = 0; 
  calcSum();
  int i = 0;
  for (auto &x: {A_sum, B_sum, C_sum, D_sum}) {
    if (x > max) idx = i;
    i++;
  }
}




