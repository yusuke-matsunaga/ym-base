#ifndef YM_FILEREGION_H
#define YM_FILEREGION_H

/// @file ym/FileRegion.h
/// @brief FileRegion のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"
#include "ym/FileInfo.h"
#include "ym/FileLoc.h"
#include "ym/FileLineColumn.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class FileRegion FileRegion.h "ym/FileRegion.h"
/// @ingroup ym
/// @brief ファイル上の領域を表すクラス
/// @sa FileLoc FileInfo FileInfoMgr
///
/// 意味的には先頭位置を表す FileLoc と末尾位置を表す FileLoc の組だが
/// 内部では効率的に表している．
//////////////////////////////////////////////////////////////////////
class FileRegion
{
public:

  /// @brief 空のデフォルトコンストラクタ
  ///
  /// 結果は不正な情報を持つオブジェクトとなる．
  FileRegion() = default;

  /// @brief 内容を指定したコンストラクタ
  FileRegion(
    FileInfo file_info, ///< [in] ファイル情報
    int start_line,     ///< [in] 先頭の行番号
    int start_column,   ///< [in] 先頭のコラム番号
    int end_line,       ///< [in] 末尾の行番号
    int end_column      ///< [in] 末尾のコラム番号
  ) : mStartFileInfo(file_info),
      mEndFileInfo(file_info),
      mStartLineColumn(start_line, start_column),
      mEndLineColumn(end_line, end_column)
  {
  }

  /// @brief 内容を指定したコンストラクタ
  FileRegion(
    FileInfo start_file_info, ///< [in] 先頭のファイル情報
    int start_line,	      ///< [in] 先頭の行番号
    int start_column,	      ///< [in] 先頭のコラム番号
    FileInfo end_file_info,   ///< [in] 末尾のファイル情報
    int end_line,             ///< [in] 末尾の行番号
    int end_column  	      ///< [in] 末尾のコラム番号
  ) : mStartFileInfo(start_file_info),
      mEndFileInfo(end_file_info),
      mStartLineColumn(start_line, start_column),
      mEndLineColumn(end_line, end_column)
  {
  }

  /// @brief 内容を指定したコンストラクタ
  FileRegion(
    const FileLoc& file_loc ///< [in] ファイル上の位置
  ) : mStartFileInfo(file_loc.file_info()),
      mEndFileInfo(file_loc.file_info()),
      mStartLineColumn(file_loc.mLineColumn),
      mEndLineColumn(file_loc.mLineColumn)
  {
  }

  /// @brief 内容を指定したコンストラクタ
  FileRegion(
    const FileLoc& start_file_loc, ///< [in] 先頭のファイル上の位置
    const FileLoc& end_file_loc    ///< [in] 末尾のファイル上の位置
  ) : mStartFileInfo(start_file_loc.file_info()),
      mEndFileInfo(end_file_loc.file_info()),
      mStartLineColumn(start_file_loc.mLineColumn),
      mEndLineColumn(end_file_loc.mLineColumn)
  {
  }

  /// @brief 内容を指定したコンストラクタ
  FileRegion(
    const FileRegion& start_file_region, ///< [in] 先頭のファイル上の位置
    const FileRegion& end_file_region    ///< [in] 末尾のファイル上の位置
  ) : mStartFileInfo(start_file_region.mStartFileInfo),
      mEndFileInfo(end_file_region.mEndFileInfo),
      mStartLineColumn(start_file_region.mStartLineColumn),
      mEndLineColumn(end_file_region.mEndLineColumn)
  {
  }

  /// @brief デストラクタ
  ~FileRegion() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データの妥当性のチェック
  /// @return 意味のある値を持っているとき true を返す．
  bool
  is_valid() const
  {
    return mStartFileInfo.is_valid();
  }

  /// @brief 先頭のファイル位置の取得
  /// @return ファイル位置
  FileLoc
  start_loc() const
  {
    return FileLoc(mStartFileInfo, start_line(), start_column());
  }

  /// @brief 先頭のファイル情報の取得
  /// @return ファイル情報を返す．
  FileInfo
  start_file_info() const
  {
    return mStartFileInfo;
  }

  /// @brief 先頭の行番号の取得
  /// @return 行番号
  int
  start_line() const
  {
    return mStartLineColumn.line();
  }

  /// @brief 先頭のコラム位置の取得
  /// @return コラム位置
  int
  start_column() const
  {
    return mStartLineColumn.column();
  }

  /// @brief 末尾のファイル位置の取得
  /// @return ファイル位置
  FileLoc
  end_loc() const
  {
    return FileLoc(mEndFileInfo, end_line(), end_column());
  }

  /// @brief 末尾のファイル情報の取得
  FileInfo
  end_file_info() const
  {
    return mEndFileInfo;
  }

  /// @brief 末尾の行番号の取得
  /// @return 行番号
  int
  end_line() const
  {
    return mEndLineColumn.line();
  }

  /// @brief 末尾のコラム位置の取得
  /// @return コラム位置
  int
  end_column() const
  {
    return mEndLineColumn.column();
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭のファイル情報
  FileInfo mStartFileInfo;

  // 末尾のファイル情報
  FileInfo mEndFileInfo;

  // 先頭の行番号とコラム位置
  FileLineColumn mStartLineColumn;

  // 末尾の行番号とコラム位置
  FileLineColumn mEndLineColumn;

};


//////////////////////////////////////////////////////////////////////
/// @name FileRegion の出力関数
/// @{

/// @relates FileRegion
/// @brief FileRegion を表示するための関数
/// @return s をそのまま返す
std::ostream&
operator<<(
  std::ostream& s,              ///< [in] 出力ストリーム
  const FileRegion& file_region ///< [in] ファイル領域の情報
);

/// @}
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM

#endif // YM_FILEREGION_H
