#ifndef XZFILE_H
#define XZFILE_H

/// @file XzFile.h
/// @brief XzFile のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "LzmaStream.h"
#include "FileBuff.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class XzFile XzFile.h "XzFile.h"
/// @brief lzma 圧縮/伸張を行なうファイルバッファ
///
/// FileBuff の継承クラスではないが，FileBuff と同様のインターフェイスを持つ．
//////////////////////////////////////////////////////////////////////
class XzFile :
  public LzmaStream
{
private:

  /// @brief 内部状態
  enum Mode {
    None,    ///< 無効
    Deflate, ///< 圧縮モード
    Inflate  ///< 伸張モード
  };


public:

  /// @brief コンストラクタ
  XzFile(
    SizeType buff_size = 4096,   ///< [in] バッファサイズ
    lzma_allocator* af = nullptr ///< [in] alloc 関数
  );

  /// @brief デストラクタ
  ~XzFile();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを圧縮用にオープンする．
  /// @retval true オープンが成功した．
  /// @retval false オープンが失敗した．
  bool
  deflate_open(
    const char* filename,                ///< [in] ファイル名
    mode_t mode = 0666,                  ///< [in] モード
    int preset = 6,                      ///< [in] 圧縮レベル ( 0 - 9: 6 がデフォルト )
    lzma_check check = LZMA_CHECK_CRC64  ///< [in] 検査方法
                                         ///<  - LZMA_CHECK_NONE
                                         ///<  - LZMA_CHECK_CRC32
                                         ///<  - LZMA_CHECK_CRC64
                                         ///<  - LZMA_CHECK_SHA256
  );

  /// @brief ファイルを圧縮用にオープンする．
  /// @retval true オープンが成功した．
  /// @retval false オープンが失敗した．
  bool
  deflate_open(
    const string& filename,              ///< [in] ファイル名
    mode_t mode = 0666,                  ///< [in] モード
    int preset = 6,                      ///< [in] 圧縮レベル ( 0 - 9: 6 がデフォルト )
    lzma_check check = LZMA_CHECK_CRC64  ///< [in] 検査方法
                                         ///<  - LZMA_CHECK_NONE
                                         ///<  - LZMA_CHECK_CRC32
                                         ///<  - LZMA_CHECK_CRC64
                                         ///<  - LZMA_CHECK_SHA256
  )
  {
    return deflate_open(filename.c_str(), mode, preset, check);
  }

  /// @brief ファイルを伸張用にオープンする．
  /// @retval true オープンが成功した．
  /// @retval false オープンが失敗した．
  bool
  inflate_open(
    const char* filename,                  ///< [in] ファイル名
    SizeType memlimit = 128 * 1024 * 1024, ///< [in] 割り当てるメモリの上限
    ymuint32 flags = 0                     ///< [in] 動作制御用のフラグ
  );

  /// @brief ファイルを伸張用にオープンする．
  /// @retval true オープンが成功した．
  /// @retval false オープンが失敗した．
  bool
  inflate_open(
    const string& filename,                ///< [in] ファイル名
    SizeType memlimit = 128 * 1024 * 1024, ///< [in] 割り当てるメモリの上限
    ymuint32 flags = 0                     ///< [in] 動作制御用のフラグ
  )
  {
    return inflate_open(filename.c_str(), memlimit, flags);
  }

  /// @brief ファイルをクローズする．
  void
  close();

  /// @brief buff[0] - buff[num - 1] の内容を書き込む．
  void
  write(
    const ymuint8* buff, ///< [in] データを格納したバッファ
    SizeType num         ///< [in] 書き込むバイト数
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

  // 内部状態
  Mode mMode{None};

  // ファイルバッファ
  FileBuff mFileBuff;

};

END_NAMESPACE_YM

#endif // XZFILE_H
