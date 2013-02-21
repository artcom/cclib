
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
#include <base/property.h>
#include <particles/gpuattractor.h>

using namespace cclib;

BOOST_FIXTURE_TEST_CASE(testFixtureStatus, GL_Fixture)
{ 
    BOOST_CHECK(running);
    BOOST_TEST_MESSAGE("GL Fixture Window running");
    // BOOST_CHECK();
}

BOOST_FIXTURE_TEST_CASE(testComponentParameters, GL_Fixture)
{
    GPUAttractorPtr a = GPUAttractor::create(Vector3f(1,2,3), 4, 5);
    BOOST_CHECK(a->getPropertyByName("strength")->getValue<float>() == 4);
    BOOST_CHECK(a->getPropertyByName("radius")->getValue<float>() == 5);
    BOOST_CHECK(a->getPropertyByName("position")->getValue<Vector3f>() == Vector3f(1,2,3));
    
    BOOST_CHECK(a->getPropertyByName("position")->getValue<Vector3f>() == a->getPosition());
    BOOST_CHECK(a->getPropertyByName("strength")->getValue<float>() == a->getStrength());
    BOOST_CHECK(a->getPropertyByName("radius")->getValue<float>() == a->getRadius());
}

