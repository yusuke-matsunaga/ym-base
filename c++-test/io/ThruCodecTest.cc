
/// @file ThruCodecTest.cc
/// @brief ThruCodecTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ThruCoder.h"
#include "ThruDecoder.h"


BEGIN_NAMESPACE_YM

TEST(ThruCodecTest, coder_test1)
{
  char buff[] = "The quick brown fox jumps over the lazy dog.";
  SizeType n = sizeof(buff);
  ostringstream obuff;
  {
    ThruCoder coder{obuff, 4096};
    coder.write(reinterpret_cast<const ymuint8*>(buff), n);
  }

  EXPECT_EQ( 0, memcmp(buff, obuff.str().c_str(), n) );
}

TEST(ThruCodecTest, coder_test2)
{
  char buff[] = "The quick brown fox jumps over the lazy dog.";
  SizeType n = sizeof(buff);
  ostringstream obuff;
  {
    ThruCoder coder{obuff, 10};
    coder.write(reinterpret_cast<const ymuint8*>(buff), n);
  }

  EXPECT_EQ( 0, memcmp(buff, obuff.str().c_str(), n) );
}

TEST(ThruCodecTest, big_buffer)
{
  char buff[] = "The quick brown fox jumps over the lazy dog.";
  SizeType n = sizeof(buff);
  ostringstream obuff;
  {
    ThruCoder coder{obuff, 4096};
    coder.write(reinterpret_cast<const ymuint8*>(buff), n);
  }

  istringstream ibuff{obuff.str()};
  char buff2[1024];
  {
    ThruDecoder decoder{ibuff, 4096};
    decoder.read(reinterpret_cast<ymuint8*>(buff2), n);
  }

  EXPECT_EQ( 0, memcmp(buff, buff2, n) );
}

TEST(ThruCodecTest, small_obuffer)
{
  ostringstream obuff;

  char buff[] = "The quick brown fox jumps over the lazy sheep.";
  SizeType n = sizeof(buff);
  {
    ThruCoder coder{obuff, 4};
    coder.write(reinterpret_cast<const ymuint8*>(buff), n);
  }

  istringstream ibuff{obuff.str()};
  char buff2[1024];
  {
    ThruDecoder decoder{ibuff, 1024};
    decoder.read(reinterpret_cast<ymuint8*>(buff2), n);
  }

  EXPECT_EQ( 0, memcmp(buff, buff2, n) );
}

TEST(ThruCodecTest, small_ibuffer)
{
  ostringstream obuff;

  char buff[] = "The quick brown fox jumps over the lazy sheep.";
  SizeType n = sizeof(buff);
  {
    ThruCoder coder{obuff, 4096};
    coder.write(reinterpret_cast<const ymuint8*>(buff), n);
  }

  istringstream ibuff{obuff.str()};
  char buff2[1024];
  {
    ThruDecoder decoder{ibuff, 10};
    decoder.read(reinterpret_cast<ymuint8*>(buff2), n);
  }

  EXPECT_EQ( 0, memcmp(buff, buff2, n) );
}

END_NAMESPACE_YM
