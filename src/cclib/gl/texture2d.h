#ifndef __CCLIB_TEXTURE2D_INCLUDED__
#define __CCLIB_TEXTURE2D_INCLUDED__

#include <cclib.h>
#include <gl/texture.h>

namespace cclib {

class Color {
    public:
        typedef std::tr1::shared_ptr<Color> Ptr;
        Color(unsigned int red=255, unsigned int green=255, unsigned int blue=255, unsigned int alpha=255) 
            : _red(red/255.0f), _green(green/255.0f), _blue(blue/255.0f), _alpha(alpha/255.0f) {};

        Color(float red, float green, float blue, float alpha = 1.0) 
            : _red(red), _green(green), _blue(blue), _alpha() {};

        ~Color() {};  

        float red() { return _red; };
        float green() { return _green; };
        float blue() { return _blue; };
        float alpha() { return _alpha; };
        
        float r() { return _red; };
        float g() { return _green; };
        float b() { return _blue; };
        float a() { return _alpha; };
        
        void red(float value) { _red = value; };
        void green(float value) { _green = value; };
        void blue(float value) { _blue = value; };
        void alpha(float value) { _alpha = value; };

        void set(Color theColor) {
            _red   = theColor.r();
            _green = theColor.g();
            _blue  = theColor.b();
            _alpha = theColor.a();
        };

    private:
        float _red;
        float _green;
        float _blue;
        float _alpha;
};

class Texture2D : public Texture {

    public:
        virtual ~Texture2D() {
        };

        static Texture2DPtr create(TextureAttributesPtr attributes, int width=0, int height=0,
                unsigned int numberOfTextures=1, GLenum target=GL_TEXTURE_2D);
    
        static Texture2DPtr create(std::vector<unsigned char> theData, GLenum theTarget,
                unsigned int theWidth, unsigned int theHeight);
    
	    Color getPixel(int x, int y);
        void setPixel(int x, int y, const Color & color);
        void dataImplementation(const std::vector<unsigned char> & theData);
    
    protected:
        virtual void allocateData(unsigned int width, unsigned int height);
        Texture2D(TextureAttributesPtr attributes,
                int width, int height, unsigned int numberOfTextures, GLenum target);

	    // private CCBufferObject _myBufferObject;
};

}; // namespace
	
#endif // includeguard
