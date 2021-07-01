#ifndef YM_IDO_H
#define YM_IDO_H

/// @file ym/IDO.h
/// @brief IDO のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013-2014, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"

#error "deprecated"

BEGIN_NAMESPACE_YM

class FileInfo;

//////////////////////////////////////////////////////////////////////
/// @class IDO IDO.h "ym/IDO.h"
/// @ingroup ym
/// @brief 入力データオブジェクト (Input Data Object)
//////////////////////////////////////////////////////////////////////
class IDO
{
public:

  /// @brief コンストラクタ
  IDO();

  /// @brief デストラクタ
  virtual
  ~IDO();


public:
  //////////////////////////////////////////////////////////////////////
  // 公開関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 1バイトの読み出し
  /// @return 読み込んだ値を返す．
  ///
  /// ODO の write_8() に対応する．
  virtual
  ymuint8
  read_8();

  /// @brief 2バイトの読み出し
  /// @return 読み込んだ値を返す．
  ///
  /// ODO の write_16() に対応する．
  virtual
  ymuint16
  read_16();

  /// @brief 4バイトの読み出し
  /// @return 読み込んだ値を返す．
  ///
  /// ODO の write_32() に対応する．
  virtual
  ymuint32
  read_32();

  /// @brief 8バイトの読み出し
  /// @return 読み込んだ値を返す．
  ///
  /// ODO の write_64() に対応する．
  virtual
  ymuint64
  read_64();

  /// @brief 単精度不動週数点数の読み出し
  /// @return 読み込んだ値を返す．
  ///
  /// ODO の write_float() に対応する．
  virtual
  float
  read_float();

  /// @brief 倍精度不動週数点数の読み出し
  /// @return 読み込んだ値を返す．
  ///
  /// ODO の write_double() に対応する．
  virtual
  double
  read_double();

  /// @brief 文字列の読み出し
  /// @return 読み込んだ値を返す．
  ///
  /// ODO の write_str() に対応する．
  virtual
  string
  read_str();

  /// @brief 一行の読み出し
  /// @param[out] str 読みだした文字列を格納する変数
  /// @return 読み出しが成功したら true を返す．
  ///
  /// 空行の場合には str に string() を入れて true を返す．
  /// データがなければ false を返す．
  virtual
  bool
  read_line(string& str);


public:
  //////////////////////////////////////////////////////////////////////
  // IDO の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 読み出し可能なら true を返す．
  virtual
  bool
  is_ready() const = 0;

  /// @brief オープン中のファイル情報を得る．
  virtual
  const FileInfo&
  file_info() const = 0;

  /// @brief 現在のファイル情報を書き換える．
  /// @param[in] file_info 新しいファイル情報
  /// @note プリプロセッサのプラグマなどで用いることを想定している．
  /// @note 通常は使わないこと．
  virtual
  void
  set_file_info(const FileInfo& file_info) = 0;

  /// @brief データを読み込む．
  /// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
  /// @param[in] n 読み込むデータサイズ
  /// @return 実際に読み込んだ量を返す．
  virtual
  int
  read(ymuint8* buff,
       int n) = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief read() を呼び出して結果をチェックする．
  /// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
  /// @param[in] n 読み込むデータサイズ
  ///
  /// 読みだしたサイズが n と異なっていたらエラーメッセージを出力する．
  void
  _read(ymuint8* buff,
	int n);

};


//////////////////////////////////////////////////////////////////////
// IDO に対するストリーム入力演算子
//////////////////////////////////////////////////////////////////////

/// @brief ブール値の読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return IDO を返す．
IDO&
operator>>(IDO& s,
	   bool& val);

/// @brief 1バイトの読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return IDO を返す．
IDO&
operator>>(IDO& s,
	   ymint8& val);

/// @brief 2バイトの読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return IDO を返す．
IDO&
operator>>(IDO& s,
	   ymint16& val);

/// @brief 4バイトの読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return IDO を返す．
IDO&
operator>>(IDO& s,
	   ymint32& val);

/// @brief 8バイトの読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return IDO を返す．
IDO&
operator>>(IDO& s,
	   ymint64& val);

/// @brief 1バイトの読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return IDO を返す．
IDO&
operator>>(IDO& s,
	   ymuint8& val);

/// @brief 2バイトの読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return IDO を返す．
IDO&
operator>>(IDO& s,
	   ymuint16& val);

/// @brief 4バイトの読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return IDO を返す．
IDO&
operator>>(IDO& s,
	   ymuint32& val);

/// @brief 8バイトの読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return IDO を返す．
IDO&
operator>>(IDO& s,
	   ymuint64& val);

/// @brief 単精度不動週数点数の読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return IDO を返す．
IDO&
operator>>(IDO& s,
	   float& val);

/// @brief 倍精度不動週数点数の読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return IDO を返す．
IDO&
operator>>(IDO& s,
	   double& val);

/// @brief 文字列の読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return IDO を返す．
IDO&
operator>>(IDO& s,
	   string& val);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ブール値の読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return IDO を返す．
inline
IDO&
operator>>(IDO& s,
	   bool& val)
{
  val = static_cast<bool>(s.read_8() != 0);
  return s;
}

// @brief 1バイトの読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return IDO を返す．
inline
IDO&
operator>>(IDO& s,
	   ymint8& val)
{
  val = static_cast<ymint8>(s.read_8());
  return s;
}

// @brief 2バイトの読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return IDO を返す．
inline
IDO&
operator>>(IDO& s,
	   ymint16& val)
{
  val = static_cast<ymint16>(s.read_16());
  return s;
}

// @brief 4バイトの読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return IDO を返す．
inline
IDO&
operator>>(IDO& s,
	   ymint32& val)
{
  val = static_cast<ymint32>(s.read_32());
  return s;
}

// @brief 8バイトの読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return IDO を返す．
inline
IDO&
operator>>(IDO& s,
	   ymint64& val)
{
  val = static_cast<ymint64>(s.read_64());
  return s;
}

// @brief 1バイトの読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return IDO を返す．
inline
IDO&
operator>>(IDO& s,
	   ymuint8& val)
{
  val = s.read_8();
  return s;
}

// @brief 2バイトの読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return IDO を返す．
inline
IDO&
operator>>(IDO& s,
	   ymuint16& val)
{
  val = s.read_16();
  return s;
}

// @brief 4バイトの読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return IDO を返す．
inline
IDO&
operator>>(IDO& s,
	   ymuint32& val)
{
  val = s.read_32();
  return s;
}

// @brief 8バイトの読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return IDO を返す．
inline
IDO&
operator>>(IDO& s,
	   ymuint64& val)
{
  val = s.read_64();
  return s;
}

// @brief 単精度不動週数点数の読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return IDO を返す．
inline
IDO&
operator>>(IDO& s,
	   float& val)
{
  val = s.read_float();
  return s;
}

// @brief 倍精度不動週数点数の読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return IDO を返す．
inline
IDO&
operator>>(IDO& s,
	   double& val)
{
  val = s.read_double();
  return s;
}

// @brief 文字列の読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return IDO を返す．
inline
IDO&
operator>>(IDO& s,
	   string& val)
{
  val = s.read_str();
  return s;
}

END_NAMESPACE_YM

#endif // YM_IDO_H
