#ifndef BINENC_H
#define BINENC_H

/// @file BinEnc.h
/// @brief BinEnc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class BinEnc BinEnc.h "ym/BinEnc.h"
/// @brief バイナリエンコーダー
///
/// ostream (の派生クラス)に対するフィルタとして働く．
/// @sa BinDec
//////////////////////////////////////////////////////////////////////
class BinEnc
{
public:

  /// @brief コンストラクタ
  BinEnc(
    std::ostream& s ///< [in] 出力先のストリーム
  ) : mS{s}
  {
    // fail|bad の時に例外を送出するようにする．
    mS.exceptions(std::ios_base::failbit | std::ios_base::badbit);
  }

  /// @brief デストラクタ
  ~BinEnc() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 1バイトの書き込み
  void
  write_8(
    std::uint8_t val ///< [in] 値
  )
  {
    raw_write(&val, sizeof(std::uint8_t));
  }

  /// @brief 2バイトの書き込み
  void
  write_16(
    std::uint16_t val ///< [in] 値
  )
  {
    raw_write(reinterpret_cast<const std::uint8_t*>(&val), sizeof(std::uint16_t));
  }

  /// @brief 4バイトの書き込み
  void
  write_32(
    std::uint32_t val ///< [in] 値
  )
  {
    raw_write(reinterpret_cast<const std::uint8_t*>(&val), sizeof(std::uint32_t));
  }

  /// @brief 8バイトの書き込み
  void
  write_64(
    std::uint64_t val ///< [in] 値
  )
  {
    raw_write(reinterpret_cast<const std::uint8_t*>(&val), sizeof(std::uint64_t));
  }

  /// @brief 可変長の整数の書き込み
  void
  write_vint(
    SizeType val ///< [in] 値
  );

  /// @brief 単精度浮動小数点数の書き込み
  void
  write_float(
    float val ///< [in] 値
  )
  {
    raw_write(reinterpret_cast<const std::uint8_t*>(&val), sizeof(float));
  }

  /// @brief 倍精度浮動小数点数の書き込み
  void
  write_double(
    double val ///< [in] 値
  )
  {
    raw_write(reinterpret_cast<const std::uint8_t*>(&val), sizeof(double));
  }

  /// @brief 文字列の書き込み
  void
  write_string(
    const std::string& val ///< [in] 値
  )
  {
    auto l = val.size();
    write_64(l);
    raw_write(reinterpret_cast<const std::uint8_t*>(val.c_str()), l);
  }

  /// @brief ブロックデータの書き込み
  void
  write_block(
    const std::uint8_t* block, ///< [in] ブロックの先頭アドレス
    SizeType n            ///< [in] データサイズ
  )
  {
    raw_write(block, n);
  }

  /// @brief シグネチャの書き込み
  ///
  /// write_string() と異なり文字数を書き込まない．
  void
  write_signature(
    const std::string& signature ///< [in] シグネチャ文字列
  )
  {
    auto l = signature.size();
    raw_write(reinterpret_cast<const std::uint8_t*>(signature.c_str()), l);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief write_XXX() の下請け関数
  void
  raw_write(
    const std::uint8_t* buff, ///< [in] データを収めた領域のアドレス
    SizeType n           ///< [in] データサイズ
  )
  {
    mS.write(reinterpret_cast<const char*>(buff), n);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力先のストリーム
  std::ostream& mS;

};


//////////////////////////////////////////////////////////////////////
// BinEnc に対するストリーム出力演算子
//////////////////////////////////////////////////////////////////////

/// @brief ブール値の書き込み
/// @return BinEnc を返す．
inline
BinEnc&
operator<<(
  BinEnc& s, ///< [in] 出力先のストリーム
  bool val   ///< [in] 値
)
{
  s.write_8(static_cast<std::uint8_t>(val));
  return s;
}

/// @brief 1バイトの書き込み
/// @return BinEnc を返す．
inline
BinEnc&
operator<<(
  BinEnc& s, ///< [in] 出力先のストリーム
  std::int8_t val ///< [in] 値
)
{
  s.write_8(static_cast<std::uint8_t>(val));
  return s;
}

/// @brief 2バイトの書き込み
/// @return BinEnc を返す．
inline
BinEnc&
operator<<(
  BinEnc& s,  ///< [in] 出力先のストリーム
  std::int16_t val ///< [in] 値
)
{
  s.write_16(static_cast<std::uint16_t>(val));
  return s;
}

/// @brief 4バイトの書き込み
/// @return BinEnc を返す．
inline
BinEnc&
operator<<(
  BinEnc& s,  ///< [in] 出力先のストリーム
  std::int32_t val ///< [in] 値
)
{
  s.write_32(static_cast<std::uint32_t>(val));
  return s;
}

/// @brief 8バイトの書き込み
/// @return BinEnc を返す．
inline
BinEnc&
operator<<(
  BinEnc& s,  ///< [in] 出力先のストリーム
  std::int64_t val ///< [in] 値
)
{
  s.write_64(static_cast<std::uint64_t>(val));
  return s;
}

/// @brief 1バイトの書き込み
/// @return BinEnc を返す．
inline
BinEnc&
operator<<(
  BinEnc& s,  ///< [in] 出力先のストリーム
  std::uint8_t val ///< [in] 値
)
{
  s.write_8(val);
  return s;
}

/// @brief 2バイトの書き込み
/// @return BinEnc を返す．
inline
BinEnc&
operator<<(
  BinEnc& s,   ///< [in] 出力先のストリーム
  std::uint16_t val ///< [in] 値
)
{
  s.write_16(val);
  return s;
}

/// @brief 4バイトの書き込み
/// @return BinEnc を返す．
inline
BinEnc&
operator<<(
  BinEnc& s,   ///< [in] 出力先のストリーム
  std::uint32_t val ///< [in] 値
)
{
  s.write_32(val);
  return s;
}

/// @brief 8バイトの書き込み
/// @return BinEnc を返す．
inline
BinEnc&
operator<<(
  BinEnc& s,   ///< [in] 出力先のストリーム
  std::uint64_t val ///< [in] 値
)
{
  s.write_64(val);
  return s;
}

/// @brief 単精度浮動小数点数の書き込み
/// @return BinEnc を返す．
inline
BinEnc&
operator<<(
  BinEnc& s, ///< [in] 出力先のストリーム
  float val  ///< [in] 値
)
{
  s.write_float(val);
  return s;
}

/// @brief 倍精度浮動小数点数の書き込み
/// @return BinEnc を返す．
inline
BinEnc&
operator<<(
  BinEnc& s, ///< [in] 出力先のストリーム
  double val ///< [in] 値
)
{
  s.write_double(val);
  return s;
}

/// @brief 文字列の書き込み
/// @return BinEnc を返す．
inline
BinEnc&
operator<<(
  BinEnc& s,             ///< [in] 出力先のストリーム
  const std::string& val ///< [in] 値
)
{
  s.write_string(val);
  return s;
}

END_NAMESPACE_YM

#endif // BINENC_H
