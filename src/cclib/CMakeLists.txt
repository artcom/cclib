CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})
INCLUDE(EqStringifyShaders)

SET(CMAKE_INCLUDE_PATH "${CMAKE_INCLUDE_PATH}:/usr/local/include")
SET(CMAKE_INCLUDE_PATH "${CMAKE_INCLUDE_PATH}:${CMAKE_CURRENT_SOURCE_DIR}")
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

# look for Cg
find_package(Cg)
if(NOT Cg_FOUND)
    message(ERROR "Cg could not be found.")
else()
    message(STATUS "Cg found here (libs): ${Cg_LIBRARIES}")
endif()

# look for GLFW
find_package(GLFW)
if(NOT GLFW_LIBRARIES)
    message(ERROR " GLFW could not be found.")
else()
    message(STATUS "GLFW found here: ${GLFW_LIBRARIES}")
    include_directories(${GLFW_INCLUDE_DIR})
endif()

# look for boost unit test framework
find_package(Boost COMPONENTS unit_test_framework OPTIONAL)
if (Boost_UNIT_TEST_FRAMEWORK_LIBRARY)
    set(BUILD_TESTS TRUE)
    include_directories(${Boost_INCLUDE_DIRS})
endif(Boost_UNIT_TEST_FRAMEWORK_LIBRARY)
    
include_directories(".")

# look for OpenGL 
find_package(OpenGL REQUIRED)

# stringify all shader files
FILE(GLOB_RECURSE SHADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/shaders" FALSE "*.?p" || "*.glsl")

MESSAGE(STATUS "stringifying shaders...")
FOREACH(shader ${SHADERS})
    STRINGIFY_SHADERS("${shader}")
ENDFOREACH(shader)
MESSAGE(STATUS "  ...done.")

# add stringified shaders to library
set(SHADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/stringified_shaders")
FILE(GLOB_RECURSE SHADER_SOURCES ABSOLUTE ${SHADER_PATH} *.fp.cpp *.vp.cpp *.glsl.cpp)


set(SOURCES 
    math/simplexnoise.cpp
    math/math.cpp
    base/component.cpp
    gl/shader.cpp    
    gl/cgshader.cpp    
    gl/glslshader.cpp
    gl/texture.cpp
    gl/texture2d.cpp
    gl/pixelstoragemodes.cpp
    gl/framebufferobject.cpp
    gl/shaderbuffer.cpp
    gl/renderbuffer.cpp
    gl/camera.cpp
    gl/graphics.cpp
    gl/mesh.cpp
    gl/bufferobject.cpp
    gl/vbomesh.cpp
    gl/geometrybuffer.cpp
    gl/convolutionshader.cpp
    gl/separategaussianblur.cpp
    gl/gpubloom.cpp
    particles/gpuupdateshader.cpp
    particles/gpuforce.cpp
    particles/gpuconstraint.cpp
    particles/gpuimpulse.cpp
    particles/gpunoise.cpp
    particles/gpuparticles.cpp
    particles/gpuparticle.cpp
    particles/gpuparticlesort.cpp
    particles/gpuindexparticleemitter.cpp
    particles/gpusimpleemitter.cpp
    particles/gpucurvelineemitter.cpp
    particles/gpuindexparticlecurveemitter.cpp
    particles/gpuparticlepointrenderer.cpp
    particles/gpuindexedparticlerenderer.cpp
    particles/gpudisplayshader.cpp
    particles/gpuforcefield.cpp
    particles/gpucurvefield.cpp    
    particles/gpucurveline.cpp    
    particles/gpugravity.cpp    
    particles/gpuattractor.cpp    
    particles/gpuviscousdrag.cpp    
    particles/gpucombinedforce.cpp        
    particles/gputargetforce.cpp        
    particles/gputimeforceblend.cpp 
    particles/gpuyforceblend.cpp 
    # particles/gpubloom.cpp
    particles/gpuyconstraint.cpp
    particles/gpuplaneconstraint.cpp
    particles/gpusortedparticlerenderer.cpp
)
 
set(HEADERS
    cclib.h
    base/property.h
    base/component.h
    math/vec2.h
    math/vec3.h
    math/math.h
    math/simplexnoise.h
    math/plane3.h
    math/matrix.h
    gl/shader.h
    gl/cgshader.h
    gl/glslshader.h
    gl/texture.h    
    gl/texture2d.h    
    gl/pixelstoragemodes.h
    gl/framebufferobject.h
    gl/shaderbuffer.h
    gl/renderbuffer.h
    gl/camera.h
    gl/graphics.h
    gl/mesh.h
    gl/bufferobject.h
    gl/vbomesh.h
    gl/geometrybuffer.h
    gl/convolutionshader.h
    gl/separategaussianblur.h
    gl/gpubloom.h
    particles/gpuupdateshader.h
    particles/gpuforce.h
    particles/gpuconstraint.h
    particles/gpuimpulse.h
    particles/gpunoise.h    
    particles/gpuparticles.h
    particles/gpuparticle.h
    particles/gpuparticlesort.h
    particles/gpusimpleemitter.h
    particles/gpucurvelineemitter.h
    particles/gpuindexparticleemitter.h
    particles/gpuindexparticlecurveemitter.h
    particles/gpuparticlerenderer.h
    particles/gpuparticlepointrenderer.h
    particles/gpuindexedparticlerenderer.h
    particles/gpudisplayshader.h
    particles/gpuforcefield.h
    particles/gpucurvefield.h
    particles/gpucurveline.h
    particles/gpugravity.h   
    particles/gpuattractor.h    
    particles/gpuviscousdrag.h    
    particles/gpuparticleemitter.h        
    particles/gpucombinedforce.h        
    particles/gputargetforce.h        
    particles/gputimeforceblend.h        
    particles/gpuyforceblend.h        
    particles/gputargetsetup.h
    # particles/gpubloom.h
    particles/gpuyconstraint.h
    particles/gpuplaneconstraint.h
    particles/gpusortedparticlerenderer.h
)

MESSAGE("Stringify shaders")
FOREACH(HEADER ${HEADERS})
    STRING(REGEX MATCH "(.*)[/\\]" DIR ${HEADER})
    INSTALL(FILES ${HEADER} DESTINATION include/cclib/${DIR})
ENDFOREACH(HEADER)

# install shaders
INSTALL(DIRECTORY ${SHADER_PATH} DESTINATION include/cclib/ PATTERN "*.cpp" EXCLUDE)

# build dynamic library for development
set(CCLIB_NAME cclib)
add_library(${CCLIB_NAME} SHARED 
    ${SHADER_SOURCES} 
    ${SOURCES} 
    ${HEADERS})

target_link_libraries(${CCLIB_NAME} ${GLEW_LIBRARY} ${Cg_LIBRARIES} ${Cg_GL_LIBRARIES} ${OPENGL_LIBRARIES})
install(TARGETS ${CCLIB_NAME} 
	DESTINATION lib
	PERMISSIONS
            OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
	) 


# build osx bundle for the unity plugin
set(CCLIB_BUNDLE_NAME cclib_unity)
add_library(${CCLIB_BUNDLE_NAME} MODULE 
    ${SHADER_SOURCES}  
    ${SOURCES} 
    capi/capi.cpp
    capi/particleswrapper.cpp
)

set_target_properties(${CCLIB_BUNDLE_NAME} PROPERTIES 
    MACOSX_BUNDLE TRUE
    SUFFIX .bundle
    PREFIX ""  
)

target_link_libraries(${CCLIB_BUNDLE_NAME} ${GLEW_LIBRARY} ${Cg_LIBRARIES} ${Cg_GL_LIBRARIES} ${OPENGL_LIBRARIES})
install(TARGETS ${CCLIB_BUNDLE_NAME} 
	DESTINATION bin 
	PERMISSIONS
        OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
) 


set(TEST_LIBRARIES ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${Cg_LIBRARIES} ${GLEW_LIBRARY} ${GLFW_LIBRARIES}) # ${CCLIB_NAME} 

if (Boost_UNIT_TEST_FRAMEWORK_LIBRARY AND GLFW_LIBRARIES)
	add_executable(vec2 test/vec2.tst.cpp)
	target_link_libraries(vec2 ${CCLIB_NAME} ${TEST_LIBRARIES})
	add_test(vec2 vec2) 

	add_executable(vec3 test/vec3.tst.cpp)
	target_link_libraries(vec3 ${CCLIB_NAME} ${TEST_LIBRARIES})
	add_test(vec3 vec3) 

	add_executable(cg_shader_test test/cgshader.tst.cpp)
	target_link_libraries(cg_shader_test ${CCLIB_NAME} ${TEST_LIBRARIES})
	add_test(cg_shader_test cg_shader_test) 

	add_executable(glsl_shader_test test/glslshader.tst.cpp)
	target_link_libraries(glsl_shader_test ${CCLIB_NAME} ${TEST_LIBRARIES})
	add_test(glsl_shader_test glsl_shader_test) 

	add_executable(shaderbuffer_test test/shaderbuffer.tst.cpp)
	target_link_libraries(shaderbuffer_test ${CCLIB_NAME} ${TEST_LIBRARIES})
	add_test(shaderbuffer_test shaderbuffer_test)

	add_executable(components_test test/components.tst.cpp)
	target_link_libraries(components_test ${CCLIB_NAME} ${TEST_LIBRARIES})
	add_test(components_test components_test) 
else()
	message("tests are skipped because Boost_UNIT_TEST_FRAMEWORK_LIBRARY and/or GLFW_LIBRARIES are missing")
endif()

# samples
if (GLFW_LIBRARIES)
	add_executable(cclibEmitDemo demo/emitDemo.cpp)
	target_link_libraries(cclibEmitDemo ${CCLIB_NAME} ${TEST_LIBRARIES})

	add_executable(cclibCurveFlowField demo/curveFlowFieldDemo.cpp)
	target_link_libraries(cclibCurveFlowField ${CCLIB_NAME} ${TEST_LIBRARIES})

	add_executable(cclibShaderBufferDemo demo/shaderBufferDemo.cpp)
	target_link_libraries(cclibShaderBufferDemo ${CCLIB_NAME} ${TEST_LIBRARIES})

	add_executable(cclibNoiseFieldDemo demo/gpuNoiseFieldDemo.cpp)
	target_link_libraries(cclibNoiseFieldDemo ${CCLIB_NAME} ${TEST_LIBRARIES})

	add_executable(cclibFragmentNoiseDemo demo/fragmentNoise.cpp)
	target_link_libraries(cclibFragmentNoiseDemo ${CCLIB_NAME} ${TEST_LIBRARIES})

	add_executable(cclibTargetsDemo demo/targets.cpp)
	target_link_libraries(cclibTargetsDemo ${CCLIB_NAME} ${TEST_LIBRARIES})
	
    add_executable(cclibYForceBlendDemo demo/yForceBlendDemo.cpp)
    target_link_libraries(cclibYForceBlendDemo ${CCLIB_NAME} ${TEST_LIBRARIES})
else()
	message("sampels are skipped because GLFW_LIBRARIES are missing")
endif()
