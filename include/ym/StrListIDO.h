#ifndef YM_STRLISTIDO_H
#define YM_STRLISTIDO_H

/// @file ym/StrListIDO.h
/// @brief StrListIDO のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym/IDO.h"
#include "ym/FileInfo.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class StrListIDO StrListIDO.h "ym/StrListIDO.h"
/// @ingroup ym
/// @brief 文字列のリストを用いた IDO の継承クラス
//////////////////////////////////////////////////////////////////////
class StrListIDO :
  public IDO
{
public:

  /// @brief コンストラクタ
  /// @param[in] str_list 文字列のリスト
  StrListIDO(const vector<string>& str_list);

  /// @brief デストラクタ
  virtual
  ~StrListIDO();


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
  int
  read(ymuint8* buff,
       int n);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル情報
  // このクラスでは意味を持たない．
  FileInfo mFileInfo;

  // 文字列のリスト
  vector<string> mStrList;

  // 現在処理中の行番号
  int mLineNum;

  // 次に読み出す位置
  int mColumnNum;

};

END_NAMESPACE_YM

#endif // YM_STRLISTIDO_H
