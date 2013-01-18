#ifndef __CCLIB_TEXTURE_INCLUDED__
#define __CCLIB_TEXTURE_INCLUDED__

#include <iostream>
#include <string>
#include <vector>
#include <tr1/memory>
#include <GL/glew.h>

#include <math/vec2.h>
#include <math/vec3.h>

namespace cclib {

struct TextureAttributes {

    GLenum wrapS;
    GLenum wrapT;
    GLenum filter; 
    GLenum mipmapFilter;
    bool generateMipmaps;
    GLenum internalFormat;
    GLenum format;
    GLenum type;

    TextureAttributes() : 
        wrapS(GL_CLAMP_TO_EDGE),
        wrapT(GL_CLAMP_TO_EDGE),
        filter(GL_LINEAR),
        mipmapFilter(GL_LINEAR),
        generateMipmaps(false),
        internalFormat(GL_RGBA),
        type(GL_RGBA),
        format(GL_FLOAT) 
    {};
};

class Texture {

    public:
        typedef std::tr1::shared_ptr<Texture> Ptr;
        virtual ~Texture(); 
        
        Texture::Ptr createTexture(GLuint target, const TextureAttributes & attributes, unsigned int numberOfTextures = 1); 
        
        // virtual void dataImplementation(CCTextureData theData);
        // virtual void updateData(final CCTextureData theData);
        
        void generateMipmaps(const bool & generateMipmaps);
        bool & generateMipmaps(); 
        
        bool & isCompressed();
        // void data(CCTextureData data);

        std::vector<GLuint> createTextureIds(const unsigned int & numberOfIds);
        void bind();
        void bind(int id);
        void unbind();
        int id();
        int id(const int & level);

        GLenum target();

        bool mustFlipVertically();

        void mustFlipVertically(bool mustFlipVertically);
        unsigned int width();
        unsigned int height();
        unsigned int size();

        Vector2i::Ptr dimension();

        int depth();
        int border();
        GLenum format();
        GLenum internalFormat();
        GLenum pixelType();
        void parameter(GLenum type, int value);
        void parameter(GLenum type, float value);
        void parameter(GLenum type, std::vector<float> values);
        int parameter(int glid);
        void wrap(GLenum textureWrap);
        void wrapR(GLenum textureWrap);
        void wrapS(GLenum textureWrap);
        void wrapT(GLenum textureWrap);
        // void textureBorderColor(CCColor theColor);
        void updateFilter();
        void anisotropicFiltering(float amount);
        void textureFilter(GLenum filter);
        void textureMipmapFilter(GLenum filter);
        void textureEnvironmentMode(GLenum mode);
        void depthTextureMode(GLenum mode);
        // void blendColor(CCColor blendColor);

    protected:
        Texture(GLuint target, const TextureAttributes & attributes, unsigned int numberOfTextures); 
        
        GLenum _target;
        GLenum _environmentMode;

        GLenum _textureFilter;
        GLenum _textureMipmapFilter;

        // CCColor _blendColor = new CCColor();

        GLenum _internalFormat;
        GLenum _format;
        GLenum _pixelType;

        // CCPixelStorageModes _storageModes;

        std::vector<GLuint> _textureIDs;
        GLuint _textureID;

        int _width;
        int _height;
        int _depth;

        int _estimatedMemorySize;

        bool _mustFlipVertically;

        bool _generateMipmaps;
        bool _isCompressed;

    private:
	    bool _hasMipmaps;
};

}; // namespace
	
#endif // includeguard
