
/// @file RangeTest.cc
/// @brief RangeTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM

TEST(RangeTest, simple_1)
{
  std::vector<int> val_list;

  const int n = 10;
  for ( auto i: Range_<int, 1>(0, n) ) {
    val_list.push_back(i);
  }

  EXPECT_EQ( n, val_list.size() );
  for ( int i = 0; i < n; ++ i ) {
    EXPECT_EQ( i, val_list[i] );
  }
}

TEST(RangeTest, simple_2)
{
  std::vector<int> val_list;

  const int n = 10;
  for ( auto i: Range(n) ) {
    val_list.push_back(i);
  }

  EXPECT_EQ( n, val_list.size() );
  for ( int i = 0; i < n; ++ i ) {
    EXPECT_EQ( i, val_list[i] );
  }
}

TEST(RangeTest, step3_1)
{
  std::vector<int> val_list;
  for ( auto i: Range_<unsigned int, 3>(0, 10) ) {
    val_list.push_back(i);
  }

  // 答は 0, 3, 6, 9
  EXPECT_EQ(  4, val_list.size() );
  EXPECT_EQ(  0, val_list[0] );
  EXPECT_EQ(  3, val_list[1] );
  EXPECT_EQ(  6, val_list[2] );
  EXPECT_EQ(  9, val_list[3] );
}

TEST(RangeTest, step3_2)
{
  std::vector<int> val_list;
  for ( auto i: Range_<long, 3>(1, 11) ) {
    val_list.push_back(i);
  }

  // 答は 1, 4, 7, 10
  EXPECT_EQ(  4, val_list.size() );
  EXPECT_EQ(  1, val_list[0] );
  EXPECT_EQ(  4, val_list[1] );
  EXPECT_EQ(  7, val_list[2] );
  EXPECT_EQ( 10, val_list[3] );
}

TEST(RangeTest, negative_1)
{
  std::vector<int> val_list;
  for ( auto i: Range_<short, -1>(5, 0) ) {
    val_list.push_back(i);
  }

  // 答えは 5, 4, 3, 2, 1
  EXPECT_EQ( 5, val_list.size() );
  EXPECT_EQ( 5, val_list[0] );
  EXPECT_EQ( 4, val_list[1] );
  EXPECT_EQ( 3, val_list[2] );
  EXPECT_EQ( 2, val_list[3] );
  EXPECT_EQ( 1, val_list[4] );
}

TEST(RangeTest, negative_2)
{
  std::vector<int> val_list;
  for ( auto i: Range_<int, -2>(5, 0) ) {
    val_list.push_back(i);
  }

  // 答えは 5, 3, 1
  EXPECT_EQ( 3, val_list.size() );
  EXPECT_EQ( 5, val_list[0] );
  EXPECT_EQ( 3, val_list[1] );
  EXPECT_EQ( 1, val_list[2] );
}

TEST(RangeTest, negative_3)
{
  std::vector<int> val_list;
  for ( auto i: Range_<int, -2>(5, -1) ) {
    val_list.push_back(i);
  }

  // 答えは 5, 3, 1
  EXPECT_EQ( 3, val_list.size() );
  EXPECT_EQ( 5, val_list[0] );
  EXPECT_EQ( 3, val_list[1] );
  EXPECT_EQ( 1, val_list[2] );
}

TEST(RangeTest, negative_4)
{
  std::vector<int> val_list;
  for ( auto i: Range_<int, -2>(5, -2) ) {
    val_list.push_back(i);
  }

  // 答えは 5, 3, 1, -1
  EXPECT_EQ( 4, val_list.size() );
  EXPECT_EQ( 5, val_list[0] );
  EXPECT_EQ( 3, val_list[1] );
  EXPECT_EQ( 1, val_list[2] );
  EXPECT_EQ(-1, val_list[3] );
}

END_NAMESPACE_YM
