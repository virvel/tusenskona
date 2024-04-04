#include "../../../libDaisy/src/dev/trill/Trill.h"
#include <tuple>

#define N_TRILL 36 // NUM PINS !! Not enough pins on hardware

enum class TrillCtrl {
  A, 
  B, 
  C, 
  D
};

// add scan settings here
struct TrillConfig {
  int prescaler;
  float threshold;
};


class TrillHandler {
  // unsigned A_offset = 9, 
  //          B_offset = A_offset + 8, 
  //          C_offset = B_offset + 9, 
  //          D_offset = C_offset + 10;

  float A[9]{0};  // 0 - 8
  float B[8]{0};  // 9 - 16
  float C[9]{0};  // 17 - 25
  float D[10]{0}; // 26 - 29 !! not enough pins on trill craft 

  float A_sum, B_sum, C_sum, D_sum;

  Trill* m_trill;


  public: 
    TrillHandler();
    int init(Trill* trill);
    int init(Trill* trill, unsigned i2c_bus);
    void config(TrillConfig);
    void poll();
    void calcSum();
    int getTop(TrillCtrl);
    int getBottom(TrillCtrl);
    int getVertical();
    float max(TrillCtrl);
    float min(TrillCtrl);

    std::tuple<float, int> globalMax();
    std::tuple<float, int> globalMin();

    float getPinValue(unsigned pin);
};
