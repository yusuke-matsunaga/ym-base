
/// @file FileInfo.cc
/// @brief FileInfo の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.

#include "ym/FileInfo.h"
#include "ym/FileLoc.h"
#include "ym/FileRegion.h"

#include "FileInfoMgr.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// スタティックオブジェクト
FileInfoMgr gTheMgr;

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス FileInfo
//////////////////////////////////////////////////////////////////////

// @brief ファイル名を指定したコンストラクタ
FileInfo::FileInfo(
  const char* filename
)
{
  mId = gTheMgr.new_file_info(filename);
}

// @brief ファイル名を指定したコンストラクタ
FileInfo::FileInfo(
  const std::string& filename
)
{
  mId = gTheMgr.new_file_info(filename.c_str());
}

// @brief ファイル名とインクルード元の親ファイルの情報
FileInfo::FileInfo(
  const char* filename,
  const FileLoc& parent_loc
)
{
  mId = gTheMgr.new_file_info(filename, parent_loc);
}

// @brief ファイル名とインクルード元の親ファイルの情報
FileInfo::FileInfo(
  const std::string& filename,
  const FileLoc& parent_loc
)
{
  mId = gTheMgr.new_file_info(filename.c_str(), parent_loc);
}

// @brief ファイル名を返す．
std::string
FileInfo::filename() const
{
  return gTheMgr.filename(mId);
}

// @brief インクルード元のファイル位置を返す．
FileLoc
FileInfo::parent_loc() const
{
  return gTheMgr.parent_loc(mId);
}

// @brief インクルード元のファイル位置の情報をすべてリストに積む．
std::vector<FileLoc>
FileInfo::parent_loc_list() const
{
  // 逆順にするために一旦 tmp_list に入れる．
  std::vector<FileLoc> tmp_list;
  for ( auto loc = parent_loc(); loc.is_valid(); loc = loc.parent_loc() ) {
    tmp_list.push_back(loc);
  }

  std::reverse(tmp_list.begin(), tmp_list.end());
  return tmp_list;
}

// @brief 内部の静的なデータをクリアする．
void
FileInfo::clear()
{
  gTheMgr.clear();
}

// @brief FileInfo 用のストリーム出力演算子
std::ostream&
operator<<(
  std::ostream& s,
  const FileInfo& file_info
)
{
  auto loc_list = file_info.parent_loc_list();
  for ( auto loc: loc_list ) {
    s << "In file included from " << loc.filename()
      << ": line " << loc.line() << ":" << std::endl;
  }
  s << file_info.filename();

  return s;
}


//////////////////////////////////////////////////////////////////////
// クラス FileLoc
//////////////////////////////////////////////////////////////////////

// @brief FileLoc を表示するための関数
std::ostream&
operator<<(
  std::ostream& s,
  const FileLoc& file_loc
)
{
  if ( file_loc.is_valid() ) {
    s << file_loc.file_info()
      << ": line " << file_loc.line()
      << ", column " << file_loc.column();
  }
  else {
    s << "invalid file_loc";
  }

  return s;
}

// @brief FileRegion を表示するための関数
std::ostream&
operator<<(
  std::ostream& s,
  const FileRegion& file_region
)
{
  auto first_fi = file_region.start_file_info();
  auto last_fi = file_region.end_file_info();
  int first_line = file_region.start_line();
  int first_column = file_region.start_column();
  int last_line = file_region.end_line();
  int last_column = file_region.end_column();

  if ( first_fi.is_valid() ) {
    // 意味のあるファイル記述子
    if ( first_fi == last_fi ) {
      // 同じファイル
      s << first_fi;
      if ( first_line == last_line ) {
	// 同じ行番号
	s << ": line " << first_line;
	if ( first_column == last_column ) {
	  // 同じコラム位置
	  s << ", column " << first_column;
	}
	else {
	  // 異なるコラム
	  s << ", column " << first_column
	    << " - " << last_column;
	}
      }
      else {
	// 異なる行
	s << ": line " << first_line
	  << ", column " << first_column
	  << " - line " << last_line
	  << ", column " << last_column;
      }
    }
    else {
      // 異なるファイル
      s << first_fi
	<< ": line " << first_line
	<< ", column " << first_column
	<< " - "
	<< last_fi
	<< ": line " << last_line
	<< ", column " << last_column;
    }
  }
  else {
    s << "---";
  }

  return s;
}

END_NAMESPACE_YM
