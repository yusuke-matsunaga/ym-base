
# release, relwithdeb, debug ターゲットライブラリの設定
#
# USAGE: ym_add_object_library ( <target-name>
#                         <source-file> [<source-file>]
#                       )
function (ym_add_object_library)
  foreach ( pos RANGE 0 ${ARGC} )
    if ( ${pos} EQUAL ${ARGC} )
      break()
    endif ()
    list (GET ARGV ${pos} argv)
    if ( ${pos} EQUAL 0 )
      # ターゲット名の設定
      # - 最適化モード
      # - プロファイルモード (_p)
      # - デバッグモード (_d)
      # の3つがある．
      set (_target_name "${argv}")
      set (_target_name_p "${argv}_p")
      set (_target_name_d "${argv}_d")
    else ()
      list (APPEND _sources ${argv})
    endif ()
  endforeach ()

  # release モードのターゲット
  add_library (${_target_name}
    OBJECT
    ${_sources}
    )

  target_compile_options (${_target_name}
    PRIVATE "-O3" "-fPIC"
    )

  target_compile_definitions (${_target_name}
    PRIVATE "-DNDEBUG"
    )

  # release with debug info モードのターゲット
  add_library (${_target_name_p}
    OBJECT
    ${_sources}
    )

  target_compile_options (${_target_name_p}
    PRIVATE "-O2" "-g" "-fPIC"
    )

  target_compile_definitions (${_target_name_p}
    PRIVATE "-DNDEBUG"
    )

  # debug モードのターゲット
  # 常に static にする．
  add_library (${_target_name_d}
    OBJECT
    ${_sources}
    )

  target_compile_options (${_target_name_d}
    PRIVATE "-g"
    )

  target_compile_definitions (${_target_name_d}
    PRIVATE
    )

endfunction ()
