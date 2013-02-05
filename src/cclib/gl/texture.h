#ifndef __CCLIB_TEXTURE_INCLUDED__
#define __CCLIB_TEXTURE_INCLUDED__

#include <cclib.h>

namespace cclib {
        

class TextureAttributes {
    public:
        
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
        virtual ~Texture(); 
        
        TexturePtr createTexture(GLuint target, TextureAttributesPtr attributes, unsigned int numberOfTextures = 1); 
        
        // virtual void dataImplementation(CCTextureData theData);
        // virtual void updateData(final CCTextureData theData);
        
        virtual void generateMipmaps(const bool & generateMipmaps);
        virtual bool & generateMipmaps(); 
        
        virtual bool & isCompressed();
        // void data(CCTextureData data);

        virtual std::vector<GLuint> createTextureIds(const unsigned int & numberOfIds);
        virtual void bind();
        virtual void bind(int id);
        virtual void unbind();
        virtual int id();
        virtual int id(const int & level);

        virtual GLenum target();

        virtual bool mustFlipVertically();

        virtual void mustFlipVertically(bool mustFlipVertically);
        virtual unsigned int width();
        virtual unsigned int height();
        virtual unsigned int size();

        virtual Vector2iPtr dimension();

        virtual int depth();
        virtual int border();
        virtual GLenum format();
        virtual GLenum internalFormat();
        virtual GLenum pixelType();
        virtual void parameter(GLenum type, int value);
        virtual void parameter(GLenum type, float value);
        virtual void parameter(GLenum type, std::vector<float> values);
        virtual int parameter(int glid);
        virtual void wrap(GLenum textureWrap);
        virtual void wrapR(GLenum textureWrap);
        virtual void wrapS(GLenum textureWrap);
        virtual void wrapT(GLenum textureWrap);
        // void textureBorderColor(CCColor theColor);
        virtual void updateFilter();
        virtual void anisotropicFiltering(float amount);
        virtual void textureFilter(GLenum filter);
        virtual void textureMipmapFilter(GLenum filter);
        virtual void textureEnvironmentMode(GLenum mode);
        virtual void depthTextureMode(GLenum mode);
        // void blendColor(CCColor blendColor);

    protected:
        Texture(GLuint target, TextureAttributesPtr attributes, unsigned int numberOfTextures); 
        
        GLenum _target;
        GLenum _environmentMode;

        GLenum _textureFilter;
        GLenum _textureMipmapFilter;

        // CCColor _blendColor = new CCColor();

        GLenum _internalFormat;
        GLenum _format;
        GLenum _pixelType;

        PixelStorageModesPtr _storageModes;

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
