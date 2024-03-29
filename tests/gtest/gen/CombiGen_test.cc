
/// @file CombiGenTest.cc
/// @brief CombiGenTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/CombiGen.h"


BEGIN_NAMESPACE_YM

// CombiGen の出力結果が exp_list と等しいか検査する．
bool
check_gen(
  CombiGen& gen,
  const int exp_list[]
)
{
  SizeType num = gen.n();
  SizeType combi_num = gen.k();
  int pos = 0;
  for ( ; !gen.is_end(); ++ gen) {
    for ( auto val: gen.elem_list() ) {
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

  return true;
}

// GenBase のメンバ関数のチェック
TEST(CombiGenTest, basic_test)
{
  const SizeType n = 10;
  const SizeType k = 5;

  CombiGen gen(n, k);

  EXPECT_EQ( n, gen.n() );
  EXPECT_EQ( k, gen.k() );

  for ( int i = 0; i < k; ++ i ) {
    int val = gen(i);
    EXPECT_EQ( i, val );
  }
}

// 4個のなかから２つ取り出す組み合わせの列挙．
TEST(CombiGenTest, test1)
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

  CombiGen gen(4, 2);
  EXPECT_TRUE( check_gen(gen, exp_list) );
}

// 10個のなかから10個取り出す組み合わせの列挙．
TEST(CombiGenTest, test2)
{
  int exp_list[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1,
    -2,
  };

  CombiGen gen(10, 10);
  EXPECT_TRUE( check_gen(gen, exp_list) );
}

// コピーコンストラクタで途中の状態のコピーが出来るかのテスト
TEST(CombiGenTest, copy1)
{
  CombiGen gen1(4, 2);
  // 2つ進める．
  ++ gen1;
  ++ gen1;
  // コピーする．
  CombiGen gen2(gen1);
  // gen2 から値を取り出す．
  // (0, 3) になっているはず．
  EXPECT_EQ( 0, gen2(0) );
  EXPECT_EQ( 3, gen2(1) );
}

// 代入演算子で途中の状態のコピーが出来るかのテスト
TEST(CombiGenTest, copy2)
{
  CombiGen gen1(4, 2);
  // 2つ進める．
  ++ gen1;
  ++ gen1;
  // コピーする．
  CombiGen gen2 = gen1;
  // gen2 から値を取り出す．
  // (0, 3) になっているはず．
  EXPECT_EQ( 0, gen2(0) );
  EXPECT_EQ( 3, gen2(1) );
}

END_NAMESPACE_YM
