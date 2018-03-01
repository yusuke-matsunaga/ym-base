
/// @file RandSamplerSimpleTest.cc
/// @brief RandSamplerSimpleTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "ym/RandSamplerSimple.h"
#include "ym/RandGen.h"


BEGIN_NAMESPACE_YM

int
RandSamplerSimpleTest(int argc,
		      char** argv)
{
  int w_array[] = { 2, 4, 10, 1 };
  int num_data = sizeof(w_array) / sizeof(int);

  RandSamplerSimple rs(num_data, w_array);

  RandGen rg;

  int sample_num = 100000;

  vector<int> count(num_data, 0);
  for (int i = 0; i < sample_num; ++ i) {
    int pos = rs.get_sample(rg);
    ASSERT_COND( pos < num_data );
    ++ count[pos];
  }

  for (int i = 0; i < num_data; ++ i) {
    cout << "Data#" << i << ": " << count[i] << endl;
  }

  return 0;
}

END_NAMESPACE_YM

int
main(int argc,
     char** argv)
{
  return nsYm::RandSamplerSimpleTest(argc, argv);
}
