
/// @file JsonScannerTest.cc
/// @brief JsonScannerTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "JsonScanner.h"
#include <sstream>


BEGIN_NAMESPACE_YM_JSON

TEST(JsonScannerTest, empty)
{
  std::istringstream s{""};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::End, tk );
}

TEST(JsonScannerTest, LCB)
{
  std::istringstream s{"{"};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::LCB, tk );
}

TEST(JsonScannerTest, RCB)
{
  std::istringstream s{"}"};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::RCB, tk );
}

TEST(JsonScannerTest, LBK)
{
  std::istringstream s{"["};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::LBK, tk );
}

TEST(JsonScannerTest, RBK)
{
  std::istringstream s{"]"};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::RBK, tk );
}

TEST(JsonScannerTest, Comma)
{
  std::istringstream s{","};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Comma, tk );
}

TEST(JsonScannerTest, Colon)
{
  std::istringstream s{":"};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Colon, tk );
}

TEST(JsonScannerTest, True)
{
  std::istringstream s{"true"};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::True, tk );
}

TEST(JsonScannerTest, False)
{
  std::istringstream s{"false"};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::False, tk );
}

TEST(JsonScannerTest, Null)
{
  std::istringstream s{"null"};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Null, tk );
}

TEST(JsonScannerTest, String)
{
  std::istringstream s{"\"abc\""};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::String, tk );
  EXPECT_EQ( "abc", scanner.cur_string() );
}

TEST(JsonScannerTest, Int1)
{
  std::istringstream s{"0"};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Int, tk );
  EXPECT_EQ( 0, scanner.cur_int() );
}

TEST(JsonScannerTest, Int2)
{
  std::istringstream s{"-0"};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Int, tk );
  EXPECT_EQ( 0, scanner.cur_int() );
}

TEST(JsonScannerTest, Int3)
{
  std::istringstream s{"123"};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Int, tk );
  EXPECT_EQ( 123, scanner.cur_int() );
}

TEST(JsonScannerTest, Int4)
{
  std::istringstream s{"-123"};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Int, tk );
  EXPECT_EQ( -123, scanner.cur_int() );
}

TEST(JsonScannerTest, Float1)
{
  std::istringstream s{"0."};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Float, tk );
  EXPECT_EQ( 0.0, scanner.cur_float() );
}

TEST(JsonScannerTest, Float2)
{
  std::istringstream s{"0.1"};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Float, tk );
  EXPECT_EQ( 0.1, scanner.cur_float() );
}

TEST(JsonScannerTest, Float3)
{
  std::istringstream s{"0.1e10"};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Float, tk );
  EXPECT_EQ( 1.0e9, scanner.cur_float() );
}

TEST(JsonScannerTest, Float4)
{
  std::istringstream s{"-0.1E10"};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Float, tk );
  EXPECT_EQ( -1.0e9, scanner.cur_float() );
}

TEST(JsonScannerTest, Float5)
{
  std::istringstream s{".1e10"};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Float, tk );
  EXPECT_EQ( 1.0e9, scanner.cur_float() );
}

TEST(JsonScannerTest, True_bad1)
{
  std::istringstream s{"tue"};

  JsonScanner scanner{s};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, True_bad2)
{
  std::istringstream s{"trrue"};

  JsonScanner scanner{s};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, True_bad3)
{
  std::istringstream s{"tru"};

  JsonScanner scanner{s};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, False_bad1)
{
  std::istringstream s{"ffalse"};

  JsonScanner scanner{s};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, False_bad2)
{
  std::istringstream s{"fase"};

  JsonScanner scanner{s};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, False_bad3)
{
  std::istringstream s{"fale"};

  JsonScanner scanner{s};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, False_bad4)
{
  std::istringstream s{"fals"};

  JsonScanner scanner{s};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, Null_bad1)
{
  std::istringstream s{"nl"};

  JsonScanner scanner{s};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, Null_bad2)
{
  std::istringstream s{"nual"};

  JsonScanner scanner{s};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, Null_bad3)
{
  std::istringstream s{"nul"};

  JsonScanner scanner{s};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, comment1)
{
  std::istringstream s{"/* comment */:"};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Colon, tk );
}

TEST(JsonScannerTest, comment1_2)
{
  std::istringstream s{"/* **** comment */:"};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Colon, tk );
}

TEST(JsonScannerTest, comment2)
{
  std::istringstream s{"//\n:"};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Colon, tk );
}

TEST(JsonScannerTest, comment2_2)
{
  std::istringstream s{"//"};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::End, tk );
}

TEST(JsonScannerTest, comment3)
{
  std::istringstream s{"#\n:"};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Colon, tk );
}

TEST(JsonScannerTest, comment_bad1)
{
  std::istringstream s{"/ * comment */:"};

  JsonScanner scanner{s};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, comment_bad2)
{
  std::istringstream s{"/ * comment "};

  JsonScanner scanner{s};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, comment_bad3)
{
  std::istringstream s{"/ * comment *"};

  JsonScanner scanner{s};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, bad1)
{
  std::istringstream s{"abccomment "};

  JsonScanner scanner{s};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, num_bad1)
{
  std::istringstream s{"-abccomment "};

  JsonScanner scanner{s};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, string_bad1)
{
  std::istringstream s{"\"abccomment "};

  JsonScanner scanner{s};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, ustring)
{
  std::istringstream s{"\"\\u3042\""};

  JsonScanner scanner{s};

  auto tk = scanner.read_token();
  EXPECT_EQ( JsonToken::String, tk );
  EXPECT_EQ( "あ", scanner.cur_string() );
}

END_NAMESPACE_YM_JSON
