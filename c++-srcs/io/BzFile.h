#ifndef BZFILE_H
#define BZFILE_H

/// @file BzFile.h
/// @brief BzFile のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "BzStream.h"
#include "FileBuff.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class BzFile BzFile.h "BzFile.h"
/// @brief bzip2 形式の圧縮/伸張を行なうファイルバッファ
//////////////////////////////////////////////////////////////////////
class BzFile :
  public BzStream
{
public:

  /// @brief コンストラクタ
  BzFile(
    SizeType buff_size = 4096, ///< [in] バッファサイズ
    alloc_func af = nullptr,   ///< [in] alloc 関数
    free_func ff = nullptr,    ///< [in] free 関数
    void* op = nullptr         ///< [in] opaque オブジェクト
  );

  /// @brief デストラクタ
  ~BzFile();


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
    const char* filename,    ///< [in] ファイル名
    mode_t mode = 0666,      ///< [in] ファイル作成用のモード
    int block_size_100k = 9, ///< [in] 作業用のメモリサイズ ( 0 - 9 )
    int verbosity = 0,       ///< [in] デバッグ用の出力レベル ( 0 - 4 )
    int work_factor = 0      ///< [in] 圧縮アルゴリズム用のパラメータ ( 0 - 250 )
  );

  /// @brief ファイルを圧縮用に開く
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルに対する書き込み許可がない．
  bool
  deflate_open(
    const string& filename,  ///< [in] ファイル名
    mode_t mode = 0666,      ///< [in] ファイル作成用のモード
    int block_size_100k = 9, ///< [in] 作業用のメモリサイズ ( 0 - 9 )
    int verbosity = 0,       ///< [in] デバッグ用の出力レベル ( 0 - 4 )
    int work_factor = 0      ///< [in] 圧縮アルゴリズム用のパラメータ ( 0 - 250 )
  )
  {
    return deflate_open(filename.c_str(), mode,
			block_size_100k, verbosity, work_factor);
  }

  /// @brief ファイルを伸張用に開く
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルが存在しない．
  ///  - ファイルに対する読み出し許可がない．
  ///  - ファイルの形式が異なる．
  bool
  inflate_open(
    const char* filename, ///< [in] ファイル名
    int verbosity = 0,    ///< [in] デバッグ用の出力レベル ( 0 - 4 )
    int small = 0         ///< [in] 伸張アルゴリズム用のパラメータ ( 0 か 非0 )
  );

  /// @brief ファイルを伸張用に開く
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルが存在しない．
  ///  - ファイルに対する読み出し許可がない．
  ///  - ファイルの形式が異なる．
  bool
  inflate_open(
    const string& filename, ///< [in] ファイル名
    int verbosity = 0,      ///< [in] デバッグ用の出力レベル ( 0 - 4 )
    int small = 0           ///< [in] 伸張アルゴリズム用のパラメータ ( 0 か 非0 )
  )
  {
    return inflate_open(filename.c_str(), verbosity, small);
  }

  /// @brief ファイルを閉じる．
  void
  close();

  /// @brief 最大 num バイトのデータを圧縮してファイルに書き込む．
  ///
  /// エラーが起こったら -1 を返す．
  void
  write(
    const ymuint8* wbuff, ///< [in] 圧縮するデータを格納するバッファ
    SizeType num          ///< [in] 書き込むデータ数(バイト)
  );

  /// @brief 圧縮されたファイルを読んで最大 num バイトをバッファに格納する．
  /// @return 実際に読み出したバイト数を返す．
  ///
  /// エラーが起こったら -1 を返す．
  SizeType
  read(
    ymuint8* rbuff, ///< [in] 展開したデータを格納するバッファ
    SizeType num    ///< [in] 読み出すデータ数(バイト)
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイルバッファ
  FileBuff mFileBuff;

};

END_NAMESPACE_YM

#endif // BZFILE_H
