
find_path(bgfx_INCLUDE_DIR bgfx.h
          HINTS "../bgfx/include")
find_path(bgfx_LIBRARY_DIR libbgfxRelease.a
          HINTS "../bgfx/.build/linux64_gcc/bin/")
set(bgfx_INCLUDE_DIRS ${bgfx_INCLUDE_DIR} )
set(bgfx_INCLUDE_DIRS "" )
mark_as_advanced(bgfx_INCLUDE_DIR bgfx_LIBRARY )
