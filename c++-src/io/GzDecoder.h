#ifndef GZDECODER_H
#define GZDECODER_H

/// @file GzDecoder.h
/// @brief GzDecoder のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "FileDecoder.h"
#include "zstream.h"
#include "FileBuff.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GzDecoder GzDecoder.h "GzDecoder.h"
/// @brief compress(Z) 形式の伸長器
//////////////////////////////////////////////////////////////////////
class GzDecoder :
  public FileDecoder
{
public:

  /// @brief コンストラクタ
  GzDecoder();

  /// @brief デストラクタ
  ~GzDecoder();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを開く
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルが存在しない．
  ///  - ファイルに対する読み出し許可がない．
  ///  - ファイルの形式が異なる．
  bool
  open(
    const char* filename ///< [in] ファイル名
  ) override;

  /// @brief ファイルを閉じる．
  void
  close() override;

  /// @brief 読み出せるデータがある時に true を返す．
  bool
  is_ready() const override;

  /// @brief 圧縮されたファイルを読んで最大 num バイトをバッファに格納する．
  /// @return 実際に読み出したバイト数を返す．
  /// @note エラーが起こったら -1 を返す．
  SizeType
  read(
    ymuint8* rbuff, ///< [in] 展開したデータを格納するバッファ
    SizeType num    ///< [in] 読み出すデータ数(バイト)
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ファイル用のバッファ
  FileBuff mBuff;

  // zstream
  zstream mZS;

  // CRC コード
  ymuint32 mCRC;

  // 出力バッファに書き込まれたサイズ
  SizeType mOutSize;

};

END_NAMESPACE_YM

#endif // GZDECODER_H
