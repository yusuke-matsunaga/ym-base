#ifndef YM_POPTMAINAPP_H
#define YM_POPTMAINAPP_H

/// @file ym/PoptMainApp.h
/// @brief PoptMainApp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @brief popt のオプション解析結果
//////////////////////////////////////////////////////////////////////
enum class PoptStat
{
  Ok,    ///< 通常の処理
  Error, ///< エラーだが処理を続行．
  Abort  ///< 処理を中断．
};


//////////////////////////////////////////////////////////////////////
/// @class Popt PoptMainApp.h "ym/PoptMainApp.h"
/// @brief popt の個々の要素を表すクラス
//////////////////////////////////////////////////////////////////////
class Popt
{
  friend class PoptMainApp;

public:

  /// @brief コンストラクタ
  ///
  /// opt_str が空文字列だったり opt_char が \0 だったりする場合もある．
  Popt(
    const char* opt_str, ///< [in] このオプションを表す文字列
    char opt_char,       ///< [in] このオプションを表す文字
    const char* opt_desc ///< [in] このオプションの説明文
  );

  /// @brief デストラクタ
  virtual
  ~Popt();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション文字列を返す．
  const char*
  opt_str() const;

  /// @brief オプション文字を返す．
  char
  opt_char() const;

  /// @brief argInfo の値を返す．
  virtual
  int
  arg_info() = 0;

  /// @brief arg の値を返す．
  virtual
  void*
  arg() = 0;

  /// @brief オプションの説明文を返す．
  const char*
  opt_desc() const;

  /// @brief オプションの引数の記述を返す．
  ///
  /// デフォルトの実装では nullptr を返す．
  virtual
  const char*
  arg_desc() const;

  /// @brief このオブジェクトが解析中に呼ばれていたら true を返す．
  bool
  is_specified() const;

  /// @brief このオブジェクトが呼ばれた回数を返す．
  int
  count() const;

  /// @brief オプションが指定されたときに呼び出される関数
  /// @retval true 正常に処理が行なわれた．
  /// @retval false 続行不可能なエラーが起こった．
  ///
  /// デフォルトではなにもしないで true を返す．
  virtual
  bool
  action();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オプション文字列
  const char* mOptStr;

  // オプションを表す文字
  char mOptChar;

  // オプションの説明文
  const char* mOptDesc;

  // このオプションが指定された回数
  int mCount;

};


//////////////////////////////////////////////////////////////////////
/// @class PoptNone PoptMainApp.h "ym/PoptMainApp.h"
/// @brief 引数をとらないオプションの処理を行なうオブジェクトの基底クラス
//////////////////////////////////////////////////////////////////////
class PoptNone :
  public Popt
{
public:

  /// @brief コンストラクタ
  PoptNone(
    const char* opt_str, ///< [in] オプション文字列
    char opt_char,       ///< [in] オプション文字
    const char* opt_desc ///< [in] オプションの説明文
  );

  /// @brief デストラクタ
  ~PoptNone();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief argInfo の値を返す．
  int
  arg_info() override;

  /// @brief arg の値を返す．
  void*
  arg() override;

};


//////////////////////////////////////////////////////////////////////
/// @class PoptArg PoptMainApp.h "ym/PoptMainApp.h"
/// @brief 引数をとるオプションの処理を行なうオブジェクトの基底クラス
//////////////////////////////////////////////////////////////////////
class PoptArg :
  public Popt
{
public:

  /// @brief コンストラクタ
  PoptArg(
    const char* opt_str,  ///< [in] オプション文字列
    char opt_char,        ///< [in] オプション文字
    const char* opt_desc, ///< [in] オプションの説明文
    const char* arg_desc  ///< [in] 引数の説明文
  );

  /// @brief デストラクタ
  ~PoptArg();


public:
  //////////////////////////////////////////////////////////////////////
  // Popt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプションの引数の記述を返す．
  const char*
  arg_desc() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 引数の説明文
  const char* mArgDesc;

};


//////////////////////////////////////////////////////////////////////
/// @class PoptStr PoptMainApp.h "ym/PoptMainApp.h"
/// @brief 文字列を引数にとるオプションの処理を行なうオブジェクト
//////////////////////////////////////////////////////////////////////
class PoptStr :
  public PoptArg
{
public:

  /// @brief コンストラクタ
  PoptStr(
    const char* opt_str,  ///< [in] オプション文字列
    char opt_char,	  ///< [in] オプション文字
    const char* opt_desc, ///< [in] オプションの説明文
    const char* arg_desc  ///< [in] 引数の説明文
  );

  /// @brief デストラクタ
  ~PoptStr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief arg_info の値を返す．
  int
  arg_info() override;

  /// @brief arg の値を返す．
  void*
  arg() override;


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスに特有の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション引数の値を返す．
  string
  val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オプション引数を格納する変数
  char* mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class PoptInt PoptMainApp.h "ym/PoptMainApp.h"
/// @brief 整数を引数にとるオプションを処理するクラス
//////////////////////////////////////////////////////////////////////
class PoptInt :
  public PoptArg
{
public:

  /// @brief コンストラクタ
  PoptInt(
    const char* opt_str,  ///< [in] オプション文字列
    char opt_char,	  ///< [in] オプション文字
    const char* opt_desc, ///< [in] オプションの説明文
    const char* arg_desc  ///< [in] 引数の説明文
  );

  /// @brief デストラクタ
  ~PoptInt();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief argInfo の値を返す．
  int
  arg_info() override;

  /// @brief arg の値を返す．
  void*
  arg() override;


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスに特有の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション引数の値を返す．
  int
  val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オプション引数を格納する変数
  int mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class PoptBool PoptMainApp.h "ym/PoptMainApp.h"
/// @brief ブール値を引数にとるオプションを処理するクラス
//////////////////////////////////////////////////////////////////////
class PoptBool :
  public PoptInt
{
public:

  /// @brief コンストラクタ
  PoptBool(
    const char* opt_str,  ///< [in] オプション文字列
    char opt_char,	  ///< [in] オプション文字
    const char* opt_desc, ///< [in] オプションの説明文
    const char* arg_desc  ///< [in] 引数の説明文
  );

  /// @brief デストラクタ
  ~PoptBool();


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスに特有の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション引数の値を返す．
  bool
  val() const;

};


//////////////////////////////////////////////////////////////////////
/// @class PoptUint PoptMainApp.h "ym/PoptMainApp.h"
/// @brief 符号なし整数を引数にとるオプションを処理するクラス
//////////////////////////////////////////////////////////////////////
class PoptUint :
  public PoptInt
{
public:

  /// @brief コンストラクタ
  PoptUint(
    const char* opt_str,  ///< [in] オプション文字列
    char opt_char,	  ///< [in] オプション文字
    const char* opt_desc, ///< [in] オプションの説明文
    const char* arg_desc  ///< [in] 引数の説明文
  );

  /// @brief デストラクタ
  ~PoptUint();


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスに特有の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション引数の値を返す．
  unsigned int
  val() const;

};


//////////////////////////////////////////////////////////////////////
/// @class PoptFloat PoptMainApp.h "ym/PoptMainApp.h"
/// @brief 単精度浮動小数点数を引数にとるオプションを処理するクラス
//////////////////////////////////////////////////////////////////////
class PoptFloat :
  public PoptArg
{
public:

  /// @brief コンストラクタ
  PoptFloat(
    const char* opt_str,  ///< [in] オプション文字列
    char opt_char,	  ///< [in] オプション文字
    const char* opt_desc, ///< [in] オプションの説明文
    const char* arg_desc  ///< [in] 引数の説明文
  );

  /// @brief デストラクタ
  ~PoptFloat();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief argInfo の値を返す．
  int
  arg_info() override;

  /// @brief arg の値を返す．
  void*
  arg() override;


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスに特有の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション引数の値を返す．
  float
  val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オプション引数を格納する変数
  float mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class PoptDouble PoptMainApp.h "ym/PoptMainApp.h"
/// @brief 倍精度浮動小数点数を引数にとるオプションを処理するクラス
//////////////////////////////////////////////////////////////////////
class PoptDouble :
  public PoptArg
{
public:

  /// @brief コンストラクタ
  PoptDouble(
    const char* opt_str,  ///< [in] オプション文字列
    char opt_char,	  ///< [in] オプション文字
    const char* opt_desc, ///< [in] オプションの説明文
    const char* arg_desc  ///< [in] 引数の説明文
  );

  /// @brief デストラクタ
  ~PoptDouble();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief argInfo の値を返す．
  int
  arg_info() override;

  /// @brief arg の値を返す．
  void*
  arg() override;


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスに特有の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション引数の値を返す．
  double
  val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オプション引数を格納する変数
  double mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class PoptMainApp PoptMainApp.h "ym/PoptMainApp.h"
/// @brief poptContext を用いる main 関数のクラス
//////////////////////////////////////////////////////////////////////
class PoptMainApp
{
public:

  /// @brief コンストラクタ
  PoptMainApp(
    const char* name = nullptr, ///< [in] 名前 (alias 用)
    bool auto_help = true       ///< [in] --help オプションを有効にするフラグ
  );

  /// @brief デストラクタ
  ~PoptMainApp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief オプションを追加する．
  void
  add_option(
    Popt* option ///< [in] 追加するオプション
  );

  /// @brief ヘルプ文字列を指定する．
  void
  set_other_option_help(
    const char* text ///< [in] ヘルプ文字列
  );

  /// @brief poptContext をリセットする．
  void
  reset();

  /// @brief オプション解析を行なう．
  PoptStat
  parse_options(
    int argc,          ///< [in] コマンド行の引数の数
    const char** argv, ///< [in] コマンド行の引数配列
    int flags          ///< [in] フラグ
  );

  /// @brief 残った引数を得る．
  /// @return 引数の数を返す．
  int
  get_args(
    vector<string>& args ///< [in] 引数を格納するベクタ
  );

  /// @brief ヘルプメッセージを出力する．
  ///
  /// <popt.h> が前世紀の遺物である FILE* を使っているので
  /// しかたなくこのようなインターフェイスになっている．
  void
  print_help(
    FILE* fp, ///< [in] FILE 構造体へのポインタ(古！)
    int flags ///< [in] フラグ(現時点では未使用)
  );

  /// @brief ユーセージ(ショートヘルプ)メッセージを出力する．
  ///
  /// <popt.h> が前世紀の遺物である FILE* を使っているので
  /// しかたなくこのようなインターフェイスになっている．
  void
  print_usage(
    FILE* fp, ///< [in] FILE 構造体へのポインタ(古！)
    int flags ///< [in] フラグ(現時点では未使用)
  );

  /// @brief usage を出力して終了する．
  void
  usage(
    int exitcode,                ///< [in] 終了コード
    const char* error = nullptr, ///< 不明
    const char* addl = nullptr   ///< 不明
  );

  /// @brief PoptMainApp 用の strerror() 関数
  static
  const char*
  popt_strerror(
    const int error ///< [in] エラーコード
  );

  /// @brief エラーが起きた場合にそのもととなったオプション文字列を返す．
  const char*
  bad_option(
    int flags ///< [in] フラグ
              ///<
              ///< フラグに使用可能な値は以下のとおり
	      ///<  - 0 なにもしない．
	      ///<  - POPT_BADOPTION_NOALIAS もっとも外側(outermost)のオプションを返す．
  );

  /// @brief alias 用のデフォルト設定を読み込む．
  /// @return 返り値に関しては man popt に記載なし．
  int
  read_default_config(
    int flags ///< [in] フラグ(現時点では未使用)
  );

  /// @brief alias 用の設定ファイルを読み込む．
  int
  read_config_file(
    const char* filename ///< [in] ファイル名
  );

  /// @brief alias を追加する．
  int
  add_alias(
    const char* long_name, ///< [in] 長い名前 (--xxxx)
    char short_name,       ///< [in] 短い名前 (-x)
    int argc,              ///< [in] 本体の引数の数
    const char** argv,     ///< [in] 本体の文字列の配列()
    int flags              ///< [in] フラグ(現時点では未使用)
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // エイリアスで用いる(らしい)名前
  const char* mName;

  // AUTOHELP オプションを有効にするとき true にするフラグ
  bool mAutoHelp;

  // 処理を行なったときに true となるフラグ
  // popt の処理は1度しか行なえない．
  bool mDone;

  // poptContext 本体
  // あえて opaque オブジェクトにしている．
  void* mCon;

  // ヘルプ文字列
  const char* mHelpText;

  // オプションオブジェクトのリスト
  vector<Popt*> mOptionList;

};

END_NAMESPACE_YM

#endif // UTILS_POPTMAINAPP_H
