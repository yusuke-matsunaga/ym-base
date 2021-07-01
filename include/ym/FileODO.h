#ifndef YM_FILEODO_H
#define YM_FILEODO_H

/// @file ym/FileODO.h
/// @brief FileODO のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013-2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ODO.h"
#include "ym/CodecType.h"


#if defined(YM_WIN32)
typedef int mode_t;
#endif

BEGIN_NAMESPACE_YM

class FileCoder;

//////////////////////////////////////////////////////////////////////
/// @class FileODO FileODO.h "ym/FileODO.h"
/// @ingroup ym
/// @brief 通常のファイルを用いた ODO の継承クラス
//////////////////////////////////////////////////////////////////////
class FileODO :
  public ODO
{
public:

  /// @brief コンストラクタ
  explicit
  FileODO(CodecType codec_type = CodecType::Through); ///< [in] Coder の種類

  /// @brief デストラクタ
  virtual
  ~FileODO();


public:
  //////////////////////////////////////////////////////////////////////
  // FileODO の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 書き込み可能なら true を返す．
  operator bool() const;

  /// @brief ファイルを開く
  bool
  open(const char* filename, ///< [in] ファイル名
       mode_t mode = 0666,   ///< [in] ファイル作成用のモード
       int opt = 0);         ///< [in] 圧縮用のオプション

  /// @brief ファイルを開く
  bool
  open(const string& filename, ///< [in] ファイル名
       mode_t mode = 0666,     ///< [in] ファイル作成用のモード
       int opt = 0);           ///< [in] 圧縮用のオプション

  /// @brief ファイルを閉じる．
  ///
  /// 以降の書き込みは行われない．
  void
  close();


public:
  //////////////////////////////////////////////////////////////////////
  // ODO の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを書き出す．
  /// @return 実際に書き出した量を返す．
  virtual
  int
  write(const ymuint8* buff, ///< [in] データを収めた領域のアドレス
	int n);              ///< [in] データサイズ


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル符号器
  FileCoder* mCoder;

};

END_NAMESPACE_YM

#endif // YM_FILEODO_H
