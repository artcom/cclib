#ifndef __CCLIB_PIXELSTORAGEMODES_INCLUDED__
#define __CCLIB_PIXELSTORAGEMODES_INCLUDED__

#include <iostream>
#include <string>
#include <vector>
#include <tr1/memory>
#include <GL/glew.h>

namespace cclib {
class PixelStorageModes {
    
    public:
        typedef std::tr1::shared_ptr<PixelStorageModes> Ptr;

        PixelStorageModes(); 
        virtual ~PixelStorageModes() {};
        
        bool swapBytes();
        void swapBytes(bool swapBytes);
        
        bool isLSBFirst();
        void isLSBFirst(bool lsbFirst);
        
        int rowLength();
        void rowLength(int rowLength);
        
        int alignment();
        void alignment(int alignment);
        
        int imageHeight();
        void imageHeight(int imageHeight);
        
        int skipPixels();
        void skipPixels(int skipPixels);
        
        int skipRows();
        void skipRows(int skipRows);
        
        int skipImages();
        void skipImages(int skipImages);
        
        void unpackStorage();
        void packStorage();
        
        void defaultUnpackStorage();

    private: 
        bool _swapBytes;
        bool _lsbFirst;
        int _rowLength;
        int _alignment;
        int _imageHeight;
        int _skipPixels;
        int _skipRows;
        int _skipImages;
};
};

#endif // includeguard
