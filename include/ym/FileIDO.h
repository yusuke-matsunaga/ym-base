#ifndef YM_FILEIDO_H
#define YM_FILEIDO_H

/// @file ym/FileIDO.h
/// @brief FileIDO のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013-2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/IDO.h"
#include "ym/FileLoc.h"
#include "ym/FileInfo.h"
#include "ym/CodecType.h"


BEGIN_NAMESPACE_YM

class FileDecoder;

//////////////////////////////////////////////////////////////////////
/// @class FileIDO FileIDO.h "ym/FileIDO.h"
/// @ingroup ym
/// @brief 通常のファイルを用いた IDO の継承クラス
//////////////////////////////////////////////////////////////////////
class FileIDO :
  public IDO
{
public:

  /// @brief コンストラクタ
  explicit
  FileIDO(CodecType codec_type = CodecType::Through); ///< [in] Decoder の種類

  /// @brief デストラクタ
  ~FileIDO();


public:
  //////////////////////////////////////////////////////////////////////
  // IDO の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 読み出し可能なら true を返す．
  bool
  is_ready() const override;

  /// @brief オープン中のファイル情報を得る．
  const FileInfo&
  file_info() const override;

  /// @brief 現在のファイル情報を書き換える．
  ///
  /// プリプロセッサのプラグマなどで用いることを想定している．
  /// 通常は使わないこと．
  void
  set_file_info(const FileInfo& file_info) override; ///< [in] 新しいファイル情報

  /// @brief データを読み込む．
  /// @return 実際に読み込んだ量を返す．
  ///
  /// エラーが起こったら負の数を返す．
  virtual
  int
  read(ymuint8* buff, ///< [in] 読み込んだデータを格納する領域の先頭アドレス．
       int n);        ///< [in] 読み込むデータサイズ


public:
  //////////////////////////////////////////////////////////////////////
  // FileIDO の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを開く
  ///
  /// 他のファイルを開いていたら強制的に close する．
  bool
  open(const char* filename,                   ///< [in] ファイル名
       const FileLoc& parent_loc = FileLoc()); ///< [in] インクルード元の親ファイルの情報

  /// @brief ファイルを開く
  ///
  /// 他のファイルを開いていたら強制的に close する．
  bool
  open(const string& filename,                 ///< [in] ファイル名
       const FileLoc& parent_loc = FileLoc()); ///< [in] インクルード元の親ファイルの情報

  /// @brief ファイルを閉じる．
  ///
  /// 以降の読み出しは行われない．
  void
  close();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル情報
  FileInfo mFileInfo;

  // ファイル復号器
  FileDecoder* mDecoder;

};

END_NAMESPACE_YM

#endif // YM_FILEIDO_H
