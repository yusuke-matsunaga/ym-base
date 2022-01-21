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
    ostream& s ///< [in] 出力先のストリーム
  ) : mS{s}
  {
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
    ymuint8 val ///< [in] 値
  )
  {
    raw_write(&val, sizeof(ymuint8));
  }

  /// @brief 2バイトの書き込み
  void
  write_16(
    ymuint16 val ///< [in] 値
  )
  {
    raw_write(reinterpret_cast<const ymuint8*>(&val), sizeof(ymuint16));
  }

  /// @brief 4バイトの書き込み
  void
  write_32(
    ymuint32 val ///< [in] 値
  )
  {
    raw_write(reinterpret_cast<const ymuint8*>(&val), sizeof(ymuint32));
  }

  /// @brief 8バイトの書き込み
  void
  write_64(
    ymuint64 val ///< [in] 値
  )
  {
    raw_write(reinterpret_cast<const ymuint8*>(&val), sizeof(ymuint64));
  }

  /// @brief 単精度浮動小数点数の書き込み
  void
  write_float(
    float val ///< [in] 値
  )
  {
    raw_write(reinterpret_cast<const ymuint8*>(&val), sizeof(float));
  }

  /// @brief 倍精度浮動小数点数の書き込み
  void
  write_double(
    double val ///< [in] 値
  )
  {
    raw_write(reinterpret_cast<const ymuint8*>(&val), sizeof(double));
  }

  /// @brief 文字列の書き込み
  void
  write_string(
    const string& val ///< [in] 値
  )
  {
    auto l = val.size();
    write_64(l);
    raw_write(reinterpret_cast<const ymuint8*>(val.c_str()), l);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief write_XXX() の下請け関数
  void
  raw_write(
    const ymuint8* buff, ///< [in] データを収めた領域のアドレス
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
  ostream& mS;

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
  s.write_8(static_cast<ymuint8>(val));
  return s;
}

/// @brief 1バイトの書き込み
/// @return BinEnc を返す．
inline
BinEnc&
operator<<(
  BinEnc& s, ///< [in] 出力先のストリーム
  ymint8 val ///< [in] 値
)
{
  s.write_8(static_cast<ymuint8>(val));
  return s;
}

/// @brief 2バイトの書き込み
/// @return BinEnc を返す．
inline
BinEnc&
operator<<(
  BinEnc& s,  ///< [in] 出力先のストリーム
  ymint16 val ///< [in] 値
)
{
  s.write_16(static_cast<ymuint16>(val));
  return s;
}

/// @brief 4バイトの書き込み
/// @return BinEnc を返す．
inline
BinEnc&
operator<<(
  BinEnc& s,  ///< [in] 出力先のストリーム
  ymint32 val ///< [in] 値
)
{
  s.write_32(static_cast<ymuint32>(val));
  return s;
}

/// @brief 8バイトの書き込み
/// @return BinEnc を返す．
inline
BinEnc&
operator<<(
  BinEnc& s,  ///< [in] 出力先のストリーム
  ymint64 val ///< [in] 値
)
{
  s.write_64(static_cast<ymuint64>(val));
  return s;
}

/// @brief 1バイトの書き込み
/// @return BinEnc を返す．
inline
BinEnc&
operator<<(
  BinEnc& s,  ///< [in] 出力先のストリーム
  ymuint8 val ///< [in] 値
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
  ymuint16 val ///< [in] 値
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
  ymuint32 val ///< [in] 値
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
  ymuint64 val ///< [in] 値
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
  BinEnc& s,        ///< [in] 出力先のストリーム
  const string& val ///< [in] 値
)
{
  s.write_string(val);
  return s;
}

END_NAMESPACE_YM

#endif // BINENC_H
