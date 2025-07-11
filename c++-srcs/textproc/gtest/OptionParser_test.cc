
/// @file OptionParserTest.cc
/// @brief OptionParserTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/OptionParser.h"


BEGIN_NAMESPACE_YM

TEST(OptionParserTest, test1)
{
  std::string input("a: x, b :2, c");

  OptionParser optparse;

  auto ans = optparse.parse(input);

  EXPECT_EQ( 3, ans.size() );
  EXPECT_EQ( std::string("a"), ans[0].first );
  EXPECT_EQ( std::string("x"), ans[0].second );
  EXPECT_EQ( std::string("b"), ans[1].first );
  EXPECT_EQ( std::string("2"), ans[1].second );
  EXPECT_EQ( std::string("c"), ans[2].first );
  EXPECT_EQ( std::string(""), ans[2].second );
}

TEST(OptionParserTest, test2)
{
  std::string input("a@ x# b @2# c");

  OptionParser optparse('#', '@');

  auto ans = optparse.parse(input);

  EXPECT_EQ( 3, ans.size() );
  EXPECT_EQ( std::string("a"), ans[0].first );
  EXPECT_EQ( std::string("x"), ans[0].second );
  EXPECT_EQ( std::string("b"), ans[1].first );
  EXPECT_EQ( std::string("2"), ans[1].second );
  EXPECT_EQ( std::string("c"), ans[2].first );
  EXPECT_EQ( std::string(""), ans[2].second );
}

END_NAMESPACE_YM
