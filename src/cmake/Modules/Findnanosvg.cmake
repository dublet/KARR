
find_path(nanosvg_INCLUDE_DIR nanosvg.h
          HINTS "../nanosvg/src")
set(nanosvg_INCLUDE_DIRS ${nanosvg_INCLUDE_DIR} )
set(nanosvg_INCLUDE_DIRS "" )
mark_as_advanced(nanosvg_INCLUDE_DIR nanosvg_LIBRARY )
