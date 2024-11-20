
/// @file BinEncDecTest.cc
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
TEST(BinEncDecTest, rw_8)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  std::uint8_t oval{0xF0};
  ofs.write_8(oval);

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  std::uint8_t ival = ifs.read_8();

  EXPECT_EQ( oval, ival );
}

// 1バイトの読み書きのテスト
TEST(BinEncDecTest, s_8)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  std::uint8_t oval{0xF0};
  ofs << oval;

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  std::uint8_t ival;
  ifs >> ival;

  EXPECT_EQ( oval, ival );
}

// 2バイトの読み書きのテスト
TEST(BinEncDecTest, rw_16)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  std::uint16_t oval{0xF0A5};
  ofs.write_16(oval);

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  std::uint16_t ival = ifs.read_16();

  EXPECT_EQ( oval, ival );
}

// 2バイトの読み書きのテスト
TEST(BinEncDecTest, s_16)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  std::uint16_t oval{0x1234};
  ofs << oval;

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  std::uint16_t ival;
  ifs >> ival;

  EXPECT_EQ( oval, ival );
}

// 4バイトの読み書きのテスト
TEST(BinEncDecTest, rw_32)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  std::uint32_t oval{0xF0A536ED};
  ofs.write_32(oval);

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  std::uint32_t ival = ifs.read_32();

  EXPECT_EQ( oval, ival );
}

// 4バイトの読み書きのテスト
TEST(BinEncDecTest, s_32)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  std::uint32_t oval{0x12345678};
  ofs << oval;

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  std::uint32_t ival;
  ifs >> ival;

  EXPECT_EQ( oval, ival );
}

// 8バイトの読み書きのテスト
TEST(BinEncDecTest, rw_64)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  std::uint64_t oval{0xF0E1D2C3B4A59688};
  ofs.write_64(oval);

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  std::uint64_t ival = ifs.read_64();

  EXPECT_EQ( oval, ival );
}

// 8バイトの読み書きのテスト
TEST(BinEncDecTest, s_64)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  std::uint64_t oval{0x123456789ABCDEF};
  ofs << oval;

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  std::uint64_t ival;
  ifs >> ival;

  EXPECT_EQ( oval, ival );
}

// 可変長整数の読み書きのテスト
TEST(BinEncDecTest, rw_vint)
{
  ostringstream obuff;
  BinEnc ofs{obuff};
  SizeType val1 = 0x0F;
  SizeType val2 = 0x0FA5;
  SizeType val3 = 0x0FA536ED;
  SizeType val4 = 0xF0E1D2C3B4A59688;

  ofs.write_vint(val1);
  ofs.write_vint(val2);
  ofs.write_vint(val3);
  ofs.write_vint(val4);

  istringstream ibuff{obuff.str()};
  BinDec ifs{ibuff};
  SizeType rval1 = ifs.read_vint();
  SizeType rval2 = ifs.read_vint();
  SizeType rval3 = ifs.read_vint();
  SizeType rval4 = ifs.read_vint();

  EXPECT_EQ( val1, rval1 );
  EXPECT_EQ( val2, rval2 );
  EXPECT_EQ( val3, rval3 );
  EXPECT_EQ( val4, rval4 );
}

// floatの読み書きのテスト
TEST(BinEncDecTest, rw_float)
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
TEST(BinEncDecTest, s_float)
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
TEST(BinEncDecTest, rw_double)
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
TEST(BinEncDecTest, s_double)
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
TEST(BinEncDecTest, rw_string)
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
TEST(BinEncDecTest, s_string)
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
TEST(BinEncDecTest, total_1)
{
  ostringstream obuff;
  BinEnc ofs{obuff};

  std::uint8_t  oval1{0xF0};
  std::uint16_t oval2{0xF0A5};
  std::uint32_t oval3{0xF0A536ED};
  std::uint64_t oval4{0xF0E1D2C3B4A59688};
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

  std::uint8_t  ival1 = ifs.read_8();
  std::uint16_t ival2 = ifs.read_16();
  std::uint32_t ival3 = ifs.read_32();
  std::uint64_t ival4 = ifs.read_64();
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
