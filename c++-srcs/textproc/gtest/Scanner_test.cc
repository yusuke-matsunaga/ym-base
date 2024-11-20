
/// @file Scanner_test.cc
/// @brief Scanner_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include <sstream>
#include "ym/Scanner.h"


BEGIN_NAMESPACE_YM

class ScannerTest :
public ::testing::TestWithParam<string>
{
public:

  ScannerTest();


public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル名
  string mFileName;

};

ScannerTest::ScannerTest() :
  mFileName{GetParam()}
{
}

TEST_P(ScannerTest, get_test1)
{
  string path{DATAPATH + mFileName};
  ifstream is{path};
  ASSERT_TRUE( is.operator bool() );

  Scanner scan{is, FileInfo{path}};

  int c1 = scan.get();
  EXPECT_EQ( 'a', c1 );

  int c2 = scan.get();
  EXPECT_EQ( 'b', c2 );

  int c3 = scan.get();
  EXPECT_EQ( 'c', c3 );

  int r1 = scan.get();
  EXPECT_EQ( '\n', r1 );

  int c4 = scan.get();
  EXPECT_EQ( 'd', c4 );

  int c5 = scan.get();
  EXPECT_EQ( 'e', c5 );

  int c6 = scan.get();
  EXPECT_EQ( 'f', c6 );

  int r2 = scan.get();
  EXPECT_EQ( '\n', r2 );

  int c7 = scan.get();
  EXPECT_EQ( 'g', c7 );

  int c8 = scan.get();
  EXPECT_EQ( 'h', c8 );

  int c9 = scan.get();
  EXPECT_EQ( 'i', c9 );

  int r3 = scan.get();
  EXPECT_EQ( '\n', r3 );
  EXPECT_FALSE( scan.is_eof() );

  int eof = scan.get();
  EXPECT_EQ( EOF, eof );
  EXPECT_TRUE( scan.is_eof() );
}

TEST_P(ScannerTest, peek_test1)
{
  string path{DATAPATH + mFileName};
  ifstream is{path};
  ASSERT_TRUE( is.operator bool() );

  Scanner scan{is, FileInfo{path}};

  int c1 = scan.peek();
  EXPECT_EQ( 'a', c1 );

  int c1_2 = scan.peek();
  EXPECT_EQ( 'a', c1_2 );

  scan.accept();

  int c2 = scan.get();
  EXPECT_EQ( 'b', c2 );

  int c3 = scan.get();
  EXPECT_EQ( 'c', c3 );

  int r1 = scan.get();
  EXPECT_EQ( '\n', r1 );

  int c4 = scan.get();
  EXPECT_EQ( 'd', c4 );

  int c5 = scan.get();
  EXPECT_EQ( 'e', c5 );

  int c6 = scan.get();
  EXPECT_EQ( 'f', c6 );

  int r2 = scan.get();
  EXPECT_EQ( '\n', r2 );

  int c7 = scan.get();
  EXPECT_EQ( 'g', c7 );

  int c8 = scan.get();
  EXPECT_EQ( 'h', c8 );

  int c9 = scan.get();
  EXPECT_EQ( 'i', c9 );

  int r3 = scan.get();
  EXPECT_EQ( '\n', r3 );

  int eof = scan.get();
  EXPECT_EQ( EOF, eof );
}

TEST_P(ScannerTest, cur_loc_test1)
{
  string path{DATAPATH + mFileName};
  ifstream is{path};
  ASSERT_TRUE( is.operator bool() );

  FileInfo file_info{path};
  Scanner scan{is, file_info};

  int c1 = scan.get();
  EXPECT_EQ( 'a', c1 );
  auto loc1 = scan.cur_pos();
  EXPECT_EQ( FileLoc(file_info, 1, 1), loc1 );

  int c2 = scan.get();
  EXPECT_EQ( 'b', c2 );
  auto loc2 = scan.cur_pos();
  EXPECT_EQ( FileLoc(file_info, 1, 2), loc2 );

  int c3 = scan.get();
  EXPECT_EQ( 'c', c3 );
  auto loc3 = scan.cur_pos();
  EXPECT_EQ( FileLoc(file_info, 1, 3), loc3 );

  int r1 = scan.get();
  EXPECT_EQ( '\n', r1 );
  auto loc_r1 = scan.cur_pos();
  EXPECT_EQ( FileLoc(file_info, 1, 4), loc_r1 );

  int c4 = scan.get();
  EXPECT_EQ( 'd', c4 );
  auto loc4 = scan.cur_pos();
  EXPECT_EQ( FileLoc(file_info, 2, 1), loc4 );

  int c5 = scan.get();
  EXPECT_EQ( 'e', c5 );
  auto loc5 = scan.cur_pos();
  EXPECT_EQ( FileLoc(file_info, 2, 2), loc5 );

  int c6 = scan.get();
  EXPECT_EQ( 'f', c6 );
  auto loc6 = scan.cur_pos();
  EXPECT_EQ( FileLoc(file_info, 2, 3), loc6 );

  int r2 = scan.get();
  EXPECT_EQ( '\n', r2 );
  auto loc_r2 = scan.cur_pos();
  EXPECT_EQ( FileLoc(file_info, 2, 4), loc_r2 );

  int c7 = scan.get();
  EXPECT_EQ( 'g', c7 );
  auto loc7 = scan.cur_pos();
  EXPECT_EQ( FileLoc(file_info, 3, 1), loc7 );

  int c8 = scan.get();
  EXPECT_EQ( 'h', c8 );
  auto loc8 = scan.cur_pos();
  EXPECT_EQ( FileLoc(file_info, 3, 2), loc8 );

  int c9 = scan.get();
  EXPECT_EQ( 'i', c9 );
  auto loc9 = scan.cur_pos();
  EXPECT_EQ( FileLoc(file_info, 3, 3), loc9 );

  int r3 = scan.get();
  EXPECT_EQ( '\n', r3 );
  auto loc_r3 = scan.cur_pos();
  EXPECT_EQ( FileLoc(file_info, 3, 4), loc_r3 );

  int eof = scan.get();
  EXPECT_EQ( EOF, eof );
  auto loc10 = scan.cur_pos();
  EXPECT_EQ( FileLoc(file_info, 4, 1), loc10 );
}

INSTANTIATE_TEST_SUITE_P(Scanner_test,
			 ScannerTest,
			 ::testing::Values("text_unix.txt", "text_mac.txt", "text_win.txt"));

END_NAMESPACE_YM
