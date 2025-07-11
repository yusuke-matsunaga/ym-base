#ifndef YM_FILELOC_H
#define YM_FILELOC_H

/// @file ym/FileLoc.h
/// @brief FileLocのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2021 Yusuke Matsunaga
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
  FileLoc(
    FileInfo file_info, ///< [in] ファイル情報
    int line,           ///< [in] 行番号
    int column          ///< [in] コラム番号
  ) : mFileInfo{file_info},
      mLineColumn{line, column}
  {
  }

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
  is_valid() const { return mFileInfo.is_valid(); }

  /// @brief ファイル情報の取得
  /// @return ファイル情報を返す．
  FileInfo
  file_info() const { return mFileInfo; }

  /// @brief ファイル名を返す．
  std::string
  filename() const { return file_info().filename(); }

  /// @brief インクルード元のファイル位置を返す．
  /// @note インクルードされていないファイルの場合には無効なデータが返される．
  FileLoc
  parent_loc() const { return file_info().parent_loc(); }

  /// @brief インクルード元のファイル位置の情報のリスト(vector)を返す．
  /// @note トップレベルのファイルが先頭になる．
  std::vector<FileLoc>
  parent_loc_list() const { return file_info().parent_loc_list(); }

  /// @brief 行番号の取得
  /// @return 行番号
  int
  line() const { return mLineColumn.line(); }

  /// @brief コラム位置の取得
  /// @return コラム位置
  int
  column() const { return mLineColumn.column(); }


public:
  //////////////////////////////////////////////////////////////////////
  // 演算子
  //////////////////////////////////////////////////////////////////////

  /// @brief 等価比較演算子
  /// @retval true 等しい
  /// @retval false 等しくない
  bool
  operator==(
    const FileLoc& right ///< [in] 右のオペランド
  ) const
  {
    return file_info() == right.file_info() &&
    line() == right.line() &&
    column() == right.column();
  }

  /// @brief 非等価比較演算子
  /// @retval true 等しくない．
  /// @retval false 等しい．
  bool
  operator!=(
    const FileLoc& right ///< [in] 右のオペランド
  ) const
  {
    return !operator==(right);
  }


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
/// @brief FileLoc を表示するための関数
/// @return s をそのまま返す
std::ostream&
operator<<(
  std::ostream& s,        ///< [in] 出力ストリーム
  const FileLoc& file_loc ///< [in] ファイル位置の情報
);

/// @}
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM

#endif // YM_FILELOC_H
