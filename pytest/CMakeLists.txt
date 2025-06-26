
# ===================================================================
# パッケージの検査
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${Python3_INCLUDE_DIRS}
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set ( py_ymbase_SOURCES
  py_ymbase.cc
  )


# ===================================================================
#  executable ターゲットの設定
# ===================================================================

add_executable( py_ymbase
  ${py_ymbase_SOURCES}
  $<TARGET_OBJECTS:py_ymbase_obj>
  $<TARGET_OBJECTS:ym_base_obj>
  )

target_compile_options( py_ymbase
  PRIVATE "-O3"
  )

target_compile_definitions( py_ymbase
  PRIVATE "-DNODEBUG"
  )

target_link_libraries ( py_ymbase
  ${YM_LIB_DEPENDS}
  ${Python3_LIBRARIES}
  )

add_executable( py_ymbase_d
  ${py_ymbase_SOURCES}
  $<TARGET_OBJECTS:py_ymbase_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

target_compile_options( py_ymbase_d
  PRIVATE "-g"
  )

target_link_libraries ( py_ymbase_d
  ${YM_LIB_DEPENDS}
  ${Python3_LIBRARIES}
  )


# ===================================================================
# テストターゲット
# ===================================================================

add_test( base_py_json_test
  py_ymbase -m pytest ${CMAKE_CURRENT_SOURCE_DIR}/json_test.py
  )
