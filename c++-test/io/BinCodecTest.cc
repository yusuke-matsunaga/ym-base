
/// @file BinCodecTest.cc
/// @brief BinDec/BinEnc のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BinDec.h"
#include "ym/BinEnc.h"


BEGIN_NAMESPACE_YM

// 1バイトの読み書きのテスト
TEST(BinCodecTest, rw_8)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  ymuint8 oval{0xF0};
  ofs.write_8(oval);

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  ymuint8 ival = ifs.read_8();

  EXPECT_EQ( oval, ival );
}

// 1バイトの読み書きのテスト
TEST(BinCodecTest, s_8)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  ymuint8 oval{0xF0};
  ofs << oval;

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  ymuint8 ival;
  ifs >> ival;

  EXPECT_EQ( oval, ival );
}

// 2バイトの読み書きのテスト
TEST(BinCodecTest, rw_16)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  ymuint16 oval{0xF0A5};
  ofs.write_16(oval);

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  ymuint16 ival = ifs.read_16();

  EXPECT_EQ( oval, ival );
}

// 2バイトの読み書きのテスト
TEST(BinCodecTest, s_16)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  ymuint16 oval{0x1234};
  ofs << oval;

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  ymuint16 ival;
  ifs >> ival;

  EXPECT_EQ( oval, ival );
}

// 4バイトの読み書きのテスト
TEST(BinCodecTest, rw_32)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  ymuint32 oval{0xF0A536ED};
  ofs.write_32(oval);

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  ymuint32 ival = ifs.read_32();

  EXPECT_EQ( oval, ival );
}

// 4バイトの読み書きのテスト
TEST(BinCodecTest, s_32)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  ymuint32 oval{0x12345678};
  ofs << oval;

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  ymuint32 ival;
  ifs >> ival;

  EXPECT_EQ( oval, ival );
}

// 8バイトの読み書きのテスト
TEST(BinCodecTest, rw_64)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  ymuint64 oval{0xF0E1D2C3B4A59688};
  ofs.write_64(oval);

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  ymuint64 ival = ifs.read_64();

  EXPECT_EQ( oval, ival );
}

// 8バイトの読み書きのテスト
TEST(BinCodecTest, s_64)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  ymuint64 oval{0x123456789ABCDEF};
  ofs << oval;

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  ymuint64 ival;
  ifs >> ival;

  EXPECT_EQ( oval, ival );
}

// floatの読み書きのテスト
TEST(BinCodecTest, rw_float)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  float oval{1.234};
  ofs.write_float(oval);

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  float ival = ifs.read_float();

  EXPECT_EQ( oval, ival );
}

// floatの読み書きのテスト
TEST(BinCodecTest, s_float)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  float oval{9.82e+10};
  ofs << oval;

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  float ival;
  ifs >> ival;

  EXPECT_EQ( oval, ival );
}

// doubleの読み書きのテスト
TEST(BinCodecTest, rw_double)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  double oval{1.234};
  ofs.write_double(oval);

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  double ival = ifs.read_double();

  EXPECT_EQ( oval, ival );
}

// doubleの読み書きのテスト
TEST(BinCodecTest, s_double)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  double oval{9.82e+10};
  ofs << oval;

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  double ival;
  ifs >> ival;

  EXPECT_EQ( oval, ival );
}

// stringの読み書きのテスト
TEST(BinCodecTest, rw_string)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  string oval{"abcdefgh"};
  ofs.write_string(oval);

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  string ival = ifs.read_string();

  EXPECT_EQ( oval, ival );
}

// stringの読み書きのテスト
TEST(BinCodecTest, s_string)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  string oval{"_123"};
  ofs << oval;

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  string ival;
  ifs >> ival;

  EXPECT_EQ( oval, ival );
}

// 複合テスト
TEST(BinCodecTest, total_1)
{
  ostringstream obuff;
  BinEnc ofs{obuff};

  ymuint8  oval1{0xF0};
  ymuint16 oval2{0xF0A5};
  ymuint32 oval3{0xF0A536ED};
  ymuint64 oval4{0xF0E1D2C3B4A59688};
  float    oval5{1.234};
  double   oval6{9.82e+10};
  string   oval7{"abcdefgh"};

  ofs.write_8(oval1);
  ofs.write_16(oval2);
  ofs.write_32(oval3);
  ofs.write_64(oval4);
  ofs.write_float(oval5);
  ofs.write_double(oval6);
  ofs.write_string(oval7);

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};

  ymuint8  ival1 = ifs.read_8();
  ymuint16 ival2 = ifs.read_16();
  ymuint32 ival3 = ifs.read_32();
  ymuint64 ival4 = ifs.read_64();
  float    ival5 = ifs.read_float();
  double   ival6 = ifs.read_double();
  string   ival7 = ifs.read_string();

  EXPECT_EQ( oval1, ival1 );
  EXPECT_EQ( oval2, ival2 );
  EXPECT_EQ( oval3, ival3 );
  EXPECT_EQ( oval4, ival4 );
  EXPECT_EQ( oval5, ival5 );
  EXPECT_EQ( oval6, ival6 );
  EXPECT_EQ( oval7, ival7 );
}

END_NAMESPACE_YM
