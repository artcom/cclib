
#include "gpusortedparticlerenderer.h"
#include <gl/graphics.h>

#include "stringified_shaders/sorted_display_vertex.glsl.h"
#include "stringified_shaders/sorted_display_fragment.glsl.h"


cclib::GPUSortedParticleRenderer::GPUSortedParticleRenderer()
: cclib::GPUParticleRenderer(),
_myPointsize(1)
{
    printf("%s\n",__PRETTY_FUNCTION__);

	std::vector<std::string> vfiles, ffiles;

	vfiles.push_back(sorted_display_vertex_glsl);
	ffiles.push_back(sorted_display_fragment_glsl);

	_myShader = cclib::GLSLShader::create(vfiles, ffiles);

}

void cclib::GPUSortedParticleRenderer::setup(cclib::GPUParticlesPtr theParticles)
{
    printf("%s\n\tsize %d\n",__PRETTY_FUNCTION__, theParticles->size());

//     cclib::GPUIndexedParticleRenderer::setup(theParticles);

	_myParticles = theParticles;
    _myMesh = cclib::VBOMesh::create(GL_POINTS, _myParticles->size());
    _myMesh->prepareVertexData(_myParticles->size(), 3);

    for(int y = 0; y < theParticles->height();y++) {
        for(int x = 0; x < theParticles->width();x++) {
            _myMesh->addVertex(x,y,0);
            _myMesh->addTextureCoords(
                            0,
                            ((float)x + 0.5f) / theParticles->width(),
                            ((float)y + 0.5f) / theParticles->height()
                            );
        }
    }

	_mySort = cclib::GPUParticleSort::create(theParticles);

	// prepare color buffer
    _myColorBuffer = cclib::ShaderBuffer::create(_myParticles->width(), _myParticles->height()
                                                 //,CCTextureTarget.TEXTURE_RECT
                                                 );

    cclib::Graphics::clearColor(1,1,1,1);
    cclib::Graphics::color(1.0f, 1.0f, 1.0f, 1.0f);
    cclib::Graphics::clear();

    _myColorBuffer->beginDraw();
    cclib::Graphics::clearColor(1,1,1,1);
    cclib::Graphics::clear();

    glDisable(GL_LIGHTING);
    cclib::Graphics::beginShape(GL_POINTS);

    for (float x = 0; x < _myParticles->width(); x++) {
        for (float y = 0; y < _myParticles->height(); y++) {

            cclib::Color color;
            //color.setHSB( cclib::random(0.05f,0.15f), cclib::random(0.8f, 0.9f), cclib::random(0.f, 1.f) );
            color.setHSB( cclib::random(0.05f,0.15f), cclib::random(0.9f, 1.0f), 1);//cclib::random(0.6f, 1.f) );
            cclib::Graphics::color(color);

//                           cclib::Color::createFromHSB(
//                    cclib::random(0.05f,0.15f), cclib::random(0.8f, 0.9f), cclib::random(0.f, 1.f)
//                                   cclib::random(0.f,1.f), cclib::random(0.f,1.f), cclib::random(0.f,1.f),cclib::random(0.5f,1.f)
//                            )
//                                   );
            cclib::Graphics::vertex(x+0.5, y+0.5);
        }
    }

    cclib::Graphics::endShape();

    _myColorBuffer->endDraw();
}

void cclib::GPUSortedParticleRenderer::update(float theDeltaTime)
{
// 	printf(".");
    _mySort->update(theDeltaTime);
}

/*

 http://stackoverflow.com/questions/4427662/whats-the-relationship-between-field-of-view-and-lens-length
 http://www.watercolorpainting.com/perspective_1_2_3_point.htm

 You can however calculate the camera position in relation to the screen in whatever units you like (This was taught to me as the "Perspective Reference Point").

 Lets say the screen is 1 meter wide and the FOV is 90 degrees (PI/2 radians). Using basic trigonometry you know that

 tanHalfFOV = tan( fov / 2 ) = opposite/adjacent.

 You know opposite (as it is half a meter, ie half the screen)

 So to calculate adjacent (ie the distance from the screen to the camera position) you simply do:

 adjacent = opposite / tan( fov / 2 )


 OR:

 tan( fov / 2) = ymax/distance

 */

void cclib::GPUSortedParticleRenderer::draw()
{
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    _myShader->start();

    cclib::Graphics::texture(0, _myParticles->dataBuffer()->attachment(0));
    cclib::Graphics::texture(1, _myParticles->dataBuffer()->attachment(1));
    cclib::Graphics::texture(2, _mySort->indices()->attachment(0));
    cclib::Graphics::texture(3, _myColorBuffer->attachment(0));

    _myShader->setUniform1i("positions", 0);
    _myShader->setUniform1i("infos", 1);
    _myShader->setUniform1i("indices", 2);
    _myShader->setUniform1i("colors", 3);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float height = viewport[3];
    _myShader->setUniform1f("tanHalfFOV", (float)(tan(DEFAULT_FOV) * height));

    _myShader->setUniform1f("pointSize", _myPointsize);
   /* _myShader->setUniform1f("minPointSize", 0.0001);
    _myShader->setUniform1f("maxPointSize", 3);   */

    _myMesh->draw();

    cclib::Graphics::noTexture();
    _myShader->end();
    glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
}

void cclib::GPUSortedParticleRenderer::drawDebug()
{
	cclib::Graphics::pushAttribute();

	int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
	float w = viewport[2];
    float h= viewport[3];

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // glOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
    glOrtho(-w/2, w/2, -h/2, h/2, 0, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);

    glColor4f(1,1,1,1);

    float size = 200;
    glTranslatef(-(w/4),(h/4),0);

	cclib::Graphics::image(_myParticles->dataBuffer()->attachment(0),	size,0,size,size); // position
    cclib::Graphics::image(_myParticles->dataBuffer()->attachment(1),	size,-size,size,size); // info
    cclib::Graphics::image(_mySort->indices()->attachment(0),			size,-(2*size),size,size); // indices
    cclib::Graphics::image(_myColorBuffer->attachment(0),				size,-(3*size),size,size); // color

// 	cclib::Graphics::image(_mySort->distances()->attachment(0),       size*2,-(2*size),size,size); // distances

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

	cclib::Graphics::popAttribute();
}