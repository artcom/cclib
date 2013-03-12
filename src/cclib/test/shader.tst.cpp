
#define BOOST_TEST_MODULE shaders 

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include <vector>
#include <string>
#include <stdexcept>

#include <cclib.h>
#include <gl/shader.h>

#include "gl_fixture.h"

#include "stringified_shaders/constraints.fp.h"
#include "stringified_shaders/contour.fp.h"
#include "stringified_shaders/contour.vp.h"
#include "stringified_shaders/forces.fp.h"
#include "stringified_shaders/impulse.fp.h"
#include "stringified_shaders/impulses.fp.h"
#include "stringified_shaders/initvalue.fp.h"
#include "stringified_shaders/initvalue01.fp.h"
#include "stringified_shaders/nearestTarget.fp.h"
#include "stringified_shaders/nearestTargetChange.fp.h"
#include "stringified_shaders/nearestTargetChange.vp.h"
#include "stringified_shaders/simplex.fp.h"
#include "stringified_shaders/springs.fp.h"
#include "stringified_shaders/timedtextureblend.fp.h"
#include "stringified_shaders/velocity.fp.h"
#include "stringified_shaders/sort/computeDistance.fp.h"
#include "stringified_shaders/sort/initSortIndex.fp.h"
#include "stringified_shaders/sort/lookupPosition.fp.h"
#include "stringified_shaders/sort/mergeSortEnd.fp.h"
#include "stringified_shaders/sort/mergeSortRecursion.fp.h"
#include "stringified_shaders/emit/curvefield_emit.fp.h"
#include "stringified_shaders/emit/simple_shader_emit.fp.h"

using namespace cclib;

BOOST_FIXTURE_TEST_CASE(testFixtureStatus, GL_Fixture)
{ 
    BOOST_CHECK(running);
    BOOST_TEST_MESSAGE("GL Fixture Window running");
    // BOOST_CHECK();
}

BOOST_FIXTURE_TEST_CASE(testShaderCombineSources, GL_Fixture)
{
    std::vector<std::string> files;
    files.push_back(std::string("line1"));
    files.push_back(std::string("line2"));
   
    std::string result = Shader::combineSources(files);
    std::string desired = std::string("line1\nline2\n");
    BOOST_CHECK(result.size() == 12);
    BOOST_CHECK(result.compare(desired) == 0);
}

BOOST_FIXTURE_TEST_CASE(testShaderConstruction, GL_Fixture)
{
    BOOST_TEST_MESSAGE(">> it should work with no shaders");
    std::vector<std::string> vfiles;
    std::vector<std::string> ffiles;
    ShaderPtr shader = Shader::create(vfiles, ffiles); 

    BOOST_TEST_MESSAGE(">> it should throw an exception for syntax errors");
    vfiles.push_back(std::string("Syntax error? \n Wait, this is not even a shader!"));
    BOOST_CHECK_THROW(Shader::create(vfiles, ffiles), std::exception);
    vfiles.clear();
    
    BOOST_TEST_MESSAGE(">> it should compile correct shaders without complaining");
    ffiles.push_back(std::string(
        "float4 main(void) : COLOR \n" \
        "{\n" \
        "    return float4(0.6, 1.0, 0.0, 1.0);\n" 
        " } "));
    ShaderPtr compiled = Shader::create(vfiles, ffiles); 
    BOOST_CHECK(compiled != NULL);
}

#define COMPILE_FP(x) vfiles.clear(); ffiles.clear();  \
    BOOST_CHECK_NO_THROW( ffiles.push_back(x); Shader::create(vfiles, ffiles) ); 
#define COMPILE_VP(x) vfiles.clear(); ffiles.clear();  \
    BOOST_CHECK_NO_THROW( vfiles.push_back(x); Shader::create(vfiles, ffiles) ); 

BOOST_FIXTURE_TEST_CASE(testRealShaderCompilation, GL_Fixture)
{
    BOOST_TEST_MESSAGE(">> it should compile the stringified shaders");
    std::vector<std::string> vfiles;
    std::vector<std::string> ffiles;
    
    COMPILE_FP(contour_fp)
    // COMPILE_FP(forces_fp) // library. no entry point 
    // COMPILE_FP(impulse_fp) // shader broken
    // COMPILE_FP(impulses_fp)
    COMPILE_FP(initvalue_fp)
    COMPILE_FP(initvalue01_fp)
    COMPILE_FP(nearestTarget_fp)
    COMPILE_FP(nearestTargetChange_fp)
    COMPILE_FP(springs_fp)
    COMPILE_FP(timedtextureblend_fp)
    // COMPILE_FP(velocity_fp) // library. no entry point 
    COMPILE_FP(computeDistance_fp)
    COMPILE_FP(initSortIndex_fp)
    COMPILE_FP(mergeSortEnd_fp)
    COMPILE_FP(mergeSortRecursion_fp)
    // COMPILE_FP(curvefield_emit_fp) // library. no entry point 
    COMPILE_FP(simple_shader_emit_fp)
   
    // 
    COMPILE_VP(nearestTargetChange_vp)
    COMPILE_VP(contour_vp)
   
    BOOST_TEST_MESSAGE(">>>> with defined entry point");
    vfiles.clear(); ffiles.clear();
    ffiles.push_back(lookupPosition_fp);
    BOOST_CHECK_NO_THROW(Shader::create(vfiles, ffiles, "main", "main") ); // entry point: lookup position 
}

BOOST_FIXTURE_TEST_CASE(testCombinedShaderCompilation, GL_Fixture)
{
    BOOST_TEST_MESSAGE(">> it should compile the combined shaders");
    std::vector<std::string> vfiles;
    std::vector<std::string> ffiles;
    
    ffiles.push_back(initvalue_fp);
    ffiles.push_back(simplex_fp);
    vfiles.push_back(contour_vp);
    
    ShaderPtr shader;
    BOOST_CHECK_NO_THROW( shader = Shader::create(vfiles, ffiles, "main", "main") ); 
    BOOST_CHECK_NO_THROW( shader->load() );
}


// BOOST_FIXTURE_TEST_CASE(testGPUUpdateShader, GL_Fixture)
// {
//     BOOST_TEST_MESSAGE(">> it should compile the combined shaders");
//     std::vector<std::string> vfiles;
//     std::vector<std::string> ffiles;
//     
//     ShaderPtr shader;
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
//     BOOST_CHECK_NO_THROW(shader = Shader::create(vfiles, ffiles));
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
//    //  ShaderPtr shader;
//    //  BOOST_CHECK_NO_THROW( shader = Shader::create(vfiles, ffiles, "main", "main") ); 
//    //  BOOST_CHECK_NO_THROW( shader->load() );
//    //  BOOST_CHECK_NO_THROW( shader->start() );
//    //  BOOST_CHECK_NO_THROW( shader->end() );
// }
