
/// @file MultiPermGenTest.cc
/// @brief MultiPermGenTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "ym/MultiPermGen.h"


BEGIN_NAMESPACE_YM

// MultiPermGen の出力結果が exp_list と等しいか検査する．
bool
check_gen(
  MultiPermGen& gen,
  const int exp_list[]
)
{
  int ng = gen.group_num();
  int pos = 0;
  for ( ; !gen.is_end(); ++ gen) {
    for ( int g = 0; g < ng; ++ g ) {
      for ( int i = 0; i < gen.k(g); ++ i ) {
	int val =  gen(g, i);
	int exp_val = exp_list[pos];
	if ( exp_val != val ) {
	  return false;
	}
	++ pos;
      }
      int exp_val = exp_list[pos];
      if ( exp_val != -1 ) {
	return false;
      }
      ++ pos;
    }
    int exp_val = exp_list[pos];
    if ( exp_val != -2 ) {
      return false;
    }
    ++ pos;
  }
  int exp_val = exp_list[pos];
  if ( exp_val != -3 ) {
    return false;
  }

  return true;
}

// 空の組み合わせのテスト
TEST(MultiPermGenTest, empty_test)
{
  vector<pair<int, int>> nk_array{};
  MultiPermGen gen{nk_array};

  EXPECT_EQ( nk_array.size(), gen.group_num() );

  for ( ; !gen.is_end(); ++ gen ) {
    ;
  }
}

// GenBase のメンバ関数のチェック
TEST(MultiPermGenTest, basic_test1)
{
  const int n1 = 10;
  const int k1 = 5;
  const int n2 = 20;
  const int k2 = 3;
  vector<pair<int, int>> nk_array{ {n1, k1}, {n2, k2} };
  MultiPermGen gen(nk_array);

  EXPECT_EQ( nk_array.size(), gen.group_num() );
  EXPECT_EQ( n1, gen.n(0) );
  EXPECT_EQ( k1, gen.k(0) );
  EXPECT_EQ( n2, gen.n(1) );
  EXPECT_EQ( k2, gen.k(1) );

  for ( int g: { 0, 1 } ) {
    for ( int i = 0; i < gen.k(g); ++ i ) {
      int val = gen(g, i);
      EXPECT_EQ( i, val );
    }
  }
}

// GenBase のメンバ関数のチェック
TEST(MultiPermGenTest, basic_test2)
{
  const int n1 = 10;
  const int k1 = 5;
  const int n2 = 20;
  const int k2 = 3;
  MultiPermGen gen({ {n1, k1}, {n2, k2} });

  EXPECT_EQ( 2, gen.group_num() );
  EXPECT_EQ( n1, gen.n(0) );
  EXPECT_EQ( k1, gen.k(0) );
  EXPECT_EQ( n2, gen.n(1) );
  EXPECT_EQ( k2, gen.k(1) );

  for ( int g: { 0, 1 } ) {
    for ( int i = 0; i < gen.k(g); ++ i ) {
      int val = gen(g, i);
      EXPECT_EQ( i, val );
    }
  }
}

// (4, 2) (3, 2) の組み合わせの列挙．
TEST(MultiPermGenTest, test1)
{
  int exp_list[] = {
    0, 1, -1, 0, 1, -1, -2,
    0, 1, -1, 0, 2, -1, -2,
    0, 1, -1, 1, 0, -1, -2,
    0, 1, -1, 1, 2, -1, -2,
    0, 1, -1, 2, 0, -1, -2,
    0, 1, -1, 2, 1, -1, -2,

    0, 2, -1, 0, 1, -1, -2,
    0, 2, -1, 0, 2, -1, -2,
    0, 2, -1, 1, 0, -1, -2,
    0, 2, -1, 1, 2, -1, -2,
    0, 2, -1, 2, 0, -1, -2,
    0, 2, -1, 2, 1, -1, -2,

    0, 3, -1, 0, 1, -1, -2,
    0, 3, -1, 0, 2, -1, -2,
    0, 3, -1, 1, 0, -1, -2,
    0, 3, -1, 1, 2, -1, -2,
    0, 3, -1, 2, 0, -1, -2,
    0, 3, -1, 2, 1, -1, -2,

    1, 0, -1, 0, 1, -1, -2,
    1, 0, -1, 0, 2, -1, -2,
    1, 0, -1, 1, 0, -1, -2,
    1, 0, -1, 1, 2, -1, -2,
    1, 0, -1, 2, 0, -1, -2,
    1, 0, -1, 2, 1, -1, -2,

    1, 2, -1, 0, 1, -1, -2,
    1, 2, -1, 0, 2, -1, -2,
    1, 2, -1, 1, 0, -1, -2,
    1, 2, -1, 1, 2, -1, -2,
    1, 2, -1, 2, 0, -1, -2,
    1, 2, -1, 2, 1, -1, -2,

    1, 3, -1, 0, 1, -1, -2,
    1, 3, -1, 0, 2, -1, -2,
    1, 3, -1, 1, 0, -1, -2,
    1, 3, -1, 1, 2, -1, -2,
    1, 3, -1, 2, 0, -1, -2,
    1, 3, -1, 2, 1, -1, -2,

    2, 0, -1, 0, 1, -1, -2,
    2, 0, -1, 0, 2, -1, -2,
    2, 0, -1, 1, 0, -1, -2,
    2, 0, -1, 1, 2, -1, -2,
    2, 0, -1, 2, 0, -1, -2,
    2, 0, -1, 2, 1, -1, -2,

    2, 1, -1, 0, 1, -1, -2,
    2, 1, -1, 0, 2, -1, -2,
    2, 1, -1, 1, 0, -1, -2,
    2, 1, -1, 1, 2, -1, -2,
    2, 1, -1, 2, 0, -1, -2,
    2, 1, -1, 2, 1, -1, -2,

    2, 3, -1, 0, 1, -1, -2,
    2, 3, -1, 0, 2, -1, -2,
    2, 3, -1, 1, 0, -1, -2,
    2, 3, -1, 1, 2, -1, -2,
    2, 3, -1, 2, 0, -1, -2,
    2, 3, -1, 2, 1, -1, -2,

    3, 0, -1, 0, 1, -1, -2,
    3, 0, -1, 0, 2, -1, -2,
    3, 0, -1, 1, 0, -1, -2,
    3, 0, -1, 1, 2, -1, -2,
    3, 0, -1, 2, 0, -1, -2,
    3, 0, -1, 2, 1, -1, -2,

    3, 1, -1, 0, 1, -1, -2,
    3, 1, -1, 0, 2, -1, -2,
    3, 1, -1, 1, 0, -1, -2,
    3, 1, -1, 1, 2, -1, -2,
    3, 1, -1, 2, 0, -1, -2,
    3, 1, -1, 2, 1, -1, -2,

    3, 2, -1, 0, 1, -1, -2,
    3, 2, -1, 0, 2, -1, -2,
    3, 2, -1, 1, 0, -1, -2,
    3, 2, -1, 1, 2, -1, -2,
    3, 2, -1, 2, 0, -1, -2,
    3, 2, -1, 2, 1, -1, -2,

    -3,
  };


  MultiPermGen gen( { {4, 2}, {3, 2} } );
  EXPECT_TRUE( check_gen(gen, exp_list) );
}

// (10, 10) (5, 5)の組み合わせの列挙．
TEST(MultiPermGenTest, test2)
{
  int exp_list[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1,
    0, 1, 2, 3, 4, -1,
    -2,
    -3,
  };

  MultiPermGen gen( { {10, 10}, {5, 5} } );

  EXPECT_EQ( 0, gen(0, 0) );
  EXPECT_EQ( 1, gen(0, 1) );
  EXPECT_EQ( 2, gen(0, 2) );
  EXPECT_EQ( 3, gen(0, 3) );
  EXPECT_EQ( 4, gen(0, 4) );
  EXPECT_EQ( 5, gen(0, 5) );
  EXPECT_EQ( 6, gen(0, 6) );
  EXPECT_EQ( 7, gen(0, 7) );
  EXPECT_EQ( 8, gen(0, 8) );
  EXPECT_EQ( 9, gen(0, 9) );
  EXPECT_EQ( 0, gen(1, 0) );
  EXPECT_EQ( 1, gen(1, 1) );
  EXPECT_EQ( 2, gen(1, 2) );
  EXPECT_EQ( 3, gen(1, 3) );
  EXPECT_EQ( 4, gen(1, 4) );
}

// コピーコンストラクタで途中の状態のコピーが出来るかのテスト
TEST(MultiPermGenTest, copy1)
{
  MultiPermGen gen1( { {4, 2}, {3, 2} } );
  // 2つ進める．
  ++ gen1;
  ++ gen1;
  // コピーする．
  MultiPermGen gen2(gen1);
  // gen2 から値を取り出す．
  // (0, 1)(1, 2) になっているはず．
  EXPECT_EQ( 0, gen2(0, 0) );
  EXPECT_EQ( 1, gen2(0, 1) );
  EXPECT_EQ( 1, gen2(1, 0) );
  EXPECT_EQ( 0, gen2(1, 1) );
}

// 代入演算子で途中の状態のコピーが出来るかのテスト
TEST(MultiPermGenTest, copy2)
{
  MultiPermGen gen1( { {4, 2}, {3, 2} } );
  // 2つ進める．
  ++ gen1;
  ++ gen1;
  // コピーする．
  MultiPermGen gen2 = gen1;
  // gen2 から値を取り出す．
  // (0, 1)(1, 2) になっているはず．
  EXPECT_EQ( 0, gen2(0, 0) );
  EXPECT_EQ( 1, gen2(0, 1) );
  EXPECT_EQ( 1, gen2(1, 0) );
  EXPECT_EQ( 0, gen2(1, 1) );
}

END_NAMESPACE_YM
