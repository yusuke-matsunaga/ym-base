#ifndef BINDEC_H
#define BINDEC_H

/// @file BinDec.h
/// @brief BinDec のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


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
    // fail|bad の時に例外を送出するようにする．
    mS.exceptions(std::ios_base::failbit | std::ios_base::badbit);
  }

  /// @brief デストラクタ
  ~BinDec() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 1バイトの読み出し
  /// @return 読み込んだ値を返す．
  std::uint8_t
  read_8()
  {
    std::uint8_t buff[1];
    raw_read(buff, 1);
    return buff[0];
  }

  /// @brief 2バイトの読み出し
  /// @return 読み込んだ値を返す．
  std::uint16_t
  read_16()
  {
    std::uint8_t buff[2];
    raw_read(buff, 2);
    std::uint16_t val =
      (static_cast<std::uint16_t>(buff[0]) <<  0) |
      (static_cast<std::uint16_t>(buff[1]) <<  8);
    return val;
  }

  /// @brief 4バイトの読み出し
  /// @return 読み込んだ値を返す．
  std::uint32_t
  read_32()
  {
    std::uint8_t buff[4];
    raw_read(buff, 4);
    std::uint32_t val =
      (static_cast<std::uint32_t>(buff[0]) <<  0) |
      (static_cast<std::uint32_t>(buff[1]) <<  8) |
      (static_cast<std::uint32_t>(buff[2]) << 16) |
      (static_cast<std::uint32_t>(buff[3]) << 24);
    return val;
  }

  /// @brief 8バイトの読み出し
  /// @return 読み込んだ値を返す．
  std::uint64_t
  read_64()
  {
    std::uint8_t buff[8];
    raw_read(buff, 8);
    std::uint64_t val =
      (static_cast<std::uint64_t>(buff[0]) <<  0) |
      (static_cast<std::uint64_t>(buff[1]) <<  8) |
      (static_cast<std::uint64_t>(buff[2]) << 16) |
      (static_cast<std::uint64_t>(buff[3]) << 24) |
      (static_cast<std::uint64_t>(buff[4]) << 32) |
      (static_cast<std::uint64_t>(buff[5]) << 40) |
      (static_cast<std::uint64_t>(buff[6]) << 48) |
      (static_cast<std::uint64_t>(buff[7]) << 56);
    return val;
  }

  /// @brief 可変長の整数の読み出し
  /// @return 読み込んだ値を返す．
  SizeType
  read_vint();

  /// @brief 単精度不動週数点数の読み出し
  /// @return 読み込んだ値を返す．
  float
  read_float()
  {
    std::uint8_t buff[sizeof(float)];
    raw_read(buff, sizeof(float));
    // かなり強引
    return *(reinterpret_cast<float*>(buff));
  }

  /// @brief 倍精度不動週数点数の読み出し
  /// @return 読み込んだ値を返す．
  double
  read_double()
  {
    std::uint8_t buff[sizeof(double)];
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
      std::uint8_t* buff = new std::uint8_t[l + 1];
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
    std::uint8_t* block, ///< [in] データを格納するブロックの先頭のアドレス
    SizeType n      ///< [in] データサイズ
  )
  {
    raw_read(block, n);
  }

  /// @brief シグネチャの読み出し
  /// @return 与えられたシグネチャと一致したら true を返す．
  bool
  read_signature(
    const string& signature ///< [in] シグネチャ文字列
  )
  {
    auto l = signature.size();
    std::uint8_t* buff = new std::uint8_t[l + 1];
    raw_read(buff, l);
    buff[l] = '\0';
    string tmp{reinterpret_cast<char*>(buff)};
    delete [] buff;
    return tmp == signature;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief read_XXX() の下請け関数
  void
  raw_read(
    std::uint8_t* buff, ///< [in] 読み出した値を格納する領域のアドレス
    SizeType n     ///< [in] 読み出すバイト数
  )
  {
    mS.read(reinterpret_cast<char*>(buff), n);
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
  std::int8_t& val ///< [out] 値を格納する変数
)
{
  val = static_cast<std::int8_t>(s.read_8());
  return s;
}

/// @brief 2バイトの読み出し
/// @return BinDec を返す．
inline
BinDec&
operator>>(
  BinDec& s,   ///< [in] 入力元のストリーム
  std::int16_t& val ///< [out] 値を格納する変数
)
{
  val = static_cast<std::uint16_t>(s.read_16());
  return s;
}

/// @brief 4バイトの読み出し
/// @return BinDec を返す．
inline
BinDec&
operator>>(
  BinDec& s,   ///< [in] 入力元のストリーム
  std::int32_t& val ///< [out] 値を格納する変数
)
{
  val = static_cast<std::uint32_t>(s.read_32());
  return s;
}

/// @brief 8バイトの読み出し
/// @return BinDec を返す．
inline
BinDec&
operator>>(
  BinDec& s,   ///< [in] 入力元のストリーム
  std::int64_t& val ///< [out] 値を格納する変数
)
{
  val = static_cast<std::uint64_t>(s.read_64());
  return s;
}

/// @brief 1バイトの読み出し
/// @return BinDec を返す．
inline
BinDec&
operator>>(
  BinDec& s,   ///< [in] 入力元のストリーム
  std::uint8_t& val ///< [out] 値を格納する変数
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
  std::uint16_t& val ///< [out] 値を格納する変数
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
  std::uint32_t& val ///< [out] 値を格納する変数
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
  std::uint64_t& val ///< [out] 値を格納する変数
)
{
  val = s.read_64();
  return s;
}

/// @brief SizeTypeの読み出し
/// @return BinDec を返す．
inline
BinDec&
operator>>(
  BinDec& s,    ///< [in] 入力元のストリーム
  SizeType& val ///< [out] 値を格納する変数
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
