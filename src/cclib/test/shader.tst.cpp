
#define BOOST_TEST_MODULE shaders 

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include <vector>
#include <string>

#include <gl/shader.h>

#include "gl_fixture.h"

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
    std::vector<std::string> vfiles;
    // vfiles.push_back(std::string("\n"));
    std::vector<std::string> ffiles;
    // ffiles.push_back(std::string("\n"));
    
    Shader::Ptr shader = Shader::createShader(vfiles, ffiles); 
}

