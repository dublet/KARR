
find_path(nanovg_INCLUDE_DIR nanovg.h
          HINTS "../nanovg/src")
set(nanovg_INCLUDE_DIRS ${nanovg_INCLUDE_DIR} )
set(nanovg_INCLUDE_DIRS "" )
mark_as_advanced(nanovg_INCLUDE_DIR nanovg_LIBRARY )
