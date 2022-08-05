#ifndef OBITSTREAM_H
#define OBITSTREAM_H

/// @file OBitStream.h
/// @brief OBitStream のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class OBitStream OBitStream.h "OBitStream.h"
/// @brief ビット単位で出力するストリームクラス
//////////////////////////////////////////////////////////////////////
class OBitStream
{
public:

  /// @brief コンストラクタ
  OBitStream(
    ostream& s ///< [in] 実際の出力先
  );

  /// @brief デストラクタ
  ~OBitStream();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ブール値(1ビット)出力
  void
  write_bool(
    bool val ///< [in] 値
  );

  /// @brief 8ビット値の出力
  void
  write_8(
    ymuint8 val ///< [in] 値
  );

  /// @brief 16ビット値の出力
  void
  write_16(
    ymuint16 val ///< [in] 値
  );

  /// @brief 32ビット値の出力
  void
  write_32(
    ymuint32 val ///< [in] 値
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バッファの内容を書き出す．
  void
  flush();

  /// @brief 1バイト進める．
  void
  next_byte();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の出力先のストリーム
  ostream& mS;

  // バッファサイズ
  static const SizeType BUFF_SIZE = 512;

  // バッファ
  ymuint8 mBuff[BUFF_SIZE];

  // バイト単位のオフセット
  SizeType mByteOffset{0};

  // ビット単位のオフセット
  SizeType mBitOffset{0};

};

END_NAMESPACE_YM

#endif // OBITSTREAM_H
