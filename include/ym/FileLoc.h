#ifndef YM_FILELOC_H
#define YM_FILELOC_H

/// @file ym/FileLoc.h
/// @brief FileLocのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"
#include "ym/FileInfo.h"
#include "ym/FileLineColumn.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class FileLoc FileLoc.h "ym/FileLoc.h"
/// @ingroup ym
/// @brief ファイル位置を表すクラス
///
/// 基本的には
/// - ファイル情報
/// - 行番号
/// - コラム位置
/// の情報を持つ
/// @sa FileInfo FileInfoMgr FileRegion
//////////////////////////////////////////////////////////////////////
class FileLoc
{
  friend class FileRegion;

public:

  /// @brief 空のコンストラクタ
  ///
  /// 無効なデータを持つ
  FileLoc() = default;

  /// @brief 内容を指定するコンストラクタ
  /// @param[in] file_info ファイル情報
  /// @param[in] line 行番号
  /// @param[in] column コラム番号
  FileLoc(FileInfo file_info,
	  int line,
	  int column);

  /// @brief デストラクタ
  ~FileLoc() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データの妥当性のチェック
  /// @retval true 意味のある値を持っている時
  /// @retval false 無効なデータの時
  bool
  is_valid() const;

  /// @brief ファイル情報の取得
  /// @return ファイル情報を返す．
  FileInfo
  file_info() const;

  /// @brief ファイル名を返す．
  string
  filename() const;

  /// @brief インクルード元のファイル位置を返す．
  /// @note インクルードされていないファイルの場合には無効なデータが返される．
  FileLoc
  parent_loc() const;

  /// @brief インクルード元のファイル位置の情報をすべてリストに積む．
  /// @param[out] loc_list ファイルの位置情報のリスト
  /// @note トップレベルのファイルが先頭になる．
  void
  parent_loc_list(vector<FileLoc>& loc_list) const;

  /// @brief 行番号の取得
  /// @return 行番号
  int
  line() const;

  /// @brief コラム位置の取得
  /// @return コラム位置
  int
  column() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル情報
  FileInfo mFileInfo;

  // 行番号とコラム位置
  FileLineColumn mLineColumn;

};


//////////////////////////////////////////////////////////////////////
/// @name FileLoc に関連する非メンバ関数
/// @{

/// @relates FileLoc
/// @brief 等価比較演算子
/// @param[in] left, right 比較するオブジェクト
/// @retval true 等しい
/// @retval false 等しくない
bool
operator==(const FileLoc& left,
	   const FileLoc& right);

/// @relates FileLoc
/// @brief 非等価比較演算子
/// @param[in] left, right 比較するオブジェクト
/// @retval true 等しくない．
/// @retval false 等しい．
bool
operator!=(const FileLoc& left,
	   const FileLoc& right);

/// @relates FileLoc
/// @brief FileLoc を表示するための関数
/// @param[in] s 出力ストリーム
/// @param[in] file_loc ファイル位置の情報
/// @return s をそのまま返す
ostream&
operator<<(ostream& s,
	   const FileLoc& file_loc);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 内容を指定するコンストラクタ
// @param[in] file_info ファイル情報
// @param[in] line 行番号
// @param[in] column コラム番号
inline
FileLoc::FileLoc(FileInfo file_info,
		 int line,
		 int column) :
  mFileInfo{file_info},
  mLineColumn{line, column}
{
}

// @brief データの妥当性のチェック
// @retval true 意味のある値を持っている時
// @retval false 無効なデータの時
inline
bool
FileLoc::is_valid() const
{
  return mFileInfo.is_valid();
}

// @brief ファイル記述子の取得
// @return ファイル記述子を返す．
inline
FileInfo
FileLoc::file_info() const
{
  return mFileInfo;
}

// @brief ファイル名を返す．
inline
string
FileLoc::filename() const
{
  return file_info().filename();
}

// @brief インクルード元のファイル位置を返す．
// @note インクルードされていないファイルの場合には無効なデータが返される．
inline
FileLoc
FileLoc::parent_loc() const
{
  return file_info().parent_loc();
}

// @brief インクルード元のファイル位置の情報をすべてリストに積む．
// @param[out] loc_list ファイルの位置情報のリスト
// @note トップレベルのファイルが先頭になる．
inline
void
FileLoc::parent_loc_list(vector<FileLoc>& loc_list) const
{
  file_info().parent_loc_list(loc_list);
}

// @brief 行番号の取得
// @return 行番号
inline
int
FileLoc::line() const
{
  return mLineColumn.line();
}

// @brief コラム位置の取得
// @return コラム位置
inline
int
FileLoc::column() const
{
  return mLineColumn.column();
}

// @relates FileLoc
// @brief 等価比較演算子
// @param[in] left, right 比較するオブジェクト
// @retval true 等しい
// @retval false 等しくない
inline
bool
operator==(const FileLoc& left,
	   const FileLoc& right)
{
  return left.file_info() == right.file_info() &&
    left.line() == right.line() && left.column() == right.column();
}

// @relates FileLoc
// @brief 非等価比較演算子
// @param[in] left, right 比較するオブジェクト
// @retval true 等しくない．
// @retval false 等しい．
inline
bool
operator!=(const FileLoc& left,
	   const FileLoc& right)
{
  return !operator==(left, right);
}

END_NAMESPACE_YM

#endif // YM_FILELOC_H
