
/// @file JsonScanner.cc
/// @brief JsonScanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "JsonScanner.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_JSON

// @brief コンストラクタ
JsonScanner::JsonScanner(
  istream& s,
  const FileInfo& file_info
) : Scanner{s, file_info}
{
}

// @brief トークンを一つ読み出す．
JsonToken
JsonScanner::read_token()
{
  if ( mUngetToken != JsonToken::None ) {
    auto tk = mUngetToken;
    mUngetToken = JsonToken::None;
    return tk;
  }
  auto tk = scan();
  mCurLoc = cur_region();
  return tk;
}

// @brief 直前に読み込んだトークンを戻す．
void
JsonScanner::unget_token(
  JsonToken tk
)
{
  ASSERT_COND( mUngetToken == JsonToken::None );
  mUngetToken = tk;
}

// @brief read_token() のサブルーチン
JsonToken
JsonScanner::scan()
{
  mCurString = "";

  int c;

 ST_INIT:
  c = get();
  set_first_loc();

  switch ( c ) {
  case EOF:
    return JsonToken::End;

  case ' ':
  case '\t':
  case '\n':
    // 先頭のホワイトスペースは読み飛ばす．
    goto ST_INIT;

  case '{':
    return JsonToken::LCB;

  case '}':
    return JsonToken::RCB;

  case '[':
    return JsonToken::LBK;

  case ']':
    return JsonToken::RBK;

  case ',':
    return JsonToken::Comma;

  case ':':
    return JsonToken::Colon;

  case '\"':
    goto ST_DQ;

  case '-':
    mCurString.put_char(c);
    goto ST_NUM_MINUS;

  case '0':
    mCurString.put_char(c);
    goto ST_NUM0;

  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    mCurString.put_char(c);
    goto ST_NUM1;

  case '.':
    mCurString.put_char(c);
    goto ST_NUM_DOT;

  case 't':
    if ( read_true() ) {
      return JsonToken::True;
    }
    goto ST_ERROR;

  case 'f':
    if ( read_false() ) {
      return JsonToken::False;
    }
    goto ST_ERROR;

  case 'n':
    if ( read_null() ) {
      return JsonToken::Null;
    }
    goto ST_ERROR;

  case '#':
    goto ST_CM2;

  case '/':
    c = get();
    if ( c == '*' ) {
      // "/*" を読み込んだ
      goto ST_CM1;
    }
    if ( c == '/' ) {
      goto ST_CM2;
    }
    goto ST_ERROR;

  default:
    goto ST_ERROR;
  }

 ST_NUM_MINUS: // '-' を読んだ直後
  c = get();
  switch ( c ) {
  case '0':
    mCurString.put_char(c);
    goto ST_NUM0;

  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    mCurString.put_char(c);
    goto ST_NUM1;

  default:
    goto ST_ERROR;
  }

 ST_NUM0: // '0' を読んだ直後
  c = peek();
  switch ( c ) {
  case '.':
    accept();
    mCurString.put_char(c);
    goto ST_NUM_DOT;

  case 'e':
  case 'E':
    accept();
    mCurString.put_char(c);
    goto ST_NUM_EXP1;

  default:
    return JsonToken::Int;
  }

 ST_NUM1: // '1'〜'9' を読んだ直後
  c = peek();
  switch ( c ) {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    accept();
    mCurString.put_char(c);
    goto ST_NUM1;

  case '.':
    accept();
    mCurString.put_char(c);
    goto ST_NUM_DOT;

  case 'e':
  case 'E':
    accept();
    mCurString.put_char(c);
    goto ST_NUM_EXP1;

  default:
    return JsonToken::Int;
  }

 ST_NUM_DOT: // '.' を読んだ直後
  c = peek();
  switch ( c ) {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    accept();
    mCurString.put_char(c);
    goto ST_NUM_DOT;

  case 'e':
  case 'E':
    accept();
    mCurString.put_char(c);
    goto ST_NUM_EXP1;

  default:
    return JsonToken::Float;
  }

 ST_NUM_EXP1:
  c = peek();
  switch ( c ) {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    accept();
    mCurString.put_char(c);
    goto ST_NUM_DOT;

  case '-':
  case '+':
    accept();
    mCurString.put_char(c);
    goto ST_NUM_EXP2;
  }

 ST_NUM_EXP2:
  c = peek();
  switch ( c ) {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    accept();
    mCurString.put_char(c);
    goto ST_NUM_EXP2;

  default:
    return JsonToken::Float;
  }

 ST_DQ: // 次の '"' までを文字列だと思う．
  c = get();
  if ( c == '\"' ) {
    return JsonToken::String;
  }
  if ( c == '\\' ) {
    c = get();
    switch ( c ) {
    case '\"': c = '\"'; break;
    case '\\': c = '\\'; break;
    case '/': c = '/'; break;
    case 'b': c = '\b'; break;
    case 'f': c = '\f'; break;
    case 'n': c = '\n'; break;
    case 'r': c = '\r'; break;
    case 't': c = '\t'; break;
    case 'u': goto ST_UHEX4; break;
    default: goto ST_ERROR;
    }
  }
  else if ( !isprint(c) ) {
    goto ST_ERROR;
  }
  mCurString.put_char(c);
  goto ST_DQ;

 ST_UHEX4: // 4桁のHEXコードを読み込み，unicode と解釈する．
  {
    char buff[5];
    for ( SizeType i = 0; i < 4; ++ i ) {
      int c = get();
      if ( !isxdigit(c) ) {
	goto ST_ERROR;
      }
      buff[i] = c;
    }
    buff[4] = '\0';
    std::uint32_t code = strtol(buff, nullptr, 16);
    // code を unicode とみなして UTF-8 に符号化する．
    if ( code <= 0x007F ) {
      char c = static_cast<char>(code);
      mCurString.put_char(c);
    }
    else if ( code <= 0x07FF ) {
      char l = static_cast<char>(code & 0x3F) | 0x80;
      char h = static_cast<char>((code >> 6) & 0x1F) | 0xC0;
      mCurString.put_char(h);
      mCurString.put_char(l);
    }
    else { // ここでは 0x10000 以上のコードはない．
      char l = static_cast<char>(code & 0x3F) | 0x80;
      char m = static_cast<char>((code >> 6) & 0x3F) | 0x80;
      char h = static_cast<char>((code >> 12) & 0x0F) | 0xE0;
      mCurString.put_char(h);
      mCurString.put_char(m);
      mCurString.put_char(l);
    }
  }
  goto ST_DQ;

 ST_CM1: // '*/' までをコメントとして読み飛ばす．
  c = get();
  if ( c == '*' ) {
    goto ST_CM1_1;
  }
  if ( c == EOF ) {
    goto ST_ERROR;
  }
  goto ST_CM1;

 ST_CM1_1:
  c = get();
  if ( c == '/' ) {
    goto ST_INIT;
  }
  if ( c == '*' ) {
    goto ST_CM1_1;
  }
  if ( c == EOF ) {
    goto ST_ERROR;
  }
  goto ST_CM1;

 ST_CM2: // 改行までをコメントとして読み飛ばす．
  c = get();
  if ( c == '\n' ) {
    goto ST_INIT;
  }
  if ( c == EOF ) {
    return JsonToken::End;
  }
  goto ST_CM2;

 ST_ERROR:
  MsgMgr::put_msg(__FILE__, __LINE__,
		  cur_region(),
		  MsgType::Error,
		  "JSON_SYNTAX_ERROR",
		  "syntax_error");
  throw std::invalid_argument{"Syntax error"};
}

// @brief 直前の read_token() で読み出した字句の文字列を返す．
string
JsonScanner::cur_string()
{
  return mCurString;
}

// @brief 直前の read_token() 読み出した字句の整数を返す．
int
JsonScanner::cur_int()
{
  return atoi(mCurString.c_str());
}

// @brief 直前の read_token() 読み出した字句の浮動小数点数を返す．
double
JsonScanner::cur_float()
{
  return atof(mCurString.c_str());
}

// @brief 直前の read_token() で読み出した字句のファイル上の位置を返す．
FileRegion
JsonScanner::cur_loc()
{
  return mCurLoc;
}

// @brief 'true' を読み込む．
bool
JsonScanner::read_true()
{
  if ( get() != 'r' ) {
    return false;
  }
  if ( get() != 'u' ) {
    return false;
  }
  if ( get() != 'e' ) {
    return false;
  }
  return true;
}

// @brief 'false' を読み込む．
bool
JsonScanner::read_false()
{
  if ( get() != 'a' ) {
    return false;
  }
  if ( get() != 'l' ) {
    return false;
  }
  if ( get() != 's' ) {
    return false;
  }
  if ( get() != 'e' ) {
    return false;
  }
  return true;
}

// @brief 'null' を読み込む．
bool
JsonScanner::read_null()
{
  if ( get() != 'u' ) {
    return false;
  }
  if ( get() != 'l' ) {
    return false;
  }
  if ( get() != 'l' ) {
    return false;
  }
  return true;
}

END_NAMESPACE_YM_JSON
