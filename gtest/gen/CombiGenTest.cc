
/// @file CombiGenTest.cc
/// @brief CombiGenTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/CombiGen.h"


BEGIN_NAMESPACE_YM

// CombiGen の出力結果が exp_list と等しいか検査する．
bool
check_cg(CombiGen& cg,
	 const int exp_list[])
{
  ymuint num = cg.num();
  ymuint combi_num = cg.combi_num();
  ymuint pos = 0;
  for ( ; !cg.is_end(); ++ cg) {
    for (ymuint i = 0; i < combi_num; ++ i) {
      ymuint val = cg(i);
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
  const ymuint n = 10;
  const ymuint k = 5;

  CombiGen cg(n, k);

  EXPECT_EQ( n, cg.num() );
  EXPECT_EQ( k, cg.combi_num() );

  for (ymuint i = 0; i < k; ++ i) {
    ymuint val = cg(i);
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

  CombiGen cg(4, 2);
  EXPECT_TRUE( check_cg(cg, exp_list) );
}

// 10個のなかから10個取り出す組み合わせの列挙．
TEST(CombiGenTest, test2)
{
  int exp_list[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1,
    -2,
  };

  CombiGen cg(10, 10);
  EXPECT_TRUE( check_cg(cg, exp_list) );
}

// コピーコンストラクタで途中の状態のコピーが出来るかのテスト
TEST(CombiGenTest, copy1)
{
  CombiGen cg1(4, 2);
  // 2つ進める．
  ++ cg1;
  ++ cg1;
  // コピーする．
  CombiGen cg2(cg1);
  // cg2 から値を取り出す．
  // (0, 3) になっているはず．
  EXPECT_EQ( 0, cg2(0) );
  EXPECT_EQ( 3, cg2(1) );
}

// 代入演算子で途中の状態のコピーが出来るかのテスト
TEST(CombiGenTest, copy2)
{
  CombiGen cg1(4, 2);
  // 2つ進める．
  ++ cg1;
  ++ cg1;
  // コピーする．
  CombiGen cg2 = cg1;
  // cg2 から値を取り出す．
  // (0, 3) になっているはず．
  EXPECT_EQ( 0, cg2(0) );
  EXPECT_EQ( 3, cg2(1) );
}

END_NAMESPACE_YM
