#ifndef YM_STRINGIDO_H
#define YM_STRINGIDO_H

/// @file ym/StringIDO.h
/// @brief StringIDO のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym/IDO.h"
#include "ym/FileInfo.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class StringIDO StringIDO.h "ym/StringIDO.h"
/// @ingroup ym
/// @brief 文字列を用いた IDO の継承クラス
//////////////////////////////////////////////////////////////////////
class StringIDO :
  public IDO
{
public:

  /// @brief コンストラクタ
  /// @param[in] str 文字列
  StringIDO(const char* str);

  /// @brief デストラクタ
  virtual
  ~StringIDO();


public:
  //////////////////////////////////////////////////////////////////////
  // 公開関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 1バイトの読み出し
  /// @return 読み込んだ値を返す．
  virtual
  ymuint8
  read_8();

  /// @brief 2バイトの読み出し
  /// @return 読み込んだ値を返す．
  virtual
  ymuint16
  read_16();

  /// @brief 4バイトの読み出し
  /// @return 読み込んだ値を返す．
  virtual
  ymuint32
  read_32();

  /// @brief 8バイトの読み出し
  /// @return 読み込んだ値を返す．
  virtual
  ymuint64
  read_64();

  /// @brief 単精度不動週数点数の読み出し
  /// @return 読み込んだ値を返す．
  virtual
  float
  read_float();

  /// @brief 倍精度不動週数点数の読み出し
  /// @return 読み込んだ値を返す．
  virtual
  double
  read_double();

  /// @brief 文字列の読み出し
  /// @return 読み込んだ値を返す．
  virtual
  string
  read_str();


public:
  //////////////////////////////////////////////////////////////////////
  // IDO の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 読み出し可能なら true を返す．
  virtual
  bool
  is_ready() const;

  /// @brief オープン中のファイル情報を得る．
  virtual
  const FileInfo&
  file_info() const;

  /// @brief 現在のファイル情報を書き換える．
  /// @param[in] file_info 新しいファイル情報
  /// @note プリプロセッサのプラグマなどで用いることを想定している．
  /// @note 通常は使わないこと．
  virtual
  void
  set_file_info(const FileInfo& file_info);

  /// @brief データを読み込む．
  /// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
  /// @param[in] n 読み込むデータサイズ
  /// @return 実際に読み込んだ量を返す．
  virtual
  ymint64
  read(ymuint8* buff,
       ymuint64 n);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル情報
  // このクラスでは意味を持たない．
  FileInfo mFileInfo;

  // 文字列
  string mString;

  // 読み出せるかどうかを示すフラグ
  bool mReady;

};

END_NAMESPACE_YM

#endif // YM_STRINGIDO_H
