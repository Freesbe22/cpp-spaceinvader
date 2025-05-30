# CMakeList.txt : CMake project for SpaceInvader, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("SpaceInvader")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glew REQUIRED)

# Add source to this project's executable.
add_executable (SpaceInvader "SpaceInvader.cpp" "SpaceInvader.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SpaceInvader PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
add_definitions(-DGLEW_STATIC)
target_include_directories(SpaceInvader PRIVATE dependencies)
target_link_libraries(SpaceInvader ${GLFW3_LIBRARY} ${GLEW_LIBRARY} OpenGL::GL)