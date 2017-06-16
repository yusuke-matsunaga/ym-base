# @file randgen.pxi
# @brief RandGen の cython ソース
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2015, 2016, 2017 Yusuke Matsunaga
# All rights reserved.


from CXX_RandGen cimport RandGen as C_RandGen


cdef extern from "stdlib.h" :
    void* malloc(size_t size)
    void free(void* ptr)


# @brief RandGen クラスの python バージョン
cdef class RandGen :
    cdef C_RandGen _this

    # @brief (再)初期化
    # @param[in] seed 乱数のシード
    def init(RandGen self, unsigned int seed) :
        self._this.init(seed)

    # @brief 配列を用いた(再)初期化
    # @param[in] init_key 初期値の配列
    # @param[in] key_length init_key の要素数
    def init_by_array(RandGen self, list init_key) :
        cdef int n = len(init_key)
        cdef unsigned int* c_init_key = <unsigned int*>malloc(sizeof(unsigned int) * n)
        cdef int i = 0
        cdef unsigned int key
        for key in init_key :
            c_init_key[i] = key
            i += 1
        self._this.init_by_array(c_init_key, n)
        free(c_init_key)

    # @brief 符号なし32ビット整数の乱数を発生する．
    def int32(RandGen self) :
        return self._this.int32()

    # @brief 符号付き31ビット整数の乱数を発生する．
    def int31(RandGen self) :
        return self._this.int31()

    # @brief 符号なし64ビット整数の乱数を発生する．
    def uint64(RandGen self) :
        return self._this.uint64()

    # @brief [0,1]の区間の実数の乱数を発生させる．
    def real1(RandGen self) :
        return self._this.real1()

    # @brief [0,1)の区間の実数の乱数を発生させる．
    def real2(RandGen self) :
        return self._this.real2()

    # @brief (0,1)の区間の実数の乱数を発生させる．
    def real3(RandGen self) :
        return self._this.real3()

    # @brief [0,1)の区間の53ビットの実数の乱数を発生させる．
    def res53(RandGen self) :
        return self._this.res53()
