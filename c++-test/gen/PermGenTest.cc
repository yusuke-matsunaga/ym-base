
/// @file PermGenTest.cc
/// @brief PermGenTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/PermGen.h"


BEGIN_NAMESPACE_YM

// PermGen の出力結果が exp_list と等しいか検査する．
bool
check_gen(PermGen& gen,
	 const int exp_list[])
{
  int num = gen.n();
  int combi_num = gen.k();
  int pos = 0;
  for ( ; !gen.is_end(); ++ gen ) {
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
TEST(PermGenTest, basic_test)
{
  const int n = 10;
  const int k = 5;

  PermGen gen(n, k);

  EXPECT_EQ( n, gen.n() );
  EXPECT_EQ( k, gen.k() );

  for ( int i = 0; i < k; ++ i ) {
    int val = gen(i);
    EXPECT_EQ( i, val );
  }
}

// 4個のなかから２つ取り出す順列の列挙．
TEST(PermGenTest, test1)
{
  int exp_list[] {
    0, 1, -1,
    0, 2, -1,
    0, 3, -1,
    1, 0, -1,
    1, 2, -1,
    1, 3, -1,
    2, 0, -1,
    2, 1, -1,
    2, 3, -1,
    3, 0, -1,
    3, 1, -1,
    3, 2, -1,
    -2,
  };

  PermGen gen(4, 2);
  EXPECT_TRUE( check_gen(gen, exp_list) );
}

// 3個のなかから3個取り出す順列の列挙．
TEST(PermGenTest, test2)
{
  int exp_list[] {
    0, 1, 2, -1,
    0, 2, 1, -1,
    1, 0, 2, -1,
    1, 2, 0, -1,
    2, 0, 1, -1,
    2, 1, 0, -1,
    -2,
  };

  PermGen gen(3, 3);
  EXPECT_TRUE( check_gen(gen, exp_list) );
}

// コピーコンストラクタで途中の状態のコピーが出来るかのテスト
TEST(PermGenTest, copy1)
{
  PermGen gen1(4, 2);
  // 2つ進める．
  ++ gen1;
  ++ gen1;
  // コピーする．
  PermGen gen2(gen1);
  // gen2 から値を取り出す．
  // (0, 3) になっているはず．
  EXPECT_EQ( 0, gen2(0) );
  EXPECT_EQ( 3, gen2(1) );
}

// 代入演算子で途中の状態のコピーが出来るかのテスト
TEST(PermGenTest, copy2)
{
  PermGen gen1(4, 2);
  // 2つ進める．
  ++ gen1;
  ++ gen1;
  // コピーする．
  PermGen gen2 = gen1;
  // gen2 から値を取り出す．
  // (0, 3) になっているはず．
  EXPECT_EQ( 0, gen2(0) );
  EXPECT_EQ( 3, gen2(1) );
}

END_NAMESPACE_YM
