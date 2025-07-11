
/// @file RandSamplerSimpleTest.cc
/// @brief RandSamplerSimpleTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "ym/Range.h"
#include "ym/Timer.h"
#include <random>


BEGIN_NAMESPACE_YM

template<class RandSampler>
void
RandSamplerTest(
  RandSampler& rs,
  const std::vector<int>& w_array
)
{
  Timer timer;
  timer.start();

  int num_data = w_array.size();
  int accum = 0;
  for ( int w: w_array ) {
    accum += w;
  }

  std::mt19937 rg;

  int sample_num = accum * 100000;

  std::vector<int> count(num_data, 0);
  for ( int i: Range(sample_num) ) {
    int pos = rs(rg);
    ASSERT_COND( pos >= 0 && pos < num_data );
    ++ count[pos];
  }

  double err_accum = 0.0;
  for ( int i: Range(num_data) ) {
    std::cout << "Data#" << i << ": " << count[i] << std::endl;
    double diff = count[i] - w_array[i] * 100000;
    err_accum += diff * diff;
  }
  double sqerr = sqrt(err_accum / (num_data * 100000));
  std::cout << "Error: " << sqerr << std::endl;
  timer.stop();
  std::cout << "CPU time: " << timer.get_time() << std::endl
	    << std::endl;
}

END_NAMESPACE_YM

int
main(
  int argc,
  char** argv
)
{
  using namespace YM_NAMESPACE;

  std::vector<int> w_array{2, 4, 10, 1};
  std::cout << "discrete_distribution(small)" << std::endl;
  std::discrete_distribution<int> dd1(w_array.begin(), w_array.end());
  RandSamplerTest(dd1, w_array);

  std::vector<int> w_array2(100);
  for ( int i: Range(100) ) {
    w_array2[i] = i + 1;
  }

  std::cout << "discrete_distribution(large)" << std::endl;
  std::discrete_distribution<int> dd2(w_array2.begin(), w_array2.end());
  RandSamplerTest(dd2, w_array2);

  return 0;
}
