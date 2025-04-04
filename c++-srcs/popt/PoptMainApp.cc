﻿
/// @file PoptMainApp.cc
/// @brief PoptMainApp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/PoptMainApp.h"

#include <popt.h>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス PoptMainApp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PoptMainApp::PoptMainApp(
  const char* name,
  bool auto_help
)
{
  mName = name;
  mAutoHelp = auto_help;
  mDone = false;
  mCon = 0;
  mHelpText = nullptr;
}

// @brief デストラクタ
PoptMainApp::~PoptMainApp()
{
  poptFreeContext(reinterpret_cast<poptContext>(mCon));
}

// @brief オプションを追加する．
void
PoptMainApp::add_option(
  Popt* option
)
{
  mOptionList.push_back(option);
}

// @brief ヘルプ文字列を指定する．
void
PoptMainApp::set_other_option_help(
  const char* text
)
{
  mHelpText = text;
}

// @brief リセットする．
void
PoptMainApp::reset()
{
  poptResetContext(reinterpret_cast<poptContext>(mCon));
}

// @brief オプション解析を行なう．
PoptStat
PoptMainApp::parse_options(
  int argc,
  const char** argv,
  int flags
)
{
  if ( mDone ) {
    cerr << "PoptMainApp::parse_options() is called more than once." << endl;
    return PoptStat::Abort;
  }

  // オプションテーブルを作る．
  int n = mOptionList.size();
  // end-marker の分を1つ足す必要がある．
  int table_size = n + 1;
  if ( mAutoHelp ) {
    ++ table_size;
  }
  poptOption* option_table = new poptOption[table_size];
  for ( int i = 0; i < n; ++ i ) {
    poptOption& option = option_table[i];
    Popt* opt = mOptionList[i];
    option.longName = opt->opt_str();
    option.shortName = opt->opt_char();
    option.argInfo = opt->arg_info();
    option.arg = opt->arg();
    option.val = i + 1;
    option.descrip = opt->opt_desc();
    option.argDescrip = opt->arg_desc();
  }
  if ( mAutoHelp ) {
    poptOption& option = option_table[n];
    option.longName = nullptr;
    option.shortName = '\0';
    option.argInfo = POPT_ARG_INCLUDE_TABLE;
    option.arg = poptHelpOptions;
    option.val = 0;
    option.descrip = "Help options:";
    option.argDescrip = nullptr;
    ++ n;
  }
  { // end-markder
    poptOption& option = option_table[n];
    option.longName = nullptr;
    option.shortName = '\0';
    option.argInfo = 0;
    option.arg = nullptr;
    option.val = 0;
    option.descrip = nullptr;
    option.argDescrip = nullptr;
  }

  // poptContext を作る．
  mCon = poptGetContext(mName, argc, argv, option_table, flags);

  if ( mHelpText != nullptr ) {
    poptSetOtherOptionHelp(reinterpret_cast<poptContext>(mCon), mHelpText);
  }

  // オプション解析行う．
  for ( ; ; ) {
    int rc = poptGetNextOpt(reinterpret_cast<poptContext>(mCon));
    if ( rc == -1 ) {
      break;
    }
    if ( rc < -1 ) {
      // エラーが起きた．
      cerr << poptBadOption(reinterpret_cast<poptContext>(mCon), POPT_BADOPTION_NOALIAS)
	   << ": " << poptStrerror(rc) << endl;
      return PoptStat::Abort;
    }
    int id = rc;

    ASSERT_COND( id > 0 );

    -- id;

    ASSERT_COND( id < mOptionList.size() );

    Popt* popt = mOptionList[id];
    ++ popt->mCount;
    // 該当するオプションの処理を行なう．
    bool stat = popt->action();
    if ( !stat ) {
      // 続行不可能なエラーが起きた．
      delete [] option_table;
      return PoptStat::Abort;
    }
  }

  delete [] option_table;
  return PoptStat::Ok;
}

// @brief 残った引数を得る．
int
PoptMainApp::get_args(
  vector<string>& args
)
{
  args.clear();
  for ( ; ; ) {
    const char* arg = poptGetArg(reinterpret_cast<poptContext>(mCon));
    if ( arg == nullptr ) {
      break;
    }
    args.push_back(string(arg));
  }
  return args.size();
}

// @brief ヘルプメッセージを出力する．
void
PoptMainApp::print_help(
  FILE* fp,
  int flags
)
{
  poptPrintHelp(reinterpret_cast<poptContext>(mCon), fp, flags);
}

// @brief ユーセージ(ショートヘルプ)メッセージを出力する．
void
PoptMainApp::print_usage(
  FILE* fp,
  int flags
)
{
  poptPrintUsage(reinterpret_cast<poptContext>(mCon), fp, flags);
}

// @brief usage を出力して終了する．
void
PoptMainApp::usage(
  int exitcode,
  const char* error,
  const char* addl
)
{
  print_usage(stderr, 0);
  if ( error ) {
    fprintf(stderr, "%s: %s\n", error, addl);
  }
  exit(exitcode);
}

// @brief PoptMainApp 用の strerror() 関数
const char*
PoptMainApp::popt_strerror(
  const int error
)
{
  return poptStrerror(error);
}

// @brief エラーが起きた場合にそのもととなったオプション文字列を返す．
const char*
PoptMainApp::bad_option(
  int flags
)
{
  return poptBadOption(reinterpret_cast<poptContext>(mCon), flags);
}

// @brief alias 用のデフォルト設定を読み込む．
int
PoptMainApp::read_default_config(
  int flags
)
{
  return poptReadDefaultConfig(reinterpret_cast<poptContext>(mCon), flags);
}

// @brief alias 用の設定ファイルを読み込む．
int
PoptMainApp::read_config_file(
  const char* filename
)
{
  return poptReadConfigFile(reinterpret_cast<poptContext>(mCon), filename);
}

// @brief alias を追加する．
int
PoptMainApp::add_alias(
  const char* long_name,
  char short_name,
  int argc,
  const char** argv,
  int flags
)
{
  struct poptAlias alias;
  alias.longName = long_name;
  alias.shortName = short_name;
  alias.argc = argc;
  alias.argv = argv;
  return poptAddAlias(reinterpret_cast<poptContext>(mCon), alias, flags);
}


//////////////////////////////////////////////////////////////////////
// クラス Popt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Popt::Popt(
  const char* opt_str,
  char opt_char,
  const char* opt_desc
) : mOptStr(opt_str),
    mOptChar(opt_char),
    mOptDesc(opt_desc),
    mCount(0)
{
}

// @brief デストラクタ
Popt::~Popt()
{
}

// @brief オプション文字列を返す．
const char*
Popt::opt_str() const
{
  return mOptStr;
}

// @brief オプション文字を返す．
char
Popt::opt_char() const
{
  return mOptChar;
}

// @brief オプションの説明文を返す．
const char*
Popt::opt_desc() const
{
  return mOptDesc;
}

// @brief オプションの引数の記述を返す．
const char*
Popt::arg_desc() const
{
  return nullptr;
}

// @brief このオブジェクトが解析中に呼ばれていたら true を返す．
bool
Popt::is_specified() const
{
  return count() > 0;
}

// @brief このオブジェクトが呼ばれた回数を返す．
int
Popt::count() const
{
  return mCount;
}

// @brief オプションが指定されたときに呼び出される関数
bool
Popt::action()
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス PoptNone
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PoptNone::PoptNone(
  const char* opt_str,
  char opt_char,
  const char* opt_desc
) : Popt{opt_str, opt_char, opt_desc}
{
}

// @brief デストラクタ
PoptNone::~PoptNone()
{
}

// @brief argInfo の値を返す．
int
PoptNone::arg_info()
{
  return POPT_ARG_NONE;
}

// @brief arg の値を返す．
void*
PoptNone::arg()
{
  return nullptr;
}



//////////////////////////////////////////////////////////////////////
// クラス PoptArg
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PoptArg::PoptArg(
  const char* opt_str,
  char opt_char,
  const char* opt_desc,
  const char* arg_desc
) : Popt{opt_str, opt_char, opt_desc},
    mArgDesc{arg_desc}
{
}

// @brief デストラクタ
PoptArg::~PoptArg()
{
}

// @brief オプションの引数の記述を返す．
const char*
PoptArg::arg_desc() const
{
  return mArgDesc;
}


//////////////////////////////////////////////////////////////////////
// クラス PoptStr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PoptStr::PoptStr(
  const char* opt_str,
  char opt_char,
  const char* opt_desc,
  const char* arg_desc
) : PoptArg{opt_str, opt_char, opt_desc, arg_desc}
{
}

// @brief デストラクタ
PoptStr::~PoptStr()
{
}

// @brief argInfo の値を返す．
int
PoptStr::arg_info()
{
  return POPT_ARG_STRING;
}

// @brief arg の値を返す．
void*
PoptStr::arg()
{
  return &mVal;
}

// @brief オプション引数の値を返す．
string
PoptStr::val() const
{
  return mVal;
}


//////////////////////////////////////////////////////////////////////
// クラス PoptInt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PoptInt::PoptInt(
  const char* opt_str,
  char opt_char,
  const char* opt_desc,
  const char* arg_desc
) : PoptArg{opt_str, opt_char, opt_desc, arg_desc}
{
}

// @brief デストラクタ
PoptInt::~PoptInt()
{
}

// @brief argInfo の値を返す．
int
PoptInt::arg_info()
{
  return POPT_ARG_INT;
}

// @brief arg の値を返す．
void*
PoptInt::arg()
{
  return &mVal;
}

// @brief オプション引数の値を返す．
int
PoptInt::val() const
{
  return mVal;
}


//////////////////////////////////////////////////////////////////////
// クラス PoptBool
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PoptBool::PoptBool(
  const char* opt_str,
  char opt_char,
  const char* opt_desc,
  const char* arg_desc
) : PoptInt{opt_str, opt_char, opt_desc, arg_desc}
{
}

// @brief デストラクタ
PoptBool::~PoptBool()
{
}

// @brief オプション引数の値を返す．
bool
PoptBool::val() const
{
  return static_cast<bool>(PoptInt::val());
}


//////////////////////////////////////////////////////////////////////
// クラス PoptUint
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PoptUint::PoptUint(
  const char* opt_str,
  char opt_char,
  const char* opt_desc,
  const char* arg_desc
) : PoptInt{opt_str, opt_char, opt_desc, arg_desc}
{
}

// @brief デストラクタ
PoptUint::~PoptUint()
{
}

// @brief オプション引数の値を返す．
unsigned int
PoptUint::val() const
{
  return static_cast<unsigned int>(PoptInt::val());
}


//////////////////////////////////////////////////////////////////////
// クラス PoptFloat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PoptFloat::PoptFloat(
  const char* opt_str,
  char opt_char,
  const char* opt_desc,
  const char* arg_desc
) : PoptArg{opt_str, opt_char, opt_desc, arg_desc}
{
}

// @brief デストラクタ
PoptFloat::~PoptFloat()
{
}

// @brief argInfo の値を返す．
int
PoptFloat::arg_info()
{
  return POPT_ARG_FLOAT;
}

// @brief arg の値を返す．
void*
PoptFloat::arg()
{
  return &mVal;
}

// @brief オプション引数の値を返す．
float
PoptFloat::val() const
{
  return mVal;
}


//////////////////////////////////////////////////////////////////////
// クラス PoptDouble
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PoptDouble::PoptDouble(
  const char* opt_str,
  char opt_char,
  const char* opt_desc,
  const char* arg_desc
) : PoptArg{opt_str, opt_char, opt_desc, arg_desc}
{
}

// @brief デストラクタ
PoptDouble::~PoptDouble()
{
}

// @brief argInfo の値を返す．
int
PoptDouble::arg_info()
{
  return POPT_ARG_DOUBLE;
}

// @brief arg の値を返す．
void*
PoptDouble::arg()
{
  return &mVal;
}

// @brief オプション引数の値を返す．
double
PoptDouble::val() const
{
  return mVal;
}

END_NAMESPACE_YM
