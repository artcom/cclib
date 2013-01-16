
#define BOOST_TEST_MODULE shaders 

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include <vector>
#include <string>
#include <stdexcept>

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
    Shader::Ptr shader = Shader::createShader(vfiles, ffiles); 

    BOOST_TEST_MESSAGE(">> it should throw an exception for syntax errors");
    vfiles.push_back(std::string("Syntax error? \n Wait, this is not even a shader!"));
    BOOST_CHECK_THROW(Shader::createShader(vfiles, ffiles), std::exception);
    vfiles.clear();
    
    BOOST_TEST_MESSAGE(">> it should compile correct shaders without complaining");
    ffiles.push_back(std::string(
        "float4 frag(void) : COLOR \n" \
        "{\n" \
        "    return float4(0.6, 1.0, 0.0, 1.0);\n" 
        " } "));
    Shader::Ptr compiled = Shader::createShader(vfiles, ffiles); 
    BOOST_CHECK(compiled != NULL);
}

#define COMPILE_FP(x) vfiles.clear(); ffiles.clear();  \
    BOOST_CHECK_NO_THROW( ffiles.push_back(x); Shader::createShader(vfiles, ffiles) ); 
#define COMPILE_VP(x) vfiles.clear(); ffiles.clear();  \
    BOOST_CHECK_NO_THROW( vfiles.push_back(x); Shader::createShader(vfiles, ffiles) ); 

BOOST_FIXTURE_TEST_CASE(testRealShaderCompilation, GL_Fixture)
{
    BOOST_TEST_MESSAGE(">> it should compile the stringified shaders");
    std::vector<std::string> vfiles;
    std::vector<std::string> ffiles;
    
    COMPILE_FP(contour_fp)
    COMPILE_FP(forces_fp)
    // COMPILE_FP(impulse_fp) // shader broken
    COMPILE_FP(impulses_fp)
    COMPILE_FP(initvalue_fp)
    COMPILE_FP(initvalue01_fp)
    COMPILE_FP(nearestTarget_fp)
    COMPILE_FP(nearestTargetChange_fp)
    COMPILE_FP(simplex_fp)
    COMPILE_FP(springs_fp)
    COMPILE_FP(timedtextureblend_fp)
    COMPILE_FP(velocity_fp)
    COMPILE_FP(computeDistance_fp)
    COMPILE_FP(initSortIndex_fp)
    COMPILE_FP(lookupPosition_fp)
    COMPILE_FP(mergeSortEnd_fp)
    COMPILE_FP(mergeSortRecursion_fp)
    COMPILE_FP(curvefield_emit_fp)
    COMPILE_FP(simple_shader_emit_fp)
   
    // 
    COMPILE_VP(nearestTargetChange_vp)
    COMPILE_VP(contour_vp)
}

BOOST_FIXTURE_TEST_CASE(testCombinedShaderCompilation, GL_Fixture)
{
    BOOST_TEST_MESSAGE(">> it should compile the combined shaders");
    std::vector<std::string> vfiles;
    std::vector<std::string> ffiles;
    
    ffiles.push_back(forces_fp);
    ffiles.push_back(velocity_fp);
    
    vfiles.push_back(nearestTargetChange_vp);
    vfiles.push_back(contour_vp);
    
    BOOST_CHECK_NO_THROW( Shader::createShader(vfiles, ffiles) ); 
    
}


