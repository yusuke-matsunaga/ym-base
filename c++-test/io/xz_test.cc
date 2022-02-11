
/// @file xz_test.cc
/// @brief XzFile のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "XzEngine.h"
#include "codec_test.h"


BEGIN_NAMESPACE_YM

// テストフィクスチャ
class xz_test :
public codec_test
{
  // 実は codec_test の別名
};

// あらかじめ xz コマンドで圧縮したファイルを伸張する．
TEST_F(xz_test, inflate)
{
  string xz_filename = string{TESTDATA_DIR} + string{"/quick_brown_fox.txt.xz"};
  ifstream ifs{xz_filename};
  ASSERT_TRUE( ifs.is_open() );

  XzEngine ibuff{&ifs};

  auto size = ibuff.read(reinterpret_cast<ymuint8*>(mBuff), BUFF_SIZE);

  EXPECT_EQ( mTestSize, size );
  EXPECT_EQ( 0, memcmp(mTestData, mBuff, mTestSize) );
}

// XzFile の deflate モードで作ったファイルを unxz コマンドで伸長する．
TEST_F(xz_test, deflate)
{
  // unxz は拡張子の ".xz" を仮定している．
  string xz_filename = mFileName + string{".xz"};

  {
    ofstream ofs{xz_filename};
    ASSERT_TRUE( ofs.is_open() );

    XzEngine obuff{&ofs};

    obuff.write(reinterpret_cast<const ymuint8*>(mTestData), mTestSize);
  }

  {
    string cmd = string{"unxz "} + xz_filename + ";";
    cmd += " diff " + mFileName;
    cmd += string{" "} + string{TESTDATA_DIR} + string{"/quick_brown_fox.txt"};
    int ret = system(cmd.c_str());
    EXPECT_EQ( 0, ret );
  }
}

TEST_F(xz_test, big_buffer)
{
  {
    ofstream ofs{mFileName};
    ASSERT_TRUE( ofs.is_open() );

    XzEngine obuff{&ofs, 4096, nullptr, 6, LZMA_CHECK_CRC64};

    obuff.write(reinterpret_cast<const ymuint8*>(mTestData), mTestSize);
  }
  {
    ifstream ifs{mFileName};
    ASSERT_TRUE( ifs.is_open() );

    SizeType memlimit = 128 * 1024 * 1024;
    XzEngine ibuff{&ifs, 4096, nullptr, memlimit, 0};

    auto size = ibuff.read(reinterpret_cast<ymuint8*>(mBuff), mTestSize);
    EXPECT_EQ( mTestSize, size );
  }
  EXPECT_EQ( 0, memcmp(mTestData, mBuff, mTestSize) );
}

TEST_F(xz_test, small_obuffer)
{
  {
    ofstream ofs{mFileName};
    ASSERT_TRUE( ofs.is_open() );

    XzEngine obuff{&ofs, 10, nullptr, 6, LZMA_CHECK_CRC64};

    obuff.write(reinterpret_cast<const ymuint8*>(mTestData), mTestSize);
  }
  {
    ifstream ifs{mFileName};
    ASSERT_TRUE( ifs.is_open() );

    SizeType memlimit = 128 * 1024 * 1024;
    XzEngine ibuff{&ifs, 4096, nullptr, memlimit, 0};

    auto size = ibuff.read(reinterpret_cast<ymuint8*>(mBuff), mTestSize);
    EXPECT_EQ( mTestSize, size );
  }

  EXPECT_EQ( 0, memcmp(mTestData, mBuff, mTestSize) );
}

TEST_F(xz_test, small_ibuffer)
{
  {
    ofstream ofs{mFileName};
    ASSERT_TRUE( ofs.is_open() );

    XzEngine obuff{&ofs, 4096, nullptr, 6, LZMA_CHECK_CRC64};

    obuff.write(reinterpret_cast<const ymuint8*>(mTestData), mTestSize);
  }
  {
    ifstream ifs{mFileName};
    ASSERT_TRUE( ifs.is_open() );

    SizeType memlimit = 128 * 1024 * 1024;
    XzEngine ibuff{&ifs, 10, nullptr, memlimit, 0};

    auto size = ibuff.read(reinterpret_cast<ymuint8*>(mBuff), mTestSize);
    EXPECT_EQ( mTestSize, size );
  }

  EXPECT_EQ( 0, memcmp(mTestData, mBuff, mTestSize) );
}

END_NAMESPACE_YM
