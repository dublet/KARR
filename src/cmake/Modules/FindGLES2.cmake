# Part of Project KARR
# (c), released under BSD license

find_path(GLES2_INCLUDE_DIR GLES2/gl2.h
          HINTS "/usr/include")
find_library(GLESV2_LIBRARY NAMES GLESv2 HINTS "/usr/lib")
find_library(EGL_LIBRARY NAMES EGL HINTS "/usr/lib")
find_library(X11_LIBRARY NAMES X11 HINTS "/usr/lib")
set(GLES2_INCLUDE_DIRS ${GLES2_INCLUDE_DIR} )
set(GLES2_LIBRARIES ${GLESV2_LIBRARY} ${EGL_LIBRARY} ${X11_LIBRARY} )
mark_as_advanced(GLES2_INCLUDE_DIR GLES2_LIBRARIES )
