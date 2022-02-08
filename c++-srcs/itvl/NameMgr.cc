
/// @file NameMgr.cc
/// @brief NameMgr の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2010, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/NameMgr.h"
#include "ItvlTree.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス NameMgr
//////////////////////////////////////////////////////////////////////

// コンストラクタ
NameMgr::NameMgr(
  const char* prefix,
  const char* suffix
) : mPrefix{prefix},
    mSuffix{suffix},
    mInterval{new ItvlTree}
{
}

// コンストラクタ
NameMgr::NameMgr(
  const string& prefix,
  const string& suffix
) : mPrefix{prefix},
    mSuffix{suffix},
    mInterval{new ItvlTree}
{
}

// デストラクタ
NameMgr::~NameMgr()
{
}

// @brief 接頭語と接尾語を変更する．
// @note 既に登録されている名前はクリアされる．
void
NameMgr::change(
  const char* prefix,
  const char* suffix
)
{
  clear();
  mPrefix = prefix;
  mSuffix = suffix;
}

// @brief 接頭語と接尾語を変更する．
// @note 既に登録されている名前はクリアされる．
void
NameMgr::change(
  const string& prefix,
  const string& suffix
)
{
  clear();
  mPrefix = prefix;
  mSuffix = suffix;
}

// 登録している名前を全てクリアする
void
NameMgr::clear()
{
  mInterval->clear();
}

// 次に使用可能な名前を接頭語も含めて返す．
// 使用可能な名前がない場合(!!!)，アボートする．
string
NameMgr::new_name(
  bool add_name
)
{
  int d = mInterval->get_min();
  ASSERT_COND( d >= 0 );

  ostringstream buf;
  buf << mPrefix << d << mSuffix;
  mTmpString = buf.str();
  mLastNum = d;

  if ( add_name ) {
    mInterval->add(d);
  }

  return mTmpString;
}

// 名前を登録する．
// name が <prefix>ddd<suffix> の形でない場合にはなにもしない．
void
NameMgr::add(
  const char* name
)
{
  // 数字を抜き出す．
  int d = str_to_num(name);
  if ( d != -1 ) {
    mInterval->add(d);
  }
}

// 名前を削除する(使用可能にする)．
// name が <prefix>ddd<suffix> の形でない場合にはなにもしない．
void
NameMgr::erase(
  const char* name
)
{
  // 数字を抜き出す．
  int d = str_to_num(name);
  if ( d != -1 ) {
    // 区間に d を追加
    mInterval->del(d);
  }
}

// name が <prefix>ddd<suffix> の形の場合に
// ddd の部分を数値に直したものを返す．
// そうでなければ -1 を返す．
int
NameMgr::str_to_num(
  const char* name
) const
{
  SizeType plen = mPrefix.size();
  SizeType slen = mSuffix.size();
  SizeType nlen = strlen(name);

  // 接頭語と接尾語を足した長さのほうが長ければ数値があるわけない．
  if ( plen + slen >= nlen ) {
    return -1;
  }

  // prefix を比較
  for ( int i = 0; i < plen; ++ i ) {
    if ( mPrefix[i] != name[i] ) {
      return -1;
    }
  }
  // suffix を比較
  for ( int i = 0; i < slen; ++ i ) {
    if ( mSuffix[i] != name[nlen - slen + i] ) {
      return -1;
    }
  }

  // 数字を抜き出す．
  int d = 0;
  for ( int i = plen; i < nlen - slen; ++ i ) {
    char c = name[i];
    if ( !isdigit(c) ) {
      // 数字以外の文字があった．
      return -1;
    }
    d = (d * 10) + (c - '0');
  }

  // それを返す．
  return d;
}

// 内容を表示する．
void
NameMgr::print(
  ostream& s
) const
{
  s << "<<<<NameMgr>>>>" << endl
    << "Prefix: '" << mPrefix << "'" << endl
    << "Suffix: '" << mSuffix << "'" << endl;
  mInterval->print(s);
}

END_NAMESPACE_YM
