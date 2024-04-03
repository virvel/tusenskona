#include "../../../libDaisy/src/dev/trill/Trill.h"
#include <tuple>

#define N_TRILL 36 // NUM PINS

enum class TrillCtrl {
  A, 
  B, 
  C, 
  D
};


class TrillHandler {
  // unsigned A_offset = 9, 
  //          B_offset = A_offset + 8, 
  //          C_offset = B_offset + 9, 
  //          D_offset = C_offset + 10;

  float A[9]; 
  float B[8]; 
  float C[9]; 
  float D[10];

  float A_sum, B_sum, C_sum, D_sum;

  Trill* m_trill;

  TrillHandler();
  int init(Trill* trill);

  public: 
    void poll();
    void calcSum();
    int getTop(TrillCtrl);
    int getBottom(TrillCtrl);
    int getVertical();
    float max(TrillCtrl);
    float min(TrillCtrl);

    std::tuple<float, int> globalMax();
    std::tuple<float, int> globalMin();
};
