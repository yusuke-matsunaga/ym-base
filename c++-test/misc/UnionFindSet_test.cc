
/// @file UnionFindSet_test.cc
/// @brief UnionFindSet_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2021 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/UnionFindSet.h"


BEGIN_NAMESPACE_YM

TEST(UnionFindSet, simple_1)
{
  UnionFindSet ufset(11);

  ufset.merge(1, 2);
  ufset.merge(3, 4);
  ufset.merge(5, 6);
  ufset.merge(7, 8);
  ufset.merge(9, 10);

  int x, y;

  auto x1 = ufset.find(1);
  EXPECT_EQ( 1, x1 );

  auto x2 = ufset.find(2);
  EXPECT_EQ( x1, x2 );

  auto x3 = ufset.find(3);
  EXPECT_EQ( 3, x3 );

  auto x4 = ufset.find(4);
  EXPECT_EQ( x3, x4 );

  auto x5 = ufset.find(5);
  EXPECT_EQ( 5, x5 );

  auto x6 = ufset.find(6);
  EXPECT_EQ( x5, x6 );

  auto x7 = ufset.find(7);
  EXPECT_EQ( 7, x7 );

  auto x8 = ufset.find(8);
  EXPECT_EQ( x7, x8 );

  auto x9 = ufset.find(9);
  EXPECT_EQ( 9, x9 );

  auto x10 = ufset.find(10);
  EXPECT_EQ( x9, x10 );

  ufset.merge(x3, x8);

  x = ufset.find(x7);
  y = ufset.find(x3);
  EXPECT_EQ( y, x );

  ufset.merge(x4, x1);
  x = ufset.find(x2);
  y = ufset.find(x3);
  EXPECT_EQ( y, x );

}

TEST(UnionFindSet, large_1)
{
  int N = 100000;
  UnionFindSet ufset(N);

  for ( int i = N - 1; i > 0; -- i ) {
    auto x = ufset.find(i);
    auto y = ufset.find(i - 1);
    ufset.merge(i, i - 1);
  }

  auto x0 = ufset.find(0);
  for ( int i = 1; i < N; ++ i ) {
    auto x = ufset.find(i);
    EXPECT_EQ( x0, x );
  }
}

END_NAMESPACE_YM
