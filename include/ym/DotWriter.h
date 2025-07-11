#ifndef DOTWRITER_H
#define DOTWRITER_H

/// @file DotWriter.h
/// @brief DotWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class DotWriter DotWriter.h "ym/DotWriter.h"
/// @brief dot 形式の出力を行うクラス
//////////////////////////////////////////////////////////////////////
class DotWriter
{
  // 属性リストを表す型
  using AttrList = std::unordered_map<std::string, std::string>;

public:

  /// @brief コンストラクタ
  DotWriter(
    std::ostream& s ///< [in] 出力ストリーム
  ) : mS{s}
  {
  }

  /// @brief デストラクタ
  ~DotWriter() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief グラフの定義の開始
  void
  graph_begin(
    const std::string& graph_type, ///< [in] グラフの種類
    const std::string& graph_name, ///< [in] グラフ名
    const AttrList& attr_list      ///< [in] 属性リスト
  );

  /// @brief グラフの定義の終了
  void
  graph_end();

  /// @brief ノードの定義
  void
  write_node(
    const std::string& node,  ///< [in] ノード名
    const AttrList& attr_list ///< [in] 属性リスト
  );

  /// @brief 枝の定義
  void
  write_edge(
    std::string from_node,    ///< [in] 始点のノード名
    std::string to_node,      ///< [in] 終点のノード名
    const AttrList& attr_list ///< [in] 属性リスト
  );

  /// @brief ランクグループの定義
  void
  write_rank_group(
    const std::vector<std::string>& node_list, ///< [in] 同一グループのノードリスト
    const std::string& rank = "same"           ///< [in] ランク属性
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性リストを出力する．
  void
  write_attr(
    const AttrList& attr_list ///< [in] 属性リスト
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力ストリーム
  std::ostream& mS;

};

END_NAMESPACE_YM

#endif // DOTWRITER_H
