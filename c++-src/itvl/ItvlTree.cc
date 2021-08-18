
/// @file ItvlTree.cc
/// @brief ItvlTree の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ItvlTree.h"


BEGIN_NAMESPACE_YM

// @brief コンストラクタ
ItvlTree::ItvlTree()
{
  clear();
}

// @brief デストラクタ
ItvlTree::~ItvlTree()
{
  delete mRoot;
}

// @brief 内容を初期化する．
void
ItvlTree::clear()
{
  delete mRoot;
  // [-1, -1] の区間を番兵として登録しておく
  auto node = new Node{-1, -1};
  insert_node(node, mRoot);
}

// @brief 追加する．
void
ItvlTree::add(
  int x ///< [in] 追加する値
)
{
  // x の左側のノードを見つける．
  auto left = find_left(x);
  int left_e = x - 1;
  if ( left != nullptr ) {
    left_e = left->E;
  }

  // x の右側のノードを見つける．
  auto right = find_right(x);
  int right_s = x + 1;
  if ( right != nullptr ) {
    right_s = right->S;
  }

  if ( left_e == x ) {
    if ( right_s == x ) {
      // left と right の隙間がぴったり埋まった．
      int right_e = right->E;
      // right を削除して left の終端を変更する．
      remove_node(right, mRoot);
      // left は再平衡化の結果変わっている可能性がある．
      left = find_left(x);
      left->E = right_e;
    }
    else {
      // left の右端に x を足す．
      left->E = x;
    }
  }
  else if ( right_s == x ) {
    // right の左端に x を足す．
    right->S = x;
  }
  else {
    // 新しい区間を追加する．
    auto node = new Node{x, x, 0};
    insert_node(node, mRoot);
  }
}

// @brief 削除する．
void
ItvlTree::del(
  int x ///< [in] 削除する値
)
{
  // x を含む区間を探す．
  auto node = find(x);
  ASSERT_COND( node != nullptr );

  if ( node->S == x ) {
    if ( node->E == x ) {
      // この node を削除する．
      remove_node(node, mRoot);
    }
    else {
      // node の左端が x
      node->S = x + 1;
    }
  }
  else if ( node->E == x ) {
    // node の右端が x
    node->E = x - 1;
  }
  else {
    // 2つの区間に分割する．
    auto node1 = new Node{x + 1, node->E};
    node->E = x - 1;
    insert_node(node1, mRoot);
  }
}

// @brief 使用されていない値を取り出す．
int
ItvlTree::get_min() const
{
  // 最も左端にあるノードを見つける．
  auto node = mRoot;
  for ( ; node->L != nullptr; node = node->L ) {
    ;
  }
  return node->E + 1;
}

ItvlTree::Node::~Node()
{
  delete L;
  delete R;
}

// @brief x を含むノードを探す．
//
// なければ nullptr を返す．
ItvlTree::Node*
ItvlTree::find(
  int x
) const
{
  auto node = mRoot;
  while ( node != nullptr ) {
    if ( x < node->S ) {
      node = node->L;
    }
    else if ( node->E < x ) {
      node = node->R;
    }
    else {
      break;
    }
  }
  return node;
}

/// @brief x よりも小さく最も右にあるノードを探す．
ItvlTree::Node*
ItvlTree::find_left(
  int x
) const
{
  // 大まかには find() と同様の処理を行なう．
  // その際に ptr->E < x のノードを ans に記録しておく．
  // 答は node ではなく ans
  auto node = mRoot;
  Node* ans = nullptr;
  while ( node != nullptr ) {
    if ( node->E < x ) {
      ans = node;
    }
    if ( x < node->S ) {
      node = node->L;
    }
    else if ( node->E < x ) {
      node = node->R;
    }
    else {
      break;
    }
  }
  return ans;
}

// @brief x よりも大きく最も左にあるノードを探す．
ItvlTree::Node*
ItvlTree::find_right(
  int x
) const
{
  // 大まかには find() と同様の処理を行なう．
  // その際に x < ptr->S のノードを ans に記録しておく．
  // 答は node ではなく ans
  auto node = mRoot;
  Node* ans = nullptr;
  while ( node != nullptr ) {
    if ( x < node->S ) {
      ans = node;
    }
    if ( x < node->S ) {
      node = node->L;
    }
    else if ( node->E < x ) {
      node = node->R;
    }
    else {
      break;
    }
  }
  return ans;
}

// @brief 区間を追加する．
bool
ItvlTree::insert_node(
  Node* node,
  Node*& ptr
)
{
  if ( ptr == nullptr ) {
    // この位置にノードを挿入する．
    ptr = node;
    // もちろｎ高さは1増える．
    return true;
  }

  if ( node->E < ptr->S ) {
    // 左側に挿入する．
    bool chg = insert_node(node, ptr->L);
    if ( !chg ) {
      // 高さが変わらなかった
      return false;
    }
    // 左側の部分木の高さが1増えた．
    ptr->B -= 1;
    switch ( ptr->B ) {
    case  0: return false; // ちょうどバランスした．高さは変わらない．
    case -1: return true;  // 左が高くなった．部分木全体も高くなった．
    }
    // ptr->B は -2 のはず．再平衡化が必要
    auto left = ptr->L;
    if ( left->B == -1 ) {
      // 左部分木の左が1高い
      //             単LL回転
      //      ptr               left
      //     /   \              /   \
      //   left   c            a    ptr
      //   /   \                   /   \
      //  a     b                 b     c
      //
      // h(a) + 1 = h(c) + 2
      // h(a) = h(b) + 1
      ptr->L = left->R;
      left->R = ptr;
      ptr->B = 0;
      ptr = left;
    }
    else {
      // 左部分木は平衡か右が高い
      //             二重LR回転
      //      ptr                  right
      //     /	 \                 /    \
      //   left   d              left   ptr
      //   /   \                 /  \   /  \
      //  a   right             a    b c    d
      //      /   \
      //     b     c
      auto right = left->R;
      left->R = right->L;
      ptr->L = right->R;
      right->L = left;
      right->R = ptr;
      if ( right->B == -1 ) {
	ptr->B = 1;
      }
      else {
	ptr->B = 0;
      }
      if ( right->B == 1 ) {
	left->B = -1;
      }
      else {
	left->B = 0;
      }
      ptr = right;
    }
    ptr->B = 0;
    return false;
  }
  else if ( ptr->E < node->S ) {
    // 右側に挿入する．
    bool chg = insert_node(node, ptr->R);
    if ( !chg ) {
      // 高さが変わらなかった
      return false;
    }

    // 右の部分木が高くなった．
    ptr->B += 1;
    switch ( ptr->B ) {
    case 0: return false; // ちょうどバランスした．高さも変わらない．
    case 1: return true; // 右が高くなった．部分木全体も高くなった．
    }
    // ここに来た時は ptr->B = -2．再平衡化が必要
    auto right = ptr->R;
    if ( right->B == 1 ) {
      // 単RR回転
      ptr->R = right->L;
      right->L = ptr;
      ptr->B = 0;
      ptr = right;
    }
    else {
      // 二重RL回転
      auto left = right->L;
      right->L = left->R;
      ptr->R = left->L;
      left->R = right;
      left->L = ptr;
      if ( left->B == 1 ) {
	ptr->B = -1;
      }
      else {
	ptr->B = 0;
      }
      if ( left->B == -1 ) {
	right->B = 1;
      }
      else {
	right->B = 0;
      }
      ptr = left;
    }
    ptr->B = 0;
    return false;
  }
  else {
    // 既存の区間の中に x が入っていた．
    abort();
  }
  return false;
}

// @brief ノードを削除する．
/// @return 部分木の高さが変わった時に true を返す．
bool
ItvlTree::remove_node(
  Node* node, ///< [in] 削除するノード
  Node*& ptr  ///< [in] 親のノード
)
{
  ASSERT_COND( ptr != nullptr );

  if ( node->E < ptr->S ) {
    // node は左部分木にある．
    bool chg = remove_node(node, ptr->L);
    if ( chg ) {
      chg = balance_left(ptr);
    }
    return chg;
  }
  else if ( ptr->E < node->S ) {
    // node は右部分木にある．
    bool chg = remove_node(node, ptr->R);
    if ( chg ) {
      chg = balance_right(ptr);
    }
    return chg;
  }
  else {
    // この場合，ptr と node の区間がオーバーラップしているので
    // ptr == node のはず
    ASSERT_COND( ptr == node );
    // node を削除する．
    if ( ptr->L == nullptr ) {
      // 右の子供しか持たない場合
      // node と右の子供を入れ替えて node を削除する．
      // 高さは1減る．
      ptr = node->R;
      node->R = nullptr;
      delete node;
      return true;
    }
    else if ( ptr->R == nullptr ) {
      // 左の子供しか持たない場合
      // node と左の子供を入れ替えて node を削除する．
      // 高さは1減る．
      ptr = node->L;
      node->L = nullptr;
      delete node;
      return true;
    }
    else {
      // 2人の子供を持つ場合
      // 左の部分木の最も右にあるノードで置き換える．
      bool chg = remove_right(node, ptr->L);
      if ( chg ) {
	chg = balance_left(ptr);
      }
      return chg;
    }
  }
}

// @brief 右隣のノードと位置を入れ替えて削除する．
// @return 部分木の高さが変わった時に true を返す．
bool
ItvlTree::remove_right(
  Node* node,
  Node*& ptr
)
{
  if ( ptr->R != nullptr ) {
    // まだ右に子供がいる．
    bool chg = remove_right(node, ptr->R);
    if ( chg ) {
      // 再平衡化を行なう．
      chg = balance_right(ptr);
    }
    return chg;
  }

  // ptr が最右端
  node->S = ptr->S;
  node->E = ptr->E;

  auto dnode = ptr;
  ptr = ptr->L;
  dnode->L = nullptr;
  delete node;
  // 高さは1減る．
  return true;
}

/// @brief 左の部分木の高さが減少したときの処理
/// @return 自分自身の高さも減少する時に true を返す．
bool
ItvlTree::balance_left(
  Node*& ptr
)
{
  ptr->B += 1;
  switch ( ptr->B ) {
  case 0: // もともと左が高かったのでバランスした．
    // ただし高さは1減少している．
    return true;
  case 1: // もともとバランスしていたので左が低くなった．
    // 右の高さが変わっていないので全体の高さは変わらない．
    return false;
  }

  // ptr->B == 2 のはず
  // 再平衡化が必要
  bool chg = false;
  auto right = ptr->R;
  auto rb = right->B;
  if ( rb != -1 ) {
    // 右部分木は平衡か右が高い
    // - h(c) = h(b) + alpha (alpha = 0 or 1)
    // - h(a) + 2 = h(right) = h(c) + 1
    //             単RR回転
    //        ptr            right
    //       /	 \           /   \
    //      a   right      ptr    c
    //          /   \     /   \
    //         b     c   a     b
    ptr->R = right->L;
    right->L = ptr;
    if ( rb == 0 ) {
      // alpha = 0
      // h(a) + 2 = h(b) + 1
      // h(ptr) = h(b) + 1 > h(c)
      // 変換前の h(ptr) = h(b) + 2
      // 変換後の h(ptr) = h(b) + 2
      ptr->B = 1;
      right->B = -1;
    }
    else {
      // alpha = 1
      // h(a) + 2 = h(right) = h(b) + 2
      // h(ptr) = h(a) + 1 = h(c)
      // 変換前の h(ptr) = h(c) + 2
      // 変換後の h(right) = h(c) + 1
      // 高さは1減少した．
      ptr->B = 0;
      right->B = 0;
      chg = true;
    }
    ptr = right;
  }
  else {
    // 右部分木は左が高い
    // - h(left) = h(d) + 1
    // - h(right) = h(left) + 1 = h(a) + 2
    //          二重RL回転
    //      ptr              left
    //     /   \             /	 \
    //    a   right        ptr   right
    //        /   \       /   \  /   \
    //     left    d     a    b c    d
    //     /   \
    //    b     c
    auto left = right->L;
    auto lb = left->B;
    right->L = left->R;
    left->R = right;
    ptr->R = left->L;
    left->L = ptr;
    if ( lb == 1 ) {
      ptr->B = -1;
    }
    else {
      ptr->B = 0;
    }
    if ( lb == -1 ) {
      right->B = 1;
    }
    else {
      right->B = 0;
    }
    ptr = left;
    ptr->B = 0;
    chg = true;
  }
  return chg;
}

// @brief 右の部分木の高さが減少したときの処理
// @return 自分自身の高さも減少する時に true を返す．
bool
ItvlTree::balance_right(
  Node*& ptr
)
{
  ptr->B -= 1;
  switch ( ptr->B ) {
  case 0: // もともと右が高かったのでバンランスした．
    // でも高さは減少した．
    return true;
  case -1: // もともとバランスしていたので右が低くなった．
    // 高さは変わらない．
    return false;
  }

  // ptr->B == -2 再平衡化が必要．
  // 左部分木の高さが2つ高い
  bool chg = false;
  auto left = ptr->L;
  auto lb = left->B;
  if ( lb != 1 ) {
    // 左部分木は平衡か左が高い
    // - h(a) = h(b) + alpha (alpha = 0 or 1)
    // - h(a) + 1 = h(c) + 2
    //             単LL回転
    //        ptr            left
    //       /	 \           /  \
    //     left   c   ==>   a   ptr
    //     /  \                /   \
    //    a    b              b     c
    ptr->L = left->R;
    left->R = ptr;
    if ( lb == 0 ) {
      // alpha = 0 の場合
      // h(a) = h(b) = h(c) + 1
      // 変換前の h(ptr) = h(b) + 2
      // 変換後の h(ptr) = h(b) + 2
      // 高さは変わっていない．
      ptr->B = -1;
      left->B = 1;
    }
    else {
      // alpha = 1 の場合
      // h(a) = h(b) + 1 = h(c) + 1
      // 変換前の h(ptr) = h(a) + 2
      // 変換後の h(left) = h(b) + 2 = h(a) + 1
      // 高さは1減少した．
      ptr->B = 0;
      left->B = 0;
      chg = true;
    }
    ptr = left;
  }
  else {
    // 左部分木は右が高い
    // - h(a) + 1 = h(right)
    // - h(right) + 1 = h(d) + 2
    //            二重LR回転
    //      ptr             right
    //     /   \           /    \
    //   left   d        left   ptr
    //   /  \            /  \   /  \
    //  a  right        a    b c    d
    //     /   \
    //    b     c
    auto right = left->R;
    auto rb = right->B;
    left->R = right->L;
    right->L = left;
    ptr->L = right->R;
    right->R = ptr;
    if ( rb == -1 ) {
      // h(b) + 1 = h(right)
      // h(c) + 1 = h(b)
      ptr->B = 1;
    }
    else {
      // h(c) + 1 = h(right)
      ptr->B = 0;
    }
    if ( rb == 1 ) {
      // h(b) + 2 = h(right) = h(a) + 1
      left->B = -1;
    }
    else {
      // h(b) + 1 = h(right) = h(a) + 1
      left->B = 0;
    }
    ptr = right;
    ptr->B = 0;
    // 変換前の h(ptr) = h(b) + 3
    // 変換後の h(ptr) = h(b) + 2
    chg = true;
  }
  return chg;
}

BEGIN_NONAMESPACE

END_NONAMESPACE

// @brief 内容を出力する．
void
ItvlTree::print(
  ostream& s ///< [in] 出力先のストリーム
) const
{
  print_sub(mRoot, s, true);
}

bool
ItvlTree::print_sub(
  Node* node,
  ostream& s,
  bool first
)
{
  if ( node == nullptr ) {
    return first;
  }
  first = print_sub(node->L, s, first);
  if ( !first ) {
    s << ", ";
  }
  s << "[" << node->S << ", " << node->E << "]";
  print_sub(node->R, s, false);
  return false;
}

END_NAMESPACE_YM
