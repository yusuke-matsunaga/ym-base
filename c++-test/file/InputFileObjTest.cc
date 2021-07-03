
/// @file InputFileObjTest.cc
/// @brief InputFileObjTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2019, 2021 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include <sstream>
#include "ym/InputFileObj.h"


BEGIN_NAMESPACE_YM

class InputFileObjTest :
public ::testing::TestWithParam<string>
{
public:

  InputFileObjTest();


public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル名
  string mFileName;

};

InputFileObjTest::InputFileObjTest() :
  mFileName{GetParam()}
{
}

TEST_P(InputFileObjTest, get_test1)
{
  string path{DATAPATH + mFileName};
  ifstream is(path);
  ASSERT_TRUE( is.operator bool() );

  InputFileObj ifo{is, FileInfo{path}};

  int c1 = ifo.get();
  EXPECT_EQ( 'a', c1 );

  int c2 = ifo.get();
  EXPECT_EQ( 'b', c2 );

  int c3 = ifo.get();
  EXPECT_EQ( 'c', c3 );

  int r1 = ifo.get();
  EXPECT_EQ( '\n', r1 );

  int c4 = ifo.get();
  EXPECT_EQ( 'd', c4 );

  int c5 = ifo.get();
  EXPECT_EQ( 'e', c5 );

  int c6 = ifo.get();
  EXPECT_EQ( 'f', c6 );

  int r2 = ifo.get();
  EXPECT_EQ( '\n', r2 );

  int c7 = ifo.get();
  EXPECT_EQ( 'g', c7 );

  int c8 = ifo.get();
  EXPECT_EQ( 'h', c8 );

  int c9 = ifo.get();
  EXPECT_EQ( 'i', c9 );

  int r3 = ifo.get();
  EXPECT_EQ( '\n', r3 );
  EXPECT_FALSE( ifo.is_eof() );

  int eof = ifo.get();
  EXPECT_EQ( EOF, eof );
  EXPECT_TRUE( ifo.is_eof() );
}

TEST_P(InputFileObjTest, peek_test1)
{
  string path{DATAPATH + mFileName};
  ifstream is(path);
  ASSERT_TRUE( is.operator bool() );

  InputFileObj ifo{is, FileInfo{path}};

  int c1 = ifo.peek();
  EXPECT_EQ( 'a', c1 );

  int c1_2 = ifo.peek();
  EXPECT_EQ( 'a', c1_2 );

  ifo.accept();

  int c2 = ifo.get();
  EXPECT_EQ( 'b', c2 );

  int c3 = ifo.get();
  EXPECT_EQ( 'c', c3 );

  int r1 = ifo.get();
  EXPECT_EQ( '\n', r1 );

  int c4 = ifo.get();
  EXPECT_EQ( 'd', c4 );

  int c5 = ifo.get();
  EXPECT_EQ( 'e', c5 );

  int c6 = ifo.get();
  EXPECT_EQ( 'f', c6 );

  int r2 = ifo.get();
  EXPECT_EQ( '\n', r2 );

  int c7 = ifo.get();
  EXPECT_EQ( 'g', c7 );

  int c8 = ifo.get();
  EXPECT_EQ( 'h', c8 );

  int c9 = ifo.get();
  EXPECT_EQ( 'i', c9 );

  int r3 = ifo.get();
  EXPECT_EQ( '\n', r3 );

  int eof = ifo.get();
  EXPECT_EQ( EOF, eof );
}

TEST_P(InputFileObjTest, cur_loc_test1)
{
  string path{DATAPATH + mFileName};
  ifstream is(path);
  ASSERT_TRUE( is.operator bool() );

  FileInfo file_info{path};
  InputFileObj ifo{is, file_info};

  int c1 = ifo.get();
  EXPECT_EQ( 'a', c1 );
  auto loc1 = ifo.cur_loc();
  EXPECT_EQ( FileLoc(file_info, 1, 1), loc1 );

  int c2 = ifo.get();
  EXPECT_EQ( 'b', c2 );
  auto loc2 = ifo.cur_loc();
  EXPECT_EQ( FileLoc(file_info, 1, 2), loc2 );

  int c3 = ifo.get();
  EXPECT_EQ( 'c', c3 );
  auto loc3 = ifo.cur_loc();
  EXPECT_EQ( FileLoc(file_info, 1, 3), loc3 );

  int r1 = ifo.get();
  EXPECT_EQ( '\n', r1 );
  auto loc_r1 = ifo.cur_loc();
  EXPECT_EQ( FileLoc(file_info, 1, 4), loc_r1 );

  int c4 = ifo.get();
  EXPECT_EQ( 'd', c4 );
  auto loc4 = ifo.cur_loc();
  EXPECT_EQ( FileLoc(file_info, 2, 1), loc4 );

  int c5 = ifo.get();
  EXPECT_EQ( 'e', c5 );
  auto loc5 = ifo.cur_loc();
  EXPECT_EQ( FileLoc(file_info, 2, 2), loc5 );

  int c6 = ifo.get();
  EXPECT_EQ( 'f', c6 );
  auto loc6 = ifo.cur_loc();
  EXPECT_EQ( FileLoc(file_info, 2, 3), loc6 );

  int r2 = ifo.get();
  EXPECT_EQ( '\n', r2 );
  auto loc_r2 = ifo.cur_loc();
  EXPECT_EQ( FileLoc(file_info, 2, 4), loc_r2 );

  int c7 = ifo.get();
  EXPECT_EQ( 'g', c7 );
  auto loc7 = ifo.cur_loc();
  EXPECT_EQ( FileLoc(file_info, 3, 1), loc7 );

  int c8 = ifo.get();
  EXPECT_EQ( 'h', c8 );
  auto loc8 = ifo.cur_loc();
  EXPECT_EQ( FileLoc(file_info, 3, 2), loc8 );

  int c9 = ifo.get();
  EXPECT_EQ( 'i', c9 );
  auto loc9 = ifo.cur_loc();
  EXPECT_EQ( FileLoc(file_info, 3, 3), loc9 );

  int r3 = ifo.get();
  EXPECT_EQ( '\n', r3 );
  auto loc_r3 = ifo.cur_loc();
  EXPECT_EQ( FileLoc(file_info, 3, 4), loc_r3 );

  int eof = ifo.get();
  EXPECT_EQ( EOF, eof );
  auto loc10 = ifo.cur_loc();
  EXPECT_EQ( FileLoc(file_info, 4, 1), loc10 );
}

class TestNewlineHandler :
  public NewlineHandler
{
public:

  TestNewlineHandler() = default;

  void
  newline(int lineno) override;


public:

  int mLineNo;

};

void
TestNewlineHandler::newline(int lineno)
{
  mLineNo = lineno;
}

TEST_P(InputFileObjTest, handler_test1)
{
  string path{DATAPATH + mFileName};
  ifstream is(path);
  ASSERT_TRUE( is.operator bool() );

  FileInfo file_info{path};
  InputFileObj ifo{is, file_info};

  auto handler = new TestNewlineHandler();
  ifo.reg_handler(unique_ptr<NewlineHandler>{handler});

  int c1 = ifo.get();
  EXPECT_EQ( 'a', c1 );
  EXPECT_EQ( 1, handler->mLineNo );

  int c2 = ifo.get();
  EXPECT_EQ( 'b', c2 );
  EXPECT_EQ( 1, handler->mLineNo );

  int c3 = ifo.get();
  EXPECT_EQ( 'c', c3 );
  EXPECT_EQ( 1, handler->mLineNo );

  int r1 = ifo.get();
  EXPECT_EQ( '\n', r1 );
  EXPECT_EQ( 2, handler->mLineNo );

  int c4 = ifo.get();
  EXPECT_EQ( 'd', c4 );
  EXPECT_EQ( 2, handler->mLineNo );

  int c5 = ifo.get();
  EXPECT_EQ( 'e', c5 );
  EXPECT_EQ( 2, handler->mLineNo );

  int c6 = ifo.get();
  EXPECT_EQ( 'f', c6 );
  EXPECT_EQ( 2, handler->mLineNo );

  int r2 = ifo.get();
  EXPECT_EQ( '\n', r2 );
  EXPECT_EQ( 3, handler->mLineNo );

  int c7 = ifo.get();
  EXPECT_EQ( 'g', c7 );
  EXPECT_EQ( 3, handler->mLineNo );

  int c8 = ifo.get();
  EXPECT_EQ( 'h', c8 );
  EXPECT_EQ( 3, handler->mLineNo );

  int c9 = ifo.get();
  EXPECT_EQ( 'i', c9 );
  EXPECT_EQ( 3, handler->mLineNo );

  int r3 = ifo.get();
  EXPECT_EQ( '\n', r3 );
  EXPECT_EQ( 4, handler->mLineNo );

  int eof = ifo.get();
  EXPECT_EQ( EOF, eof );
  EXPECT_EQ( 4, handler->mLineNo );

}

INSTANTIATE_TEST_SUITE_P(InputFileObj_test,
			 InputFileObjTest,
			 ::testing::Values("text_unix.txt", "text_mac.txt", "text_win.txt"));

END_NAMESPACE_YM
