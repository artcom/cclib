
#define BOOST_TEST_MODULE shaders 

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include <vector>
#include <string>
#include <stdexcept>

#include <gl/texture.h>

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
}
