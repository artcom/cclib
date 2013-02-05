
#define BOOST_TEST_MODULE shaders 

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include <vector>
#include <string>
#include <stdexcept>

#include <cclib.h>
#include <gl/shader.h>
#include <gl/shadertexture.h>
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
    ShaderTexturePtr s;
    BOOST_CHECK_NO_THROW(s = ShaderTexture::create(123, 456));
    BOOST_CHECK(s->width() == 123);
    BOOST_CHECK(s->height() == 456);
    BOOST_CHECK(s->target() == GL_TEXTURE_RECTANGLE);
    
    BOOST_CHECK(s->checkStatus());
    BOOST_CHECK(s->numberOfBits() == 32);
    BOOST_CHECK(s->numberOfChannels() == 3);
    
    BOOST_CHECK_NO_THROW(s->beginDraw());
            
    Graphics::clearColor(1.0f, 0.0f, 0.0f, 1.0f);
    Graphics::clear();
    Graphics::color(1.0f, 0.0f, 0.0f, 1.0f); // drawing red into the texture
    Graphics::rect(-200, -200, 50, 50);
            
    BOOST_CHECK_NO_THROW(s->endDraw());
    BOOST_CHECK_NO_THROW(s->getPixel(1, 1));
}

