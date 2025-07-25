﻿#ifndef YM_FILE_H
#define YM_FILE_H

/// @file ym/File.h
/// @brief ファイル操作関係のクラスのヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

/// @brief パスの型を表す列挙型
enum class PathType {
  Absolute, ///< 絶対パス
  Home,     ///< ホームからの相対パス．最初の文字列はユーザ名
  Relative  ///< カレントディレクトリからの相対パス
};


//////////////////////////////////////////////////////////////////////
/// @class PathName File.h "ym/File.h"
/// @ingroup YmUtils
/// @brief パス名を表すクラス
///
/// 直感的に言えばパス名を表す文字列のみをもっているクラスだが，
/// 相対パスやチルダ("~")展開のようなパス名固有の処理を内蔵したクラス
//////////////////////////////////////////////////////////////////////
class PathName
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  ///
  /// 結果は相対パスのカレントディレクトリを表すオブジェクトとなる
  PathName() = default;

  /// @brief 文字列からの変換コンストラクタ
  PathName(
    const std::string& path_str ///< [in] パスを表す文字列
  );

  /// @brief 文字列からの変換コンストラクタ
  PathName(
    const char* path_str ///< [in] パスを表す文字列
  );

  /// @brief デストラクタ
  ~PathName() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief パス名の妥当性チェック
  /// @return 意味のある値を持っているとき true を返す．
  ///
  /// ここでは形式のみチェックする．
  /// そのパスが存在するか等は関係ない．
  /// 空の場合は valid ではない．
  bool
  is_valid() const { return !mPathList.empty(); }

  /// @brief パスの型を返す．
  /// @return パスの型
  PathType
  type() const { return mType; }

  /// @brief パス名の文字列表現を返す．
  /// @return パス名の文字列表現
  std::string
  str() const;

  /// @brief パス名のヘッダを返す．
  /// @return パス名のヘッダ (最後の名前を含まないもの)
  ///
  /// 元々が階層を含まない場合には空になる．
  PathName
  head() const;

  /// @brief パス名のテイルを返す．
  /// @return パス名のテイル (ヘッダを含まないもの)
  ///
  /// パス名が '/' で終わっていたらテイルは空となる．
  std::string
  tail() const;

  /// @brief パス名の本体(tail から "."と拡張子を除いた物)を返す
  /// @return パス名の本体(tail から "."と拡張子を除いた物)を返す．
  std::string
  body() const;

  /// @brief パス名の拡張子を返す．
  /// @return パス名の拡張子
  ///
  /// "." を含まない場合には空になる．
  std::string
  ext() const;

  /// @brief パス名の展開
  /// @return 展開したパス
  ///
  /// パスタイプが kHome と kRelative の時にフルパス形式に展開する．
  /// 結果は必ず PathType::Absolute になる．
  PathName
  expand() const;

#if 0
#if defined(YM_WIN32)
  /// @brief パスが存在しているか調べる．
  /// @return 存在していたら true を返す．
  bool
  stat(
    struct _stat64i32* sbp = nullptr ///< [out] stat システムコールの結果を格納する構造体
                                     ///< nullptr ならどこにも結果を格納しない
  ) const;
#else
  /// @brief パスが存在しているか調べる．
  /// @return 存在していたら true を返す．
  ///
  /// nullptr ならどこにも結果を格納しない
  bool
  stat(
    struct stat* sbp = nullptr ///< [out] stat システムコールの結果を格納する構造体
                               ///< nullptr ならどこにも結果を格納しない
  ) const;
#endif
#else
  /// @brief パスが存在しているか調べる．
  /// @return 存在していたら true を返す．
  bool
  stat() const;
#endif

  /// @brief 末尾にパスをつなげる．
  /// @return 結合したパス
  ///
  /// - src のタイプが kRelative でないときは無視する．
  /// - src が空の場合も無視する．
  /// - 自分自身が空の場合には src を代入する．
  const PathName&
  operator+=(
    const PathName& src ///< [in] 追加するパス
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を直接指定したコンストラクタ
  PathName(
    PathType type,                            ///< [in] パスタイプ
    const std::vector<std::string>& path_list ///< [in] パスリスト
  ) : mType{type},
      mPathList{path_list}
  {
  }

  /// @brief 拡張子の直前のドットの位置を返す．
  /// @return 拡張子の直前のドットの位置
  ///
  /// ただし，"." や ".." の場合や先頭がドットで始まっていて，
  /// それが唯一のドットの場合には拡張子はなしと見なす．
  static
  std::string::size_type
  dot_pos(
    const std::string& path ///< [in] 対象のパス文字列
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // タイプ
  PathType mType{PathType::Relative};

  // パス名を表す本体
  std::vector<std::string> mPathList;

};

/// @name PathName に関連したファイル名操作関数
/// @{

/// @relates PathName
/// @ingroup YmUtils
/// @brief 2つのパス名を連結する．
/// @return 連結したパス名
PathName
operator+(
  const PathName& opr1, ///< [in] 先頭のパス名
  const PathName& opr2  ///< [in] 末尾のパス名
);

/// @relates PathName
/// @ingroup YmUtils
/// @brief カレントディレクトリを表す絶対パス名を返す．
/// @return カレントディレクトリを表す絶対パス名を返す．
PathName
cur_work_dir();

/// @relates PathName
/// @ingroup YmUtils
/// @brief ユーザのホームディレクトリの取得
/// @return login のホームディレクトリ
PathName
user_home(
  const std::string& login ///< [in] ユーザーアカウント名
);

/// @}


//////////////////////////////////////////////////////////////////////
/// @class SearchPathList File.h "ym/File.h"
/// @ingroup YmUtils
/// @brief サーチパス(のリスト)を表すクラス
/// @sa PathName
//////////////////////////////////////////////////////////////////////
class SearchPathList
{
public:

  /// @brief コンストラクタ
  ///
  /// 空のリストを作る．
  SearchPathList() = default;

  /// @brief 文字列からの変換コンストラクタ
  ///
  /// サーチパス文字列は ':' で区切られた文字列で '.'
  /// はカレントディレクトリを表す．
  /// また，末尾が '/' で終わっている場合にはそのサブディレクトリ
  /// も探索する．
  SearchPathList(
    const std::string& str ///< [in] サーチパスを表す文字列
  )
  {
    to_list(str, mList);
  }

  /// @brief コピーコンストラクタ
  SearchPathList(
    const SearchPathList& src ///< [in] 代入元のオブジェクト
  ) = default;

  /// @brief 代入演算子
  SearchPathList&
  operator=(
    const SearchPathList& src ///< [in] 代入元のオブジェクト
  ) = default;

  /// @brief デストラクタ
  ~SearchPathList() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 以前の内容をクリアして文字列をセットする．
  ///
  /// サーチパス文字列は ':' で区切られた文字列で '.'
  /// はカレントディレクトリを表す．
  /// また，末尾が '/' で終わっている場合にはそのサブディレクトリ
  /// も探索する．
  void
  set(
    const std::string& str ///< [in] セットするサーチパスを表す文字列
  )
  {
    mList.clear();
    to_list(str, mList);
  }

  /// @brief サーチパスの先頭に path を追加する．
  ///
  /// path は ':' を含んでいても良い
  void
  add_top(
    const std::string& path ///< [in] 追加するパス
  )
  {
    std::vector<PathName> tmp_list;
    to_list(path, tmp_list);
    mList.insert(mList.begin(), tmp_list.begin(), tmp_list.end());
  }

  /// @brief サーチパスの末尾に path を追加する．
  ///
  /// path は ':' を含んでいても良い
  void
  add_end(
    const std::string& path ///< [in] 追加するパス
  )
  {
    to_list(path, mList);
  }

  /// @brief サーチパスを考慮して filename を探す
  /// @retval 最初に見つかったファイルの full-path
  /// @retval 空のパス名 見つからない場合
  ///
  /// サーチパスが空ならカレントディレクトリで filename を探す．
  PathName
  search(
    const PathName& filename ///< [in] ファイル名
  ) const;

  /// @brief 現在のサーチパスを取り出す．
  /// @return サーチパスを文字列に変換したもの
  std::string
  to_string(
    const std::string& separator = ":" ///< [in] 区切り文字
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // 文字列を PathName のリストに変換する
  static
  void
  to_list(
    const std::string& str,
    std::vector<PathName>& pathname_list
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際のサーチパスのリスト
  std::vector<PathName> mList;

};

END_NAMESPACE_YM

#endif // YM_FILE_H
