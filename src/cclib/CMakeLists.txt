
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})
INCLUDE(EqStringifyShaders)

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/usr/local/include")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/usr/local/lib")

set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_RUNTIME    OFF)


# look for GLEW
find_package(GLEW)
if(NOT GLEW_FOUND)
    message(ERROR "GLEW could not be found.")
else()
    message(STATUS "GLEW found here: ${GLEW_LIBRARY}")
    include_directories(${GLEW_INCLUDE_PATH})
endif()

# look for GLFW
find_package(GLFW)
if(NOT GLFW_LIBRARIES)
    message(ERROR " GLFW could not be found.")
else()
    message(STATUS "GLFW found here: ${GLFW_LIBRARIES}")
    include_directories(${GLFW_INCLUDE_DIR})
endif()

# look for Cg
find_package(Cg)
if(NOT Cg_FOUND)
    message(ERROR "Cg could not be found.")
else()
    message(STATUS "Cg found here (libs): ${Cg_LIBRARIES}")
    include_directories({Cg_LIBRARIES})
endif()

# look for boost unit test framework
find_package(Boost COMPONENTS unit_test_framework OPTIONAL)
if (Boost_UNIT_TEST_FRAMEWORK_LIBRARY)
    set(BUILD_TESTS TRUE)
    include_directories(${Boost_INCLUDE_DIRS})
    include_directories(".")
endif(Boost_UNIT_TEST_FRAMEWORK_LIBRARY)

# look for OpenGL 
find_package(OpenGL REQUIRED)

# stringify all shader files
FILE(GLOB_RECURSE SHADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/shaders" FALSE "*.?p")

MESSAGE(STATUS "stringifying shaders...")
FOREACH(shader ${SHADERS})
    STRINGIFY_SHADERS("${shader}")
ENDFOREACH(shader)
MESSAGE(STATUS "  ...done.")

# add stringified shaders to library
set(SHADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/stringified_shaders")
FILE(GLOB_RECURSE SHADER_SOURCES ABSOLUTE ${SHADER_PATH} *.fp.cpp *.vp.cpp)

set(SOURCES 
    gl/shader.cpp    
    gl/texture.cpp    
    gl/texture2D.cpp    
    gl/pixelstoragemodes.cpp
    gl/framebufferobject.cpp
)
 
set(HEADERS 
    math/vec2.h
    math/vec3.h
    gl/shader.h
    gl/texture.h    
    gl/texture2D.h    
    gl/pixelstoragemodes.h
    gl/framebufferobject.h
)
 
set(CCLIB_NAME cclib)
add_library(${CCLIB_NAME} SHARED 
    ${SHADER_SOURCES} 
    ${SOURCES} 
    ${HEADERS})

target_link_libraries(${CCLIB_NAME} ${GLEW_LIBRARY} ${Cg_LIBRARIES} ${OPENGL_LIBRARIES})
install(TARGETS ${CCLIB_NAME} DESTINATION bin)

# install(FILES ${HEADERS} DESTINATION include/${CCLIB_NAME})

set(TEST_LIBRARIES ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${Cg_LIBRARIES} ${GLEW_LIBRARY} ${GLFW_LIBRARIES}) # ${CCLIB_NAME} 

# XXX add automatic test detection

add_executable(vec2 test/vec2.tst.cpp)
target_link_libraries(vec2 ${CCLIB_NAME} ${TEST_LIBRARIES})
add_test(vec2 vec2) 

add_executable(vec3 test/vec3.tst.cpp)
target_link_libraries(vec3 ${CCLIB_NAME} ${TEST_LIBRARIES})
add_test(vec3 vec3) 

add_executable(shader_test test/shader.tst.cpp)
target_link_libraries(shader_test ${CCLIB_NAME} ${TEST_LIBRARIES})
add_test(shader_test shader_test) 

