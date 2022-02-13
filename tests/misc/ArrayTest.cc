
/// @file ArrayTest.cc
/// @brief ArrayTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/Array.h"


BEGIN_NAMESPACE_YM

TEST(ArrayTest, int1)
{
  int a[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  Array<int> a1(a, 0, 10);

  int i = 0;
  for ( auto elem: a1 ) {
    EXPECT_EQ( a[i], elem );
    ++ i;
  }
}

TEST(ArrayTest, int2)
{
  int a[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  Array<int> a1(a, 3, 6);

  int i = 0;
  for ( auto elem: a1 ) {
    EXPECT_EQ( a[i + 3], elem );
    ++ i;
  }
}

END_NAMESPACE_YM
