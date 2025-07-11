
/// @file DotWriter.cc
/// @brief DotWriter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/DotWriter.h"


BEGIN_NAMESPACE_YM

// @brief グラフの定義の開始
void
DotWriter::graph_begin(
  const std::string& graph_type,
  const std::string& graph_name,
  const AttrList&  attr_list
)
{
  mS << graph_type << " "
     << graph_name << " {" << std::endl
     << "  graph";
  write_attr(attr_list);
}

// @brief グラフの定義の終了
void
DotWriter::graph_end()
{
  mS << "}" << std::endl;
}

// @brief ノードの定義
void
DotWriter::write_node(
  const std::string& node,
  const AttrList& attr_list
)
{
  mS << "  "
     << node;
  write_attr(attr_list);
 }

// @brief 枝の定義
void
DotWriter::write_edge(
  std::string from_node,
  std::string to_node,
  const  AttrList& attr_list
)
{
  mS << "  " << from_node << " -> " << to_node;
  write_attr(attr_list);
}

// @brief ランクグループの定義
void
DotWriter::write_rank_group(
  const std::vector<std::string>& node_list,
  const std::string& rank
)
{
  mS << "  { rank = " << rank << ";";
  for ( auto node: node_list ) {
    mS << " " << node << ";";
  }
  mS << "}" << std::endl;
}

// @brief 属性リストを出力する．
void
DotWriter::write_attr(
  const AttrList& attr_list
)
{
  if ( attr_list.empty() ) {
    return;
  }
  mS << " [";
  const char* comma = "";
  for ( auto& p: attr_list ) {
    auto attr_name = p.first;
    auto attr_val = p.second;
    mS << comma;
    comma = ", ";
    mS << attr_name
       << " = "
       << attr_val;
  }
  mS << "]" << std::endl;
}

END_NAMESPACE_YM
