#include "../mersenne_twister.h"
#include "../basics.h"

int main()
{
  int j;
  mersenne_twister_seed(4357);
  for (j = 0; j < 1000; ++j) {
    print("%5f ", mersenne_twister_real());
    if (j % 8 == 7)
      print("\n");
  }
  return 0;
}
