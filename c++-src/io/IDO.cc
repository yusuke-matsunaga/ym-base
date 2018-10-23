
/// @file IDO.cc
/// @brief IDO の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/IDO.h"
#include "ym/MsgMgr.h"
#include "ym/FileRegion.h"
#include "ym/StrBuff.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス IDO
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IDO::IDO()
{
}

// @brief デストラクタ
IDO::~IDO()
{
}

// @brief 1バイトの読み出し
// @param[in] s 入力元のストリーム
ymuint8
IDO::read_8()
{
  ymuint8 buff[1];

  _read(buff, 1);
  return buff[0];
}

// @brief 2バイトの読み出し
// @param[in] s 入力元のストリーム
ymuint16
IDO::read_16()
{
  ymuint8 buff[2];

  _read(buff, 2);
  ymuint16 val =
    (static_cast<ymuint16>(buff[0]) <<  0) |
    (static_cast<ymuint16>(buff[1]) <<  8);
  return val;
}

// @brief 4バイトの読み出し
// @param[in] s 入力元のストリーム
ymuint32
IDO::read_32()
{
  ymuint8 buff[4];

  _read(buff, 4);
  ymuint32 val =
    (static_cast<ymuint32>(buff[0]) <<  0) |
    (static_cast<ymuint32>(buff[1]) <<  8) |
    (static_cast<ymuint32>(buff[2]) << 16) |
    (static_cast<ymuint32>(buff[3]) << 24);
  return val;
}

// @brief 8バイトの読み出し
// @param[in] s 入力元のストリーム
ymuint64
IDO::read_64()
{
  ymuint8 buff[8];

  _read(buff, 8);
  ymuint64 val =
    (static_cast<ymuint64>(buff[0]) <<  0) |
    (static_cast<ymuint64>(buff[1]) <<  8) |
    (static_cast<ymuint64>(buff[2]) << 16) |
    (static_cast<ymuint64>(buff[3]) << 24) |
    (static_cast<ymuint64>(buff[4]) << 32) |
    (static_cast<ymuint64>(buff[5]) << 40) |
    (static_cast<ymuint64>(buff[6]) << 48) |
    (static_cast<ymuint64>(buff[7]) << 56);
  return val;
}

// @brief 単精度不動週数点数の読み出し
// @param[in] s 入力元のストリーム
float
IDO::read_float()
{
  ymuint8 buff[sizeof(float)];

  // かなり強引
  _read(buff, sizeof(float));
  return *(reinterpret_cast<float*>(buff));
}

// @brief 倍精度不動週数点数の読み出し
// @param[in] s 入力元のストリーム
double
IDO::read_double()
{
  ymuint8 buff[sizeof(double)];

  // かなり強引
  _read(buff, sizeof(double));
  return *(reinterpret_cast<double*>(buff));
}

// @brief 文字列の読み出し
// @param[in] s 入力元のストリーム
string
IDO::read_str()
{
  ymuint64 l = read_64();
  if ( l > 0 ) {
    ymuint8* strbuf = new ymuint8[l + 1];
    _read(strbuf, l);
    strbuf[l] = '\0';
    string ans(reinterpret_cast<char*>(strbuf));
    delete [] strbuf;
    return ans;
  }
  else {
    return string();
  }
}

// @brief 一行の読み出し
// @param[out] str 読みだした文字列を格納する変数
// @return 読み出しが成功したら true を返す．
//
// 空行の場合には str に string() を入れて true を返す．
// データがなければ false を返す．
bool
IDO::read_line(string& str)
{
  bool empty = true;
  StrBuff buf;
  for ( ; ; ) {
    ymuint8 c;
    int ret = read(&c, 1);
    if ( ret == 0 ) {
      break;
    }
    empty = false;
    if ( c == '\n' || c == '\r' ) {
      break;
    }
    buf.put_char(c);
  }
  if ( empty ) {
    return false;
  }
  str = buf;
  return true;
}

// @brief read() を呼び出して結果をチェックする．
void
IDO::_read(ymuint8* buff,
	   int n)
{
  int ret = read(buff, n);
  if ( ret != n ) {
    ostringstream buf;
    buf << "IDO::_read(" << n << ") failed. read " << ret << " bytes.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion(),
		    MsgType::Failure,
		    "IDO",
		    buf.str());
  }
}

END_NAMESPACE_YM
