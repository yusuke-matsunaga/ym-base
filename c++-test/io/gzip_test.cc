
/// @file gzip_test.cc
/// @brief GzFile のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include <stdlib.h>
#include "GzEngine.h"
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
  ifstream ifs{filename};
  ASSERT_TRUE( ifs.is_open() );

  GzEngine ibuff{ifs};

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
    ofstream ofs{gz_filename};
    ASSERT_TRUE( ofs.is_open() );

    GzEngine obuff{ofs};

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
    ofstream ofs{mFileName};
    ASSERT_TRUE( ofs.is_open() );

    GzEngine obuff{ofs, 4096};
    obuff.write(reinterpret_cast<const ymuint8*>(mTestData), mTestSize);
  }
  {
    ifstream ifs{mFileName};
    ASSERT_TRUE( ifs.is_open() );

    GzEngine ibuff{ifs, 4096};
    auto size = ibuff.read(reinterpret_cast<ymuint8*>(mBuff), mTestSize);
    EXPECT_EQ( mTestSize, size );
  }
  EXPECT_EQ( 0, memcmp(mTestData, mBuff, mTestSize) );
}

TEST_F(gzip_test, small_obuffer)
{
  {
    ofstream ofs{mFileName};
    ASSERT_TRUE( ofs.is_open() );

    GzEngine obuff{ofs, 10};
    obuff.write(reinterpret_cast<const ymuint8*>(mTestData), mTestSize);
  }

  {
    ifstream ifs{mFileName};
    ASSERT_TRUE( ifs.is_open() );

    GzEngine ibuff{ifs, 4096};
    auto size = ibuff.read(reinterpret_cast<ymuint8*>(mBuff), mTestSize);
    EXPECT_EQ( mTestSize, size );
  }

  EXPECT_EQ( 0, memcmp(mTestData, mBuff, mTestSize) );
}

TEST_F(gzip_test, small_ibuffer)
{
  {
    ofstream ofs{mFileName};
    ASSERT_TRUE( ofs.is_open() );

    GzEngine obuff{ofs, 4096};
    obuff.write(reinterpret_cast<const ymuint8*>(mTestData), mTestSize);
  }

  {
    ifstream ifs{mFileName};
    ASSERT_TRUE( ifs.is_open() );

    GzEngine ibuff{ifs, 10};
    auto size = ibuff.read(reinterpret_cast<ymuint8*>(mBuff), mTestSize);
    EXPECT_EQ( mTestSize, size );
  }

  EXPECT_EQ( 0, memcmp(mTestData, mBuff, mTestSize) );
}

END_NAMESPACE_YM
