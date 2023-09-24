
/// @file JsonScannerTest.cc
/// @brief JsonScannerTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "JsonScanner.h"
#include "ym/MsgMgr.h"
#include "ym/StreamMsgHandler.h"


BEGIN_NAMESPACE_YM_JSON

TEST(JsonScannerTest, empty)
{
  istringstream s{""};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::End, tk );
}

TEST(JsonScannerTest, LCB)
{
  istringstream s{"{"};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::LCB, tk );
}

TEST(JsonScannerTest, RCB)
{
  istringstream s{"}"};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::RCB, tk );
}

TEST(JsonScannerTest, LBK)
{
  istringstream s{"["};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::LBK, tk );
}

TEST(JsonScannerTest, RBK)
{
  istringstream s{"]"};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::RBK, tk );
}

TEST(JsonScannerTest, Comma)
{
  istringstream s{","};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Comma, tk );
}

TEST(JsonScannerTest, Colon)
{
  istringstream s{":"};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Colon, tk );
}

TEST(JsonScannerTest, True)
{
  istringstream s{"true"};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::True, tk );
}

TEST(JsonScannerTest, False)
{
  istringstream s{"false"};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::False, tk );
}

TEST(JsonScannerTest, Null)
{
  istringstream s{"null"};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Null, tk );
}

TEST(JsonScannerTest, String)
{
  istringstream s{"\"abc\""};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::String, tk );
  EXPECT_EQ( "abc", scanner.cur_string() );
}

TEST(JsonScannerTest, Int1)
{
  istringstream s{"0"};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Int, tk );
  EXPECT_EQ( 0, scanner.cur_int() );
}

TEST(JsonScannerTest, Int2)
{
  istringstream s{"-0"};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Int, tk );
  EXPECT_EQ( 0, scanner.cur_int() );
}

TEST(JsonScannerTest, Int3)
{
  istringstream s{"123"};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Int, tk );
  EXPECT_EQ( 123, scanner.cur_int() );
}

TEST(JsonScannerTest, Int4)
{
  istringstream s{"-123"};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Int, tk );
  EXPECT_EQ( -123, scanner.cur_int() );
}

TEST(JsonScannerTest, Float1)
{
  istringstream s{"0."};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Float, tk );
  EXPECT_EQ( 0.0, scanner.cur_float() );
}

TEST(JsonScannerTest, Float2)
{
  istringstream s{"0.1"};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Float, tk );
  EXPECT_EQ( 0.1, scanner.cur_float() );
}

TEST(JsonScannerTest, Float3)
{
  istringstream s{"0.1e10"};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Float, tk );
  EXPECT_EQ( 1.0e9, scanner.cur_float() );
}

TEST(JsonScannerTest, Float4)
{
  istringstream s{"-0.1E10"};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Float, tk );
  EXPECT_EQ( -1.0e9, scanner.cur_float() );
}

TEST(JsonScannerTest, Float5)
{
  istringstream s{".1e10"};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Float, tk );
  EXPECT_EQ( 1.0e9, scanner.cur_float() );
}

TEST(JsonScannerTest, True_bad1)
{
  istringstream s{"tue"};

  JsonScanner scanner{s, FileInfo{}};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, True_bad2)
{
  istringstream s{"trrue"};

  JsonScanner scanner{s, FileInfo{}};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, True_bad3)
{
  istringstream s{"tru"};

  JsonScanner scanner{s, FileInfo{}};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, False_bad1)
{
  istringstream s{"ffalse"};

  JsonScanner scanner{s, FileInfo{}};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, False_bad2)
{
  istringstream s{"fase"};

  JsonScanner scanner{s, FileInfo{}};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, False_bad3)
{
  istringstream s{"fale"};

  JsonScanner scanner{s, FileInfo{}};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, False_bad4)
{
  istringstream s{"fals"};

  JsonScanner scanner{s, FileInfo{}};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, Null_bad1)
{
  istringstream s{"nl"};

  JsonScanner scanner{s, FileInfo{}};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, Null_bad2)
{
  istringstream s{"nual"};

  JsonScanner scanner{s, FileInfo{}};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, Null_bad3)
{
  istringstream s{"nul"};

  JsonScanner scanner{s, FileInfo{}};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, comment1)
{
  istringstream s{"/* comment */:"};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Colon, tk );
}

TEST(JsonScannerTest, comment1_2)
{
  istringstream s{"/* **** comment */:"};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Colon, tk );
}

TEST(JsonScannerTest, comment2)
{
  istringstream s{"//\n:"};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Colon, tk );
}

TEST(JsonScannerTest, comment2_2)
{
  istringstream s{"//"};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::End, tk );
}

TEST(JsonScannerTest, comment3)
{
  istringstream s{"#\n:"};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();

  EXPECT_EQ( JsonToken::Colon, tk );
}

TEST(JsonScannerTest, comment_bad1)
{
  istringstream s{"/ * comment */:"};

  JsonScanner scanner{s, FileInfo{}};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, comment_bad2)
{
  istringstream s{"/ * comment "};

  JsonScanner scanner{s, FileInfo{}};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, comment_bad3)
{
  istringstream s{"/ * comment *"};

  JsonScanner scanner{s, FileInfo{}};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, bad1)
{
  istringstream s{"abccomment "};

  JsonScanner scanner{s, FileInfo{}};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, num_bad1)
{
  istringstream s{"-abccomment "};

  JsonScanner scanner{s, FileInfo{}};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, string_bad1)
{
  istringstream s{"\"abccomment "};

  JsonScanner scanner{s, FileInfo{}};

  EXPECT_THROW(
    auto tk = scanner.read_token(),
    std::invalid_argument
  );
}

TEST(JsonScannerTest, ustring)
{
  istringstream s{"\"\\u3042\""};

  JsonScanner scanner{s, FileInfo{}};

  auto tk = scanner.read_token();
  EXPECT_EQ( JsonToken::String, tk );
  EXPECT_EQ( "あ", scanner.cur_string() );
}

END_NAMESPACE_YM_JSON
