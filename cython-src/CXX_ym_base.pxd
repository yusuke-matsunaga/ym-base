#
# @file CXX_ym_base.pxd
# @brief ym_base-cython 用の定義ファイル
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2016 Yusuke Matsunaga
# All rights reserved.


cdef extern from "ym/RandGen.h" namespace "nsYm" :

    # RandGen クラスの cython バージョン
    cdef cppclass RandGen :
        RandGen()
        void init(unsigned int seed)
        void init_by_array(unsigned int* init_key, unsigned int key_length)
        unsigned int int32()
        int int31()
        unsigned long uint64()
        double real1()
        double real2()
        double real3()
        double res53()


cdef extern from "ym/RandCombiGen.h" namespace "nsYm" :

    # RandCombiGen クラスの cython バージョン
    cdef cppclass RandCombiGen :
        RandCombiGen(unsigned int n, unsigned int k)
        unsigned int num()
        unsigned int combi_num()
        void generate(RandGen& rg)
        unsigned int elem(unsigned int pos)


cdef extern from "ym/RandPermGen.h" namespace "nsYm" :

    # RandPermGen クラスの cython バージョン
    cdef cppclass RandPermGen :
        RandPermGen(unsigned int n)
        unsigned int num()
        void generate(RandGen& rg)
        unsigned int elem(unsigned int pos)


cdef extern from "ym/RandSampler.h" namespace "nsYm" :

    # RandSampler クラスの cython バージョン
    cdef cppclass RandSampler :
        RandSampler(unsigned int num, unsigned int* weight_array)
        unsigned int num()
        unsigned int weight(unsigned int)
        unsigned int get_sample(RandGen& rg)
