
/// @file ShStringTest.cc
/// @brief ShStringTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2019 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/ShString.h"


BEGIN_NAMESPACE_YM

TEST(ShStringTest, test1)
{
  ShString a{"abc"};
  ShString b{"abc"};

  EXPECT_TRUE( a == b );
}

END_NAMESPACE_YM
