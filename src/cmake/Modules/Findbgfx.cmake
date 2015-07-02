# Part of Project KARR
# (c), released under BSD license

find_path(bgfx_INCLUDE_DIR bgfx.h
          HINTS "../bgfx/include")
find_library(bgfx_LIBRARY NAMES bgfxRelease
          HINTS "../bgfx/.build/linux64_gcc/bin/")
set(bgfx_INCLUDE_DIRS ${bgfx_INCLUDE_DIR} )
set(bgfx_LIBRARIES ${bgfx_LIBRARY} )
mark_as_advanced(bgfx_INCLUDE_DIR bgfx_LIBRARY )
