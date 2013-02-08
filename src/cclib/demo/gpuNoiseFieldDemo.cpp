#include <cclib.h>
#include <GL/glfw.h>

#include <particles/gpunoise.h>
#include <stringified_shaders/simplex.fp.h>
#include <gl/shader.h>
#include <gl/mesh.h>
#include <gl/vbomesh.h>
#include <gl/graphics.h>

const char * noisefield_vp = "uniform float noiseScale = 1; \n"
                             "uniform float3 noiseOffset = float3(0,0,0);\n"
                             "uniform float4x4 modelViewProjectionMatrix: state.matrix.mvp;\n"
                             "\n"
                             "void main(\n"
                             "	in float4 iPosition	: POSITION,\n"
                             "	in float4 iColor		: COLOR,\n"
                             "	out float4 oPosition	: POSITION,\n"
                             "	out float4 ocolor		: COLOR\n"
                             ") { \n"
                             "\n"
                             "	float3 noiseOffset2 = noiseOffset.yzx;\n"
                             "	float myNoiseX = noise((iPosition).xyz * noiseScale + noiseOffset) + noise((iPosition).xyz * noiseScale + noiseOffset2);\n"
                             "	float myNoiseY = noise((iPosition).xyz * noiseScale + noiseOffset+100) + noise((iPosition).xyz * noiseScale + noiseOffset2+100);\n"
                             "	float myNoiseZ = noise((iPosition).xyz * noiseScale + noiseOffset+200) + noise((iPosition).xyz * noiseScale + noiseOffset2+200);\n"
                             "	\n"
                             "	iPosition += float4(myNoiseX,myNoiseY,myNoiseZ,0) * 30 * iColor.x;\n"
                             "	// Transform position from object space to clip space\n"
                             "  	oPosition = mul(modelViewProjectionMatrix, iPosition);\n"
                             "	ocolor = iColor; \n"
                             "}\n";     

using namespace cclib;

class NoiseFieldDemo {

    private:    
        ShaderPtr _myShader;
        CGparameter _myNoiseScaleParameter;
        CGparameter _myNoiseOffsetParameter;

        // Arcball _myArcball;
        VBOMeshPtr _myMesh;
        
        float time;
        Vector3fPtr _myOffset;
        Vector3fPtr _myOldOffset;
        Vector3fPtr _myNewOffset;
   	
    	float blend;
    	float max;

        int frame;
        

    public:
        bool running;
        
        NoiseFieldDemo() :
            frame(0), running(true)
        { 
            if(!glfwInit()) {
                running = false;
            }

            // Open an OpenGL window
            if(!glfwOpenWindow( 1400, 750, 
                        0, 0, 0, 0, 0, 0, GLFW_WINDOW )) {
                glfwTerminate();
                running = false;
            }

            GLenum err = glewInit();
            if(GLEW_OK != err){
                running = false;
            }
            
            setup();
        }

        void setup() {
            time = 0.0f;
            _myOffset = Vector3fPtr(new Vector3f());
            _myOldOffset = Vector3fPtr(new Vector3f());
            _myNewOffset = Vector3fPtr(new Vector3f(10.0f, 10.0f, 0.0f));
           
            blend = 0;
            max = 30;

            Graphics::clearColor(0);
            std::vector<std::string> vfiles;
            vfiles.push_back(simplex_fp);
            vfiles.push_back(noisefield_vp);
            
            std::vector<std::string> noFiles;
            
            _myShader = Shader::create(vfiles, noFiles);
            _myShader->load();

            _myNoiseScaleParameter = _myShader->vertexParameter("noiseScale");
            _myNoiseOffsetParameter = _myShader->vertexParameter("noiseOffset");
            GPUNoise::attachVertexNoise(_myShader);

            // _myArcball = new CCArcball(this);
            _myMesh = VBOMesh::create(GL_POINTS, 1000000);

            Graphics::pointSize(1);
            Graphics::strokeWeight(0.1f);
            Graphics::smooth();
        
            for(float x = -1000; x < 1000; x +=2){
                for(float y = -500; y < 500; y +=2){
                    _myMesh->addColor(0.2, 0, 0.1);
                    _myMesh->addVertex(x, y, -30);
                    _myMesh->addColor(0, 0, 1, 0.15f);
                    _myMesh->addVertex(x, y, 30);
                }
            }
            
        }

        ~NoiseFieldDemo() { 
            glfwTerminate();
        }

        void update(double theDeltaTime) {
            glPushMatrix();
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            int width, height = 0;
            glfwGetWindowSize(&width, &height);
            glViewport(0, 0, width, height);
            gluPerspective(60, (float)width/(float)height, 1, 10000);
            gluLookAt(0.0, 0.0, 650, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
            
            ///////

            time += theDeltaTime * 1;
            // if(time > max ){
            //     time -= max;
            //     _myOldOffset = _myNewOffset;
            //     _myNewOffset = CCVecMath.random(-10, 10, -10, 10, -10, 10);
            // }

            // blend = (CCMath.cos((time / max) * CCMath.PI + CCMath.PI) +1) / 2;
            // _myOffset = CCVecMath.blend(blend, _myOldOffset, _myNewOffset);
           
            // draw

            Graphics::clear();
            // _myArcball.draw(g);
            Graphics::color(1.0f, 1.0f, 1.0f, 0.05f);
            Graphics::noDepthTest();
            
            Graphics::blend(BLEND_MODE_ADD);
            _myShader->start();
            _myShader->parameter(_myNoiseScaleParameter, time/100.0f); // (1 - CCMath.abs((blend - 0.5f) * 2))*0.01f + 0.002f);
            _myShader->parameter(_myNoiseOffsetParameter, _myOffset);
            
            Graphics::rect(-width/2, -height/2, 20, 20);

            _myMesh->draw();
            _myShader->end();
            Graphics::noBlend();

            //
            glfwSwapBuffers();
            //if (++frame % 100 == 0) 
                std::cout << "frame: " << ++frame << std::endl;
            
            running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
            
            glPopMatrix();
        }
};


int main() {
    try {
        NoiseFieldDemo demo;

        while (demo.running) {
            demo.update(1.0/60.0);
        }
    } catch (Exception & e) {
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}

