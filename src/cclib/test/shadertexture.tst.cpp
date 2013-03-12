
#define BOOST_TEST_MODULE shaderbuffer 

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include <vector>
#include <string>
#include <stdexcept>

#include <cclib.h>
#include <gl/shader.h>
#include <gl/shaderbuffer.h>
#include <gl/graphics.h>

#include "gl_fixture.h"

using namespace cclib;

BOOST_FIXTURE_TEST_CASE(testFixtureStatus, GL_Fixture)
{ 
    BOOST_CHECK(running);
    BOOST_TEST_MESSAGE("GL Fixture Window running");
}

BOOST_FIXTURE_TEST_CASE(testShaderTexture, GL_Fixture)
{
    ShaderBufferPtr s;
    BOOST_CHECK_NO_THROW(s = ShaderBuffer::create(123, 456));
    BOOST_CHECK(s->width() == 123);
    BOOST_CHECK(s->height() == 456);
    // BOOST_CHECK_THROW(s->attachment(1), std::exception);
    BOOST_CHECK(s->attachment(0)->target() == GL_TEXTURE_RECTANGLE);
    
    BOOST_CHECK(s->checkStatus());
    BOOST_CHECK(s->numberOfBits() == 32);
    BOOST_CHECK(s->numberOfChannels() == 3);
    
    BOOST_CHECK_NO_THROW(s->beginDraw());
            
    Graphics::clearColor(0.1f, 0.2f, 0.3f, 1.0f);
    Graphics::clear();
    Graphics::color(0.4f, 0.5f, 0.6f, 1.0f); // drawing a single blue pixel into the texture
    Graphics::rect(1, 0, 1, 1);
            
    BOOST_CHECK_NO_THROW(s->endDraw());
    std::vector<float> data;
    BOOST_CHECK_NO_THROW(data = s->getData(0));
    BOOST_CHECK(data.size() == s->width() * s->height() * 3); 
    
    BOOST_CHECK(data[0] == 0.1f);
    BOOST_CHECK(data[1] == 0.2f);
    BOOST_CHECK(data[2] == 0.3f); 
    
    BOOST_CHECK(data[3] == 0.4f); 
    BOOST_CHECK(data[4] == 0.5f); 
    BOOST_CHECK(data[5] == 0.6f); 
    
    BOOST_CHECK(data[6] == 0.1f);
    BOOST_CHECK(data[7] == 0.2f);
    BOOST_CHECK(data[8] == 0.3f); 
}

