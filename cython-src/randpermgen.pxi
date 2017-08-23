#
# @file randpermgen.pxi
# @brief randpermgen の cython インターフェイス
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2017 Yusuke Matsunaga
# All rights reserved.

from CXX_RandPermGen cimport RandPermGen as C_RandPermGen


# @brief RandPermGen クラスの python バージョン
cdef class RandPermGen :
    cdef C_RandPermGen* _thisptr

    # @brief コンストラクタ
    # @param[in] n 全要素数
    def __cinit__(RandPermGen self, unsigned int n) :
        self._thisptr = new C_RandPermGen(n)

    # @brief デストラクタ
    def __dealloc__(RandPermGen self) :
        if self._thisptr != NULL :
            del self._thisptr

    # @brief 全要素数を返す．
    @property
    def num(RandPermGen self) :
        return self._thisptr.num()

    # @brief ランダムな順列を生成する．
    # @param[in] randgen 乱数生成器
    def generate(RandPermGen self, RandGen randgen) :
        self._thisptr.generate(randgen._this)

    # @brief 順列の要素を取り出す．
    # @param[in] pos 要素の位置番号 ( 0 <= pos < num() )
    def elem(RandPermGen self, pos) :
        return self._thisptr.elem(pos)

    # @brief 順列の要素のリストを返す．
    #
    # 正確にはジェネレータを返す．
    def elem_list(RandPermGen self) :
        cdef int pos
        for pos in range(self.num()) :
            yield self._thisptr.elem(pos)
