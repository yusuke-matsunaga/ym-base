
/// @file gzip_test.cc
/// @brief GzFile のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include <stdlib.h>
#include "GzFile.h"
#include "codec_test.h"


BEGIN_NAMESPACE_YM

// テストフィクスチャ
class gzip_test :
public codec_test
{
  // 実は codec_test の別名
};

// あらかじめ gzip コマンドで圧縮したファイルを伸張する．
TEST_F(gzip_test, inflate)
{
  string filename = string{TESTDATA_DIR} + string{"/quick_brown_fox.txt.gz"};

  GzFile ibuff;
  auto ret = ibuff.inflate_open(filename);
  ASSERT_TRUE( ret );
  auto size = ibuff.read(reinterpret_cast<ymuint8*>(mBuff), BUFF_SIZE);

  EXPECT_EQ( mTestSize, size );
  EXPECT_EQ( 0, memcmp(mTestData, mBuff, mTestSize) );
}

// BzFile の deflate モードで作ったファイルを bunzip2 コマンドで伸長する．
TEST_F(gzip_test, deflate)
{
  // gunzip は拡張子の ".gz" を仮定している．
  string gz_filename = mFileName + string{".gz"};
  {
    GzFile obuff;
    auto ret = obuff.deflate_open(gz_filename, 0666);
    ASSERT_TRUE( ret );

    obuff.write(reinterpret_cast<const ymuint8*>(mTestData), mTestSize);
  }
  {
    string cmd = string{"gunzip "} + gz_filename + ";";
    cmd += " diff " + mFileName;
    cmd += string{" "} + string{TESTDATA_DIR} + string{"/quick_brown_fox.txt"};
    int ret = system(cmd.c_str());
    EXPECT_EQ( 0, ret );
  }
}

TEST_F(gzip_test, big_buffer)
{
  {
    GzFile obuff{4096};
    int level = 6;
    auto ret = obuff.deflate_open(mFileName, 0666, level);
    ASSERT_TRUE( ret );

    obuff.write(reinterpret_cast<const ymuint8*>(mTestData), mTestSize);
  }
  {
    GzFile ibuff{4096};
    auto ret = ibuff.inflate_open(mFileName);
    ASSERT_TRUE( ret );
    ibuff.read(reinterpret_cast<ymuint8*>(mBuff), mTestSize);
  }
  EXPECT_EQ( 0, memcmp(mTestData, mBuff, mTestSize) );
}

TEST_F(gzip_test, small_obuffer)
{
  {
    GzFile obuff{10};
    int level = 6;
    auto ret = obuff.deflate_open(mFileName, 0666, level);
    ASSERT_TRUE( ret );

    obuff.write(reinterpret_cast<const ymuint8*>(mTestData), mTestSize);
  }
  {
    GzFile ibuff{4096};
    auto ret = ibuff.inflate_open(mFileName);
    ASSERT_TRUE( ret );
    ibuff.read(reinterpret_cast<ymuint8*>(mBuff), mTestSize);
  }

  EXPECT_EQ( 0, memcmp(mTestData, mBuff, mTestSize) );
}

TEST_F(gzip_test, small_ibuffer)
{
  {
    GzFile obuff{4096};
    int level = 6;
    auto ret = obuff.deflate_open(mFileName, 0666, level);
    ASSERT_TRUE( ret );

    obuff.write(reinterpret_cast<const ymuint8*>(mTestData), mTestSize);
  }
  {
    GzFile ibuff{10};
    auto ret = ibuff.inflate_open(mFileName);
    ASSERT_TRUE( ret );
    ibuff.read(reinterpret_cast<ymuint8*>(mBuff), mTestSize);
  }

  EXPECT_EQ( 0, memcmp(mTestData, mBuff, mTestSize) );
}

END_NAMESPACE_YM
