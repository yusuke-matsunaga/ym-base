
/// @file MultiSetCombiGenTest.cc
/// @brief MultiSetCombiGenTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/MultiSetCombiGen.h"


BEGIN_NAMESPACE_YM

// MultiSetCombiGen の出力結果が exp_list と等しいか検査する．
bool
check_gen(
  MultiSetCombiGen& gen,
  const int exp_list[]
)
{
  SizeType ng = gen.group_num();
  SizeType pos = 0;
  int val;
  int exp_val;
  for ( ; !gen.is_end(); ++ gen) {
    for ( SizeType i = 0; i < gen.k(); ++ i ) {
      val =  gen(i);
      exp_val = exp_list[pos];
      if ( exp_val != val ) {
	goto error;
      }
      ++ pos;
    }
    exp_val = exp_list[pos];
    if ( exp_val != -1 ) {
      goto error;
    }
    ++ pos;
  }
  exp_val = exp_list[pos];
  if ( exp_val != -2 ) {
    goto error;
  }

  return true;

 error:
  return false;
}

// GenBase のメンバ関数のチェック
TEST(MultiSetCombiGenTest, basic_test1)
{
  const SizeType n1 = 10;
  const SizeType n2 = 20;
  const SizeType n3 = 25;
  const SizeType k = 40;
  std::vector<SizeType> num_array{n1, n2, n3};
  MultiSetCombiGen gen(num_array, k);

  EXPECT_EQ( num_array.size(), gen.group_num() );
  EXPECT_EQ( n1, gen.n(0) );
  EXPECT_EQ( n2, gen.n(1) );
  EXPECT_EQ( n3, gen.n(2) );
  EXPECT_EQ( k, gen.k() );

  for ( int i = 0; i < k; ++ i ) {
    int val = gen(i);
    int exp_val;
    if ( i < 10 ) {
      exp_val = 0;
    }
    else if ( i < 30 ) {
      exp_val = 1;
    }
    else {
      exp_val = 2;
    }
    EXPECT_EQ( exp_val, val );
  }
}

// GenBase のメンバ関数のチェック
TEST(MultiSetCombiGenTest, basic_test2)
{
  const SizeType n1 = 10;
  const SizeType n2 = 20;
  const SizeType n3 = 25;
  const SizeType k = 40;
  MultiSetCombiGen gen({n1, n2, n3}, k);

  EXPECT_EQ( 3, gen.group_num() );
  EXPECT_EQ( n1, gen.n(0) );
  EXPECT_EQ( n2, gen.n(1) );
  EXPECT_EQ( n3, gen.n(2) );
  EXPECT_EQ( k, gen.k() );

  for ( int i = 0; i < k; ++ i ) {
    int val = gen(i);
    int exp_val;
    if ( i < 10 ) {
      exp_val = 0;
    }
    else if ( i < 30 ) {
      exp_val = 1;
    }
    else {
      exp_val = 2;
    }
    EXPECT_EQ( exp_val, val );
  }
}

// (3, 3), 3 の組み合わせの列挙．
TEST(MultiSetCombiGenTest, test1)
{
  int exp_list[] = {
    0, 0, 0, -1,
    0, 0, 1, -1,
    0, 1, 1, -1,
    1, 1, 1, -1,

    -2
  };

  MultiSetCombiGen gen({ 3, 3}, 3);
  EXPECT_TRUE( check_gen(gen, exp_list) );
}

// (1, 1, 1, 1), 2 の組み合わせの列挙．
TEST(MultiSetCombiGenTest, test2)
{
  int exp_list[] = {
    0, 1, -1,
    0, 2, -1,
    0, 3, -1,
    1, 2, -1,
    1, 3, -1,
    2, 3, -1,

    -2,
  };

  MultiSetCombiGen gen({1, 1, 1, 1}, 2);
  EXPECT_TRUE( check_gen(gen, exp_list) );
}

// コピーコンストラクタで途中の状態のコピーが出来るかのテスト
TEST(MultiSetCombiGenTest, copy1)
{
  MultiSetCombiGen gen1({3, 3}, 3);
  // 2つ進める．
  ++ gen1;
  ++ gen1;
  // コピーする．
  MultiSetCombiGen gen2(gen1);
  // gen2 から値を取り出す．
  // (0, 1, 1) になっているはず
  EXPECT_EQ( 0, gen2(0) );
  EXPECT_EQ( 1, gen2(1) );
  EXPECT_EQ( 1, gen2(2) );
}

// 代入演算子で途中の状態のコピーが出来るかのテスト
TEST(MultiSetCombiGenTest, copy2)
{
  MultiSetCombiGen gen1({3, 3}, 3);
  // 2つ進める．
  ++ gen1;
  ++ gen1;
  // コピーする．
  MultiSetCombiGen gen2 = gen1;
  // gen2 から値を取り出す．
  // (0, 1, 1) になっているはず
  EXPECT_EQ( 0, gen2(0) );
  EXPECT_EQ( 1, gen2(1) );
  EXPECT_EQ( 1, gen2(2) );
}

END_NAMESPACE_YM
