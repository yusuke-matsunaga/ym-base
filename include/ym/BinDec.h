#ifndef BINDEC_H
#define BINDEC_H

/// @file BinDec.h
/// @brief BinDec のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include "ym/MsgMgr.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class BinDec BinDec.h "ym/BinDec.h"
/// @brief バイナリデコーダー
///
/// istream のフィルタとして働く
/// @sa BinEnc
//////////////////////////////////////////////////////////////////////
class BinDec
{
public:

  /// @brief コンストラクタ
  BinDec(
    istream& s ///< [in] 入力元のストリーム
  ) : mS{s}
  {
  }

  /// @brief デストラクタ
  ~BinDec() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 1バイトの読み出し
  /// @return 読み込んだ値を返す．
  ymuint8
  read_8()
  {
    ymuint8 buff[1];
    raw_read(buff, 1);
    return buff[0];
  }

  /// @brief 2バイトの読み出し
  /// @return 読み込んだ値を返す．
  ymuint16
  read_16()
  {
    ymuint8 buff[2];
    raw_read(buff, 2);
    ymuint16 val =
      (static_cast<ymuint16>(buff[0]) <<  0) |
      (static_cast<ymuint16>(buff[1]) <<  8);
    return val;
  }

  /// @brief 4バイトの読み出し
  /// @return 読み込んだ値を返す．
  ymuint32
  read_32()
  {
    ymuint8 buff[4];
    raw_read(buff, 4);
    ymuint32 val =
      (static_cast<ymuint32>(buff[0]) <<  0) |
      (static_cast<ymuint32>(buff[1]) <<  8) |
      (static_cast<ymuint32>(buff[2]) << 16) |
      (static_cast<ymuint32>(buff[3]) << 24);
    return val;
  }

  /// @brief 8バイトの読み出し
  /// @return 読み込んだ値を返す．
  ymuint64
  read_64()
  {
    ymuint8 buff[8];
    raw_read(buff, 8);
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

  /// @brief 単精度不動週数点数の読み出し
  /// @return 読み込んだ値を返す．
  float
  read_float()
  {
    ymuint8 buff[sizeof(float)];
    raw_read(buff, sizeof(float));
    // かなり強引
    return *(reinterpret_cast<float*>(buff));
  }

  /// @brief 倍精度不動週数点数の読み出し
  /// @return 読み込んだ値を返す．
  double
  read_double()
  {
    ymuint8 buff[sizeof(double)];
    raw_read(buff, sizeof(double));
    // かなり強引
    return *(reinterpret_cast<double*>(buff));
  }

  /// @brief 文字列の読み出し
  /// @return 読み込んだ値を返す．
  string
  read_string()
  {
    auto l = read_64();
    if ( l > 0 ) {
      ymuint8* buff = new ymuint8[l + 1];
      raw_read(buff, l);
      buff[l] = '\0';
      string ans(reinterpret_cast<char*>(buff));
      delete [] buff;
      return ans;
    }
    else {
      return string{};
    }
  }

  /// @brief ブロックの読み出し
  void
  read_block(
    ymuint8* block, ///< [in] データを格納するブロックの先頭のアドレス
    SizeType n      ///< [in] データサイズ
  )
  {
    raw_read(block, n);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief read_XXX() の下請け関数
  ///
  /// n バイト読みだせなかった場合にはエラーメッセージを出力する．
  void
  raw_read(
    ymuint8* buff, ///< [in] 読み出した値を格納する領域のアドレス
    SizeType n     ///< [in] 読み出すバイト数
  )
  {
    mS.read(reinterpret_cast<char*>(buff), n);
    if ( mS.fail() ) {
      ostringstream buf;
      buf << "BinDec::raw_read(" << n << ") failed.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      FileRegion{},
		      MsgType::Failure,
		      "BinDec",
		      buf.str());
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ストリーム
  istream& mS;

};


//////////////////////////////////////////////////////////////////////
// BinDec に対するストリーム入力演算子
//////////////////////////////////////////////////////////////////////

/// @brief ブール値の読み出し
/// @return BinDec を返す．
inline
BinDec&
operator>>(
  BinDec& s, ///< [in] 入力元のストリーム
  bool& val  ///< [out] 値を格納する変数
)
{
  val = static_cast<bool>(s.read_8());
  return s;
}

/// @brief 1バイトの読み出し
/// @return BinDec を返す．
inline
BinDec&
operator>>(
  BinDec& s,  ///< [in] 入力元のストリーム
  ymint8& val ///< [out] 値を格納する変数
)
{
  val = static_cast<ymint8>(s.read_8());
  return s;
}

/// @brief 2バイトの読み出し
/// @return BinDec を返す．
inline
BinDec&
operator>>(
  BinDec& s,   ///< [in] 入力元のストリーム
  ymint16& val ///< [out] 値を格納する変数
)
{
  val = static_cast<ymuint16>(s.read_16());
  return s;
}

/// @brief 4バイトの読み出し
/// @return BinDec を返す．
inline
BinDec&
operator>>(
  BinDec& s,   ///< [in] 入力元のストリーム
  ymint32& val ///< [out] 値を格納する変数
)
{
  val = static_cast<ymuint32>(s.read_32());
  return s;
}

/// @brief 8バイトの読み出し
/// @return BinDec を返す．
inline
BinDec&
operator>>(
  BinDec& s,   ///< [in] 入力元のストリーム
  ymint64& val ///< [out] 値を格納する変数
)
{
  val = static_cast<ymuint64>(s.read_64());
  return s;
}

/// @brief 1バイトの読み出し
/// @return BinDec を返す．
inline
BinDec&
operator>>(
  BinDec& s,   ///< [in] 入力元のストリーム
  ymuint8& val ///< [out] 値を格納する変数
)
{
  val = s.read_8();
  return s;
}

/// @brief 2バイトの読み出し
/// @return BinDec を返す．
inline
BinDec&
operator>>(
  BinDec& s,    ///< [in] 入力元のストリーム
  ymuint16& val ///< [out] 値を格納する変数
)
{
  val = s.read_16();
  return s;
}

/// @brief 4バイトの読み出し
/// @return BinDec を返す．
inline
BinDec&
operator>>(
  BinDec& s,    ///< [in] 入力元のストリーム
  ymuint32& val ///< [out] 値を格納する変数
)
{
  val = s.read_32();
  return s;
}

/// @brief 8バイトの読み出し
/// @return BinDec を返す．
inline
BinDec&
operator>>(
  BinDec& s,    ///< [in] 入力元のストリーム
  ymuint64& val ///< [out] 値を格納する変数
)
{
  val = s.read_64();
  return s;
}

/// @brief 単精度不動週数点数の読み出し
/// @return BinDec を返す．
inline
BinDec&
operator>>(
  BinDec& s, ///< [in] 入力元のストリーム
  float& val ///< [out] 値を格納する変数
)
{
  val = s.read_float();
  return s;
}

/// @brief 倍精度不動週数点数の読み出し
/// @return BinDec を返す．
inline
BinDec&
operator>>(
  BinDec& s,  ///< [in] 入力元のストリーム
  double& val ///< [out] 値を格納する変数
)
{
  val = s.read_double();
  return s;
}

/// @brief 文字列の読み出し
/// @return BinDec を返す．
inline
BinDec&
operator>>(
  BinDec& s,  ///< [in] 入力元のストリーム
  string& val ///< [out] 値を格納する変数
)
{
  val = s.read_string();
  return s;
}

END_NAMESPACE_YM

#endif // BINDEC_H
