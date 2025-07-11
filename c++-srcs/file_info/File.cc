
/// @file File.cc
/// @brief ファイル操作関係のクラスの実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/File.h"

#if defined(YM_WIN32)
#include <direct.h>
#endif
/// @todo pwd.h や sys/param.h がないときの対処
#if defined(YM_HAVE_PWD_H)
#  include <pwd.h>
#endif
#if defined(YM_HAVE_SYS_PARAM_H)
#  include <sys/param.h>
#endif
#if defined(YM_HAVE_SYS_STAT_H) || defined(YM_WIN32)
#  include <sys/stat.h>
#  include <sys/types.h>
#endif


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// パス名を表すクラス
//////////////////////////////////////////////////////////////////////

// 文字列からの変換コンストラクタ
PathName::PathName(
  const std::string& path_str
)
{
  if ( path_str.size() > 0 ) {
    std::string::size_type pos = 0;
    if ( path_str[0] == '/' ) {
      mType = PathType::Absolute;
      pos = 1;
    }
    else if ( path_str[0] == '~' ) {
      mType = PathType::Home;
      pos = 1;
    }
    else {
      mType = PathType::Relative;
    }
    for ( ; ; ) {
      std::string::size_type pos2 = path_str.find('/', pos);
      if ( pos2 != std::string::npos ) {
	mPathList.push_back(path_str.substr(pos, pos2 - pos));
	pos = pos2 + 1;
      }
      else {
	mPathList.push_back(path_str.substr(pos));
	break;
      }
    }
  }
}

// 文字列からの変換コンストラクタ
PathName::PathName(
  const char* path_str
) : PathName(std::string{path_str})
{
}

// パス名の文字列表現を返す．
std::string
PathName::str() const
{
  std::string ans;
  switch ( type() ) {
  case PathType::Absolute:
    ans += '/';
    break;
  case PathType::Home:
    ans += '~';
    break;
  case PathType::Relative:
    break;
  }
  const char* sep = "";
  for ( auto str: mPathList ) {
    ans += sep;
    ans += str;
    sep = "/";
  }
  return ans;
}

// パス名のヘッダ(最後の名前を含まないもの)を返す．
PathName
PathName::head() const
{
  std::vector<std::string> new_list{mPathList};
  if ( !new_list.empty() ) {
    new_list.pop_back();
  }
  return PathName(mType, new_list);
}

// パス名のテイル(ヘッダを含まないもの)を返す．
std::string
PathName::tail() const
{
  if ( mPathList.empty() ) {
    return std::string{};
  }
  return mPathList.back();
}

// 拡張子の直前のドットの位置を返す．
std::string::size_type
PathName::dot_pos(
  const std::string& path
)
{
  if ( path == ".." ) {
    return std::string::npos;
  }
  std::string::size_type p = path.rfind('.');
  if ( p == 0 ) {
    // path == "." の場合もここではじかれる．
    return std::string::npos;
  }
  return p;
}

// パス名の本体(テイルから "." と拡張子を除いた物)を返す．
std::string
PathName::body() const
{
  std::string tmp = tail();
  std::string::size_type p = dot_pos(tmp);
  return tmp.substr(0, p);
}

// パス名の拡張子を返す．
std::string
PathName::ext() const
{
  std::string tmp = tail();
  std::string::size_type p = dot_pos(tmp);
  if ( p == std::string::npos ) {
    return std::string{};
  }
  return tmp.substr(p + 1, std::string::npos);
}

// パスタイプが PathType::Home と PathType::Relative の時にフルパス形式に展開する．
PathName
PathName::expand() const
{
  switch ( type() ) {
  case PathType::Absolute:
    return *this;

  case PathType::Home:
    {
      auto p1 = mPathList.begin();
      ++ p1;
      std::vector<std::string> new_list{p1, mPathList.end()};
      return user_home(mPathList.front()) + PathName( PathType::Relative, new_list);
    }

  case PathType::Relative:
    return cur_work_dir() + (*this);
  }
  // ダミー
  return PathName();
}

#if 0
#if defined(YM_WIN32)
// パスが存在しているか調べる．
bool
PathName::stat(struct _stat64i32* sbp) const
{
  struct _stat64i32 dummy;
  if ( !sbp ) {
    sbp = &dummy;
  }
  return _stat(str().c_str(), sbp) == 0;
}
#else
// パスが存在しているか調べる．
bool
PathName::stat(struct stat* sbp) const
{
  struct stat dummy;
  if ( !sbp ) {
    sbp = &dummy;
  }
  return ::stat(str().c_str(), sbp) == 0;
}
#endif
#else
#if defined(YM_WIN32)
// パスが存在しているか調べる．
bool
PathName::stat() const
{
  struct _stat64i32 dummy;
  return _stat(str().c_str(), &dummy) == 0;
}
#else
// パスが存在しているか調べる．
bool
PathName::stat() const
{
  struct stat dummy;
  return ::stat(str().c_str(), &dummy) == 0;
}
#endif
#endif

// 末尾にパスをつなげる．
const PathName&
PathName::operator+=(
  const PathName& src
)
{
  if ( is_valid() ) {
    if ( src.is_valid() && src.type() == PathType::Relative ) {
      if ( mPathList.back() == std::string() ) {
	// 最後が空('/'で終わっている)の場合にはその空要素を取り除く
	mPathList.pop_back();
      }
      mPathList.insert(mPathList.end(),
		       src.mPathList.begin(), src.mPathList.end());
    }
  }
  else {
    // this が空なので src をコピーする．
    mType = src.mType;
    mPathList = src.mPathList;
  }
  return *this;
}

// 2つのパス名を連結する．
PathName
operator+(
  const PathName& opr1,
  const PathName& opr2
)
{
  // ほとんど C++ の定石のコード
  return PathName(opr1) += opr2;
}


//////////////////////////////////////////////////////////////////////
// サーチパス(のリスト)を表すクラス
//////////////////////////////////////////////////////////////////////

#if 0
// コンストラクタ
SearchPathList::SearchPathList()
{
}

// 文字列からの変換コンストラクタ
SearchPathList::SearchPathList(
  const std::string& str
)
{
  to_list(str, mList);
}

// @brief コピーコンストラクタ
SearchPathList::SearchPathList(
  const SearchPathList& src
) : mList(src.mList)
{
}

// @brief 代入演算子
// @param[in] src 代入元のオブジェクト
const SearchPathList&
SearchPathList::operator=(const SearchPathList& src)
{
  // 基本的にデフォルトの実装で大丈夫なはず．
  mList = src.mList;

  return *this;
}

// デストラクタ
SearchPathList::~SearchPathList()
{
}

// 以前の内容をクリアして文字列をセットする．
void
SearchPathList::set(const string& str)
{
  mList.clear();
  to_list(str, mList);
}

// サーチパスの先頭に path を追加する．
void
SearchPathList::add_top(const string& path)
{
  vector<PathName> tmp_list;
  to_list(path, tmp_list);
  mList.insert(mList.begin(), tmp_list.begin(), tmp_list.end());
}

// サーチパスの末尾に path を追加する．
void
SearchPathList::add_end(const string& path)
{
  to_list(path, mList);
}
#endif

// サーチパスを考慮して filename を探す
PathName
SearchPathList::search(
  const PathName& filename
) const
{
  switch ( filename.type() ) {
  case PathType::Absolute:
  case PathType::Home:
    {
      // 絶対パスならそれを試すだけ
      // ホームディレクトリからの相対パスの場合も唯一のパスを試す．
      auto tmp = filename.expand();
      if ( tmp.stat() ) {
	return tmp;
      }
    }
    break;

  case PathType::Relative:
    if ( mList.empty() ) {
      // サーチパスが空の場合はカレントディレクトリからの相対パス
      // とみなす．
      auto tmp = filename.expand();
      if ( tmp.stat() ) {
	return tmp;
      }
    }
    else {
      // 相対パスの場合, 頭に search_path をつけて探す．
      for ( auto path: mList ) {
	auto tmp = (path + filename).expand();
	if ( tmp.stat() ) {
	  return tmp;
	}
      }
    }
    break;
  }

  return PathName();
}

// 現在のサーチパスを取り出す．
std::string
SearchPathList::to_string(
  const std::string& separator
) const
{
  std::string ans;
  std::string sep = "";
  for ( auto path: mList ) {
    ans += sep;
    ans += path.str();
    sep = separator;
  }
  return ans;
}

// 文字列を PathName のリストに変換する
void
SearchPathList::to_list(
  const std::string& str,
  std::vector<PathName>& pathname_list
)
{
  std::string::size_type pos1 = 0;
  std::string::size_type pos2;
  for ( ; (pos2 = str.find(':', pos1)) != std::string::npos; pos1 = pos2 + 1) {
    pathname_list.push_back(PathName(str.substr(pos1, pos2)));
  }
  pathname_list.push_back(PathName(str.substr(pos1, std::string::npos)));
}


// カレントディレクトリの取得
PathName
cur_work_dir()
{
#if defined(YM_WIN32)
  const int max_pathlen = _MAX_PATH;
#else
  const int max_pathlen = MAXPATHLEN;
#endif
  char buff[max_pathlen + 1];
#if defined(YM_WIN32)
  char* tmp = _getcwd(buff, sizeof(buff));
#else
  char* tmp = getcwd(buff, sizeof(buff));
#endif
  if ( tmp == nullptr ) {
    // なんか知らないけどエラー
    return PathName();
  }

  return PathName(buff);
}

// ユーザのホームディレクトリの取得
PathName
user_home(
  const std::string& login
)
{
  std::string hdir;
  if ( login == std::string() ) {
#if defined(YM_WIN32)
    char* buffer;
    size_t num;
    errno_t en = _dupenv_s(&buffer, &num, "HOME");
    if ( en == 0 ) {
      hdir = buffer;
      free(buffer);
    }
#else
    char* henv = getenv("HOME");
    if ( henv ) {
      hdir = henv;
    }
#endif
  }
  else {
#if defined(YM_UNIX)
    struct passwd* ppw = getpwnam(login.c_str());
    if ( ppw ) {
      hdir = ppw->pw_dir;
    }
    endpwent();
#endif
  }
  return PathName(hdir);
}

END_NAMESPACE_YM
