#include "pixelstoragemodes.h"

using namespace cclib;

PixelStorageModes::PixelStorageModes() 
    : _swapBytes(false), _lsbFirst(false),
      _rowLength(0), _alignment(4), _imageHeight(0),
      _skipPixels(0), _skipRows(0), _skipImages(0) 
{};

bool 
PixelStorageModes::swapBytes() {
    return _swapBytes;
};
   
void 
PixelStorageModes::swapBytes(bool swapBytes) {
    _swapBytes = swapBytes;
};

bool 
PixelStorageModes::isLSBFirst() {
    return _lsbFirst;
}
void 
PixelStorageModes::isLSBFirst(bool lsbFirst) {
    _lsbFirst = lsbFirst;
}

int 
PixelStorageModes::rowLength() {
    return _rowLength;
}

void 
PixelStorageModes::rowLength(int rowLength) {
    _rowLength = rowLength;
}

int 
PixelStorageModes::alignment() {
    return _alignment;
}

void 
PixelStorageModes::alignment(int alignment) {
    _alignment = alignment;
}

int 
PixelStorageModes::imageHeight() {
    return _imageHeight;
}

void 
PixelStorageModes::imageHeight(int imageHeight) {
    _imageHeight = imageHeight;
}
int 
PixelStorageModes::skipPixels() {
    return _skipPixels;
}

void 
PixelStorageModes::skipPixels(int skipPixels) {
    _skipPixels = skipPixels;
}

int 
PixelStorageModes::skipRows() {
    return _skipRows;
}

void 
PixelStorageModes::skipRows(int skipRows) {
    _skipRows = skipRows;
}

int 
PixelStorageModes::skipImages() {
    return _skipImages;
}

void 
PixelStorageModes::skipImages(int skipImages) {
    _skipImages = skipImages;
}

void 
PixelStorageModes::unpackStorage() {
    glPixelStorei(GL_UNPACK_SWAP_BYTES, _swapBytes ? 1 : 0);
    glPixelStorei(GL_UNPACK_LSB_FIRST, _lsbFirst ? 1 : 0);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, _rowLength);
    glPixelStorei(GL_UNPACK_ALIGNMENT, _alignment);
    glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, _imageHeight);

    glPixelStorei(GL_UNPACK_SKIP_PIXELS, _skipPixels);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, _skipRows);
    glPixelStorei(GL_UNPACK_SKIP_IMAGES, _skipImages);
}

void 
PixelStorageModes::packStorage() {
    glPixelStorei(GL_PACK_SWAP_BYTES, _swapBytes ? 1 : 0);
    glPixelStorei(GL_PACK_LSB_FIRST, _lsbFirst ? 1 : 0);

    glPixelStorei(GL_PACK_ROW_LENGTH, _rowLength);
    glPixelStorei(GL_PACK_ALIGNMENT, _alignment);
    glPixelStorei(GL_PACK_IMAGE_HEIGHT, _imageHeight);

    glPixelStorei(GL_PACK_SKIP_PIXELS, _skipPixels);
    glPixelStorei(GL_PACK_SKIP_ROWS, _skipRows);
    glPixelStorei(GL_PACK_SKIP_IMAGES, _skipImages);
}

void 
PixelStorageModes::defaultUnpackStorage() {
    glPixelStorei(GL_UNPACK_SWAP_BYTES, 0);
    glPixelStorei(GL_UNPACK_LSB_FIRST, 0);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, 0);

    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_IMAGES, 0);
}


