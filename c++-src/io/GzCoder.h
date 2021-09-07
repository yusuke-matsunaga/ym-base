#ifndef GZCODER_H
#define GZCODER_H

/// @file GzCoder.h
/// @brief GzCoder のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "FileCoder.h"
#include "zstream.h"
#include "FileBuff.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GzCoder GzCoder.h "GzCoder.h"
/// @brief gzip'ed 形式の圧縮器
//////////////////////////////////////////////////////////////////////
class GzCoder :
  public FileCoder
{
public:

  /// @brief コンストラクタ
  GzCoder();

  /// @brief デストラクタ
  ~GzCoder();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @param[in] mode ファイル作成用のモード
  /// @param[in] level 圧縮レベル
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルに対する書き込み許可がない．
  bool
  open(
    const char* filename,
    mode_t mode = 0666,
    int level = 0
  ) override;

  /// @brief ファイルを閉じる．
  void
  close() override;

  /// @brief 書き込み可能の時に true を返す．
  bool
  is_ready() const override;

  /// @brief 最大 num バイトのデータを圧縮してファイルに書き込む．
  /// @param[in] wbuff 圧縮するデータを格納するバッファ
  /// @param[in] num 書き込むデータ数(バイト)
  /// @return 実際に書き込んだバイト数を返す．
  /// @note エラーが起こったら -1 を返す．
  SizeType
  write(
    const ymuint8* wbuff,
    SizeType num
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力用のバッファ
  FileBuff mBuff;

  // zstream
  zstream mZ;

  // CRC コード
  int mCRC;

  // 出力されたデータサイズ
  SizeType mOutSize;

};

END_NAMESPACE_YM

#endif // GZCODER_H
