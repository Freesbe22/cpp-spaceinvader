set(_GLEW_HEADER_SEARCH_DIRS
  "${CMAKE_SOURCE_DIR}/dependencies/GL/include"
)

set(_GLEW_LIB_SEARCH_DIRS
  "${CMAKE_SOURCE_DIR}/dependencies/GL/lib")

find_path(GLEW_INCLUDE_DIR GL/glew.h PATHS ${_GLEW_HEADER_SEARCH_DIRS})
find_library(GLEW_LIBRARY_RELEASE NAMES glew32s PATHS ${_GLEW_LIB_SEARCH_DIRS})
find_library(GLEW_LIBRARY_DEBUG NAMES glew32sd PATHS ${_GLEW_LIB_SEARCH_DIRS})

include(SelectLibraryConfigurations)
select_library_configurations(GLEW)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(glew DEFAULT_MSG GLEW_LIBRARY GLEW_INCLUDE_DIR)

mark_as_advanced(GLEW_INCLUDE_DIR GLEW_LIBRARY)
