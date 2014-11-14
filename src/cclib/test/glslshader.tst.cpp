
#define BOOST_TEST_MODULE shaders 

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include <vector>
#include <string>
#include <stdexcept>

#include <cclib.h>
#include <gl/shader.h>
#include <gl/glslshader.h>

#include "gl_fixture.h"

#include "stringified_shaders/bloom_vert.glsl.cpp"
#include "stringified_shaders/bloom_frag.glsl.cpp"

using namespace cclib;

BOOST_FIXTURE_TEST_CASE(testFixtureStatus, GL_Fixture)
{ 
    BOOST_CHECK(running);
    BOOST_TEST_MESSAGE("GL Fixture Window running");
    //BOOST_CHECK();
}

BOOST_FIXTURE_TEST_CASE(testShaderConstruction, GL_Fixture)
{
    BOOST_TEST_MESSAGE(">> it should work with no shaders");
    std::vector<std::string> vfiles;
    std::vector<std::string> ffiles;
    GLSLShaderPtr shader = GLSLShader::create(vfiles, ffiles);

    BOOST_TEST_MESSAGE(">> it should throw an exception for syntax errors");
    vfiles.push_back(std::string("Syntax error? \n Wait, this is not even a shader!"));
    
    BOOST_CHECK_THROW(GLSLShader::create(vfiles, ffiles), std::exception);
}

BOOST_FIXTURE_TEST_CASE(testShaderUniform, GL_Fixture)
{
    std::vector<std::string> vfiles;
    std::vector<std::string> ffiles;
    
    BOOST_TEST_MESSAGE(">> it should compile correct shaders without complaining");
    ffiles.push_back(std::string(
        "uniform float testValue;\n" \
        "void main(void) \n" \
        "{\n" \
        "  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n" 
        " } "));
    
    vfiles.push_back(std::string(
         "void main(void) \n" \
         "{\n" \
         "  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
         " } "));
    GLSLShaderPtr compiled = GLSLShader::create(vfiles, ffiles);
    BOOST_CHECK(compiled != GLSLShaderPtr(NULL));
    
    compiled->start();
    compiled->setUniform1f("testValue", 99.f);
    compiled->end();
    
}

#define COMPILE_FP(x) vfiles.clear(); ffiles.clear();  \
    BOOST_CHECK_NO_THROW( ffiles.push_back(x); CGShader::create(vfiles, ffiles) ); 
#define COMPILE_VP(x) vfiles.clear(); ffiles.clear();  \
    BOOST_CHECK_NO_THROW( vfiles.push_back(x); CGShader::create(vfiles, ffiles) ); 

BOOST_FIXTURE_TEST_CASE(testRealShaderCompilation, GL_Fixture)
{
    BOOST_TEST_MESSAGE(">> it should compile the stringified shaders");
    std::vector<std::string> vfiles;
    std::vector<std::string> ffiles;
    
    vfiles.push_back(std::string(bloom_vert_glsl));
    ffiles.push_back(std::string(bloom_frag_glsl));
    
    BOOST_CHECK_NO_THROW(GLSLShader::create(vfiles, ffiles, "main", "main") ); // entry point: lookup position
}

//BOOST_FIXTURE_TEST_CASE(testCombinedShaderCompilation, GL_Fixture)
//{
//    BOOST_TEST_MESSAGE(">> it should compile the combined shaders");
//    std::vector<std::string> vfiles;
//    std::vector<std::string> ffiles;
//    
//    ffiles.push_back(initvalue_fp);
//    ffiles.push_back(simplex_fp);
//    vfiles.push_back(contour_vp);
//    
//    CGShaderPtr shader;
//    BOOST_CHECK_NO_THROW( shader = CGShader::create(vfiles, ffiles, "main", "main") ); 
//    BOOST_CHECK_NO_THROW( shader->load() );
//}


// BOOST_FIXTURE_TEST_CASE(testGPUUpdateShader, GL_Fixture)
// {
//     BOOST_TEST_MESSAGE(">> it should compile the combined shaders");
//     std::vector<std::string> vfiles;
//     std::vector<std::string> ffiles;
//     
//     CGShaderPtr shader;
//     ffiles.push_back(std::string(simplex_fp));
//     ffiles.push_back(std::string(forces_fp));
//     ffiles.push_back(std::string(constraints_fp));
//     ffiles.push_back(std::string(impulses_fp));
//     ffiles.push_back(std::string(velocity_fp));
//     ffiles.push_back(std::string(
//                                  "float4 main(void) : COLOR \n" \
//                                  "{\n" \
//                                  "    return float4(0.6, 1.0, 0.0, 1.0);\n"
//                                  " } "));
//     
//     BOOST_CHECK_NO_THROW(shader = CGShader::create(vfiles, ffiles));
//     /* shader->fragmentParameter("positionTexture");
//     shader->fragmentParameter("infoTexture");
//     shader->fragmentParameter("velocityTexture");
//     shader->fragmentParameter("deltaTime"); */
//     shader->load();
//     shader->end();
// }

// BOOST_FIXTURE_TEST_CASE(testShaderStartStop, GL_Fixture)
// {
//     std::vector<std::string> vfiles;
//     std::vector<std::string> ffiles;
//     
//     ffiles.push_back(initvalue_fp);
//     vfiles.push_back(contour_vp);
//     
//    //  CGShaderPtr shader;
//    //  BOOST_CHECK_NO_THROW( shader = CGShader::create(vfiles, ffiles, "main", "main") ); 
//    //  BOOST_CHECK_NO_THROW( shader->load() );
//    //  BOOST_CHECK_NO_THROW( shader->start() );
//    //  BOOST_CHECK_NO_THROW( shader->end() );
// }
