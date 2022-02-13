
/// @file compress_test.cc
/// @brief compress_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include <stdlib.h>
#include "ZEngine.h"
#include "codec_test.h"


BEGIN_NAMESPACE_YM

// テストフィクスチャ
class compress_test :
public codec_test
{
  // 実は codec_test の別名
};

// あらかじめ compress コマンドで圧縮したファイルを伸張する．
TEST_F(compress_test, inflate)
{
  string z_filename = string{TESTDATA_DIR} + string{"/quick_brown_fox.txt.Z"};
  ifstream ifs{z_filename};
  ASSERT_TRUE( ifs.is_open() );

  ZEngine ibuff{ifs};

  auto size = ibuff.read(reinterpret_cast<ymuint8*>(mBuff), BUFF_SIZE);

  EXPECT_EQ( mTestSize, size );
  EXPECT_EQ( 0, memcmp(mTestData, mBuff, mTestSize) );
}

// ZFile の deflate モードで作ったファイルを uncompress コマンドで伸長する．
TEST_F(compress_test, deflate)
{
  // uncompress は拡張子の ".Z" を勝手に仮定している．
  string z_filename = mFileName + string{".Z"};
  {
    ofstream ofs{z_filename};
    ASSERT_TRUE( ofs.is_open() );

    ZEngine obuff{ofs};

    obuff.write(reinterpret_cast<const ymuint8*>(mTestData), mTestSize);
  }
  {
    string cmd = string{"uncompress "} + mFileName + ";";
    cmd += " diff " + mFileName;
    cmd += string{" "} + string{TESTDATA_DIR} + string{"/quick_brown_fox.txt"};
    int ret = system(cmd.c_str());
    EXPECT_EQ( 0, ret );
  }
}

TEST_F(compress_test, big_buffer)
{
  {
    ofstream ofs{mFileName};
    ASSERT_TRUE( ofs.is_open() );

    ZEngine obuff{ofs, 4096};

    obuff.write(reinterpret_cast<const ymuint8*>(mTestData), mTestSize);
  }
  {
    ifstream ifs{mFileName};
    ASSERT_TRUE( ifs.is_open() );

    ZEngine ibuff{ifs, 4096};

    auto size = ibuff.read(reinterpret_cast<ymuint8*>(mBuff), mTestSize);
    EXPECT_EQ( mTestSize, size );
  }
  EXPECT_EQ( 0, memcmp(mTestData, mBuff, mTestSize) );
}

TEST_F(compress_test, small_obuffer)
{
  {
    ofstream ofs{mFileName};
    ASSERT_TRUE( ofs.is_open() );

    ZEngine obuff{ofs, 10};

    obuff.write(reinterpret_cast<const ymuint8*>(mTestData), mTestSize);
  }
  {
    ifstream ifs{mFileName};
    ASSERT_TRUE( ifs.is_open() );

     ZEngine ibuff{ifs, 4096};

    auto size = ibuff.read(reinterpret_cast<ymuint8*>(mBuff), mTestSize);
    EXPECT_EQ( mTestSize, size );
  }
  EXPECT_EQ( 0, memcmp(mTestData, mBuff, mTestSize) );
}

TEST_F(compress_test, small_ibuffer)
{
  {
    ofstream ofs{mFileName};
    ASSERT_TRUE( ofs.is_open() );

    ZEngine obuff{ofs, 4096};

    obuff.write(reinterpret_cast<const ymuint8*>(mTestData), mTestSize);
  }
  {
    ifstream ifs{mFileName};
    ASSERT_TRUE( ifs.is_open() );

    ZEngine ibuff{ifs, 10};

    auto size = ibuff.read(reinterpret_cast<ymuint8*>(mBuff), mTestSize);
    EXPECT_EQ( mTestSize, size );
  }
  EXPECT_EQ( 0, memcmp(mTestData, mBuff, mTestSize) );
}

END_NAMESPACE_YM
