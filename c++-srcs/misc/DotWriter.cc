
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
  const string& graph_type,
  const string& graph_name,
  const unordered_map<string, string>& attr_list
)
{
  mS << graph_type << " "
     << graph_name << " {" << endl
     << "  graph";
  write_attr(attr_list);
}

// @brief グラフの定義の終了
void
DotWriter::graph_end()
{
  mS << "}" << endl;
}

// @brief ノードの定義
void
DotWriter::write_node(
  const string& node,
  const unordered_map<string, string>& attr_list
)
{
  mS << "  "
     << node;
  write_attr(attr_list);
 }

// @brief 枝の定義
void
DotWriter::write_edge(
  string from_node,
  string to_node,
  const unordered_map<string, string>& attr_list
)
{
  mS << "  " << from_node << " -> " << to_node;
  write_attr(attr_list);
}

// @brief ランクグループの定義
void
DotWriter::write_rank_group(
  const vector<string>& node_list,
  const string& rank
)
{
  mS << "  { rank = " << rank << ";";
  for ( auto node: node_list ) {
    mS << " " << node << ";";
  }
  mS << "}" << endl;
}

// @brief 属性リストを出力する．
void
DotWriter::write_attr(
  const unordered_map<string, string>& attr_list
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
  mS << "]" << endl;
}

END_NAMESPACE_YM
