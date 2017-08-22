#include <iostream>
#include <stdio.h>

#include "approx_counter.h"

approx_counter<int64_t> ctr;

// const auto COUNT = 524288; // (524288 + 2*524288)/3;
const auto COUNT = 1LL << 20;
float ratio = 0.0;

const auto ITERATIONS = 1000;
const int NTHREADS=4;

void foo() {
  for (int i = 0; i < COUNT; i++) {
    ++ctr;
  }
  ratio += (ctr() * 1.0) / (COUNT * NTHREADS);
}

int
main()
{
  auto truth = 0;
  auto approx = 0;
  for (auto it = 0; it < ITERATIONS; it++) {
    ctr.reset();
    std::thread * t[NTHREADS];
    for (auto i = 0; i < NTHREADS; i++) {
      t[i] = new std::thread(foo);
    }
    for (auto i = 0; i < NTHREADS; i++) {
      t[i]->join();
    }
    //    std::cout << ((float) ctr() - (float) COUNT * NTHREADS) / ((float) COUNT * NTHREADS) << std::endl;
    truth += COUNT * NTHREADS;
    approx += ctr();
  }
  
  std::cout << truth << ", " << approx << std::endl;
  std::cout << "ratio = " << ratio / (NTHREADS * ITERATIONS) << std::endl;
  std::cout << "diff = " << (truth - approx) << std::endl;
  std::cout << "diff = " << (1.0 * (truth - approx))/truth << std::endl;
  
  return 0;
}
