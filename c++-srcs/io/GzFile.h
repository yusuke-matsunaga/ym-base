#ifndef GZFILE_H
#define GZFILE_H

/// @file GzFile.h
/// @brief GzFile のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ZStream.h"
#include "FileBuff.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GzFile GzFile.h "GzFile.h"
/// @brief gzip'ed 形式の圧縮/伸張を行なうファイルバッファ
///
/// FileBuff の継承クラスではないが，FileBuff と同様のインターフェイスを持つ．
//////////////////////////////////////////////////////////////////////
class GzFile :
  public ZStream
{
public:

  /// @brief コンストラクタ
  GzFile(
    SizeType buff_size = 4096, ///< [in] バッファサイズ
    alloc_func af = Z_NULL,    ///< [in] alloc 関数
    free_func ff = Z_NULL,     ///< [in] free 関数
    voidp op = Z_NULL          ///< [in] opaque オブジェクト
  );

  /// @brief デストラクタ
  ~GzFile();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを圧縮用に開く
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルに対する書き込み許可がない．
  bool
  deflate_open(
    const char* filename, ///< [in] ファイル名
    mode_t mode = 0666,   ///< [in] ファイル作成用のモード
    int level = 0         ///< [in] 圧縮レベル
  );

  /// @brief ファイルを圧縮用に開く
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルに対する書き込み許可がない．
  bool
  deflate_open(
    const string& filename, ///< [in] ファイル名
    mode_t mode = 0666,     ///< [in] ファイル作成用のモード
    int level = 0           ///< [in] 圧縮レベル
  )
  {
    return deflate_open(filename.c_str(), mode, level);
  }

  /// @brief ファイルを伸張用に開く
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルが存在しないか読み出し許可がない．
  bool
  inflate_open(
    const char* filename ///< [in] ファイル名
  );

  /// @brief ファイルを伸張用に開く
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルが存在しないか読み出し許可がない．
  bool
  inflate_open(
    const string& filename ///< [in] ファイル名
  )
  {
    return inflate_open(filename.c_str());
  }

  /// @brief ファイルを閉じる．
  void
  close();

  /// @brief 最大 num バイトのデータを圧縮してファイルに書き込む．
  void
  write(
    const ymuint8* wbuff, ///< [in] データを格納したバッファ
    SizeType num          ///< [in] 書き込むバイト数
  );

  /// @brief num バイトを読み込み buff[] に格納する．
  /// @return 読み込んだバイト数を返す．
  SizeType
  read(
    ymuint8* buff, ///< [in] データを格納するバッファ
    SizeType num   ///< [in] 読み込むバイト数．
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイルバッファ
  FileBuff mFileBuff;

  // CRC コード
  int mCRC;

  // 出力されたデータサイズ
  SizeType mOutSize;

};

END_NAMESPACE_YM

#endif // GZFILE_H
