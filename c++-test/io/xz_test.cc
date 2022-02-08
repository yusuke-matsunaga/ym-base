
/// @file xz_test.cc
/// @brief XzFile のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "XzFile.h"
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

  XzFile ibuff;
  auto ret = ibuff.inflate_open(xz_filename);
  ASSERT_TRUE( ret );
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
    XzFile obuff;
    auto ret = obuff.deflate_open(xz_filename, 0666);
    ASSERT_TRUE( ret );

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
    XzFile obuff{4096};
    int level = 6;
    auto ret = obuff.deflate_open(mFileName, 0666, level, LZMA_CHECK_CRC64);
    ASSERT_TRUE( ret );

    obuff.write(reinterpret_cast<const ymuint8*>(mTestData), mTestSize);
  }
  {
    XzFile ibuff{4096};
    SizeType memlimit = 128 * 1024 * 1024;
    ymuint32 flags = 0;
    auto ret = ibuff.inflate_open(mFileName, memlimit, flags);
    ASSERT_TRUE( ret );
    ibuff.read(reinterpret_cast<ymuint8*>(mBuff), mTestSize);
  }
  EXPECT_EQ( 0, memcmp(mTestData, mBuff, mTestSize) );
}

TEST_F(xz_test, small_obuffer)
{
  {
    XzFile obuff{10};
    int level = 6;
    auto ret = obuff.deflate_open(mFileName, 0666, level, LZMA_CHECK_CRC64);
    ASSERT_TRUE( ret );

    obuff.write(reinterpret_cast<const ymuint8*>(mTestData), mTestSize);
  }
  {
    XzFile ibuff{4096};
    SizeType memlimit = 128 * 1024 * 1024;
    ymuint32 flags = 0;
    auto ret = ibuff.inflate_open(mFileName, memlimit, flags);
    ASSERT_TRUE( ret );
    ibuff.read(reinterpret_cast<ymuint8*>(mBuff), mTestSize);
  }

  EXPECT_EQ( 0, memcmp(mTestData, mBuff, mTestSize) );
}

TEST_F(xz_test, small_ibuffer)
{
  {
    XzFile obuff{4096};
    int level = 6;
    auto ret = obuff.deflate_open(mFileName, 0666, level, LZMA_CHECK_CRC64);
    ASSERT_TRUE( ret );

    obuff.write(reinterpret_cast<const ymuint8*>(mTestData), mTestSize);
  }
  {
    XzFile ibuff{10};
    SizeType memlimit = 128 * 1024 * 1024;
    ymuint32 flags = 0;
    auto ret = ibuff.inflate_open(mFileName, memlimit, flags);
    ASSERT_TRUE( ret );
    ibuff.read(reinterpret_cast<ymuint8*>(mBuff), mTestSize);
  }

  EXPECT_EQ( 0, memcmp(mTestData, mBuff, mTestSize) );
}

END_NAMESPACE_YM
