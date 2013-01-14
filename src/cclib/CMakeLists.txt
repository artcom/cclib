
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/usr/local/include")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/usr/local/lib")

set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(Boost COMPONENTS unit_test_framework OPTIONAL)

if (Boost_UNIT_TEST_FRAMEWORK_LIBRARY)
    message("found boost ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}" )
    set(BUILD_TESTS TRUE)
    include_directories(${Boost_INCLUDE_DIRS})
    include_directories(".")
endif(Boost_UNIT_TEST_FRAMEWORK_LIBRARY)


# set(SOURCES 
# )
# 
# set(HEADERS 
#     math/vec2.h
# )
# 
# set(CCLIB_NAME cclib)
# 
# add_library(${CCLIB_NAME} SHARED ${SOURCES} ${HEADERS})
# target_link_libraries(${CCLIB_NAME} ${Boost_LIBRARIES})
# install(TARGETS ${CCLIB_NAME} DESTINATION bin)

# install(FILES ${HEADERS} DESTINATION include/${CCLIB_NAME})

set(BUILD_LIBRARIES ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}) # ${CCLIB_NAME} 

# XXX add automatic test detection

add_executable(vec2 test/vec2.tst.cpp)
target_link_libraries(vec2 ${CCLIB_NAME} ${BUILD_LIBRARIES})
add_test(vec2 vec2) 

add_executable(vec3 test/vec3.tst.cpp)
target_link_libraries(vec3 ${CCLIB_NAME} ${BUILD_LIBRARIES})
add_test(vec3 vec3) 

