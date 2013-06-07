// in parts shamelessly stolen from here (or from any other revision)
// http://procedural-misc.googlecode.com/svn-history/r30/sandbox/trunk/hm_terrain_via_tex/main.cpp

#include <gl/cgshader.h>
#include "gpunoise.h"

using namespace cclib;

GPUNoisePtr GPUNoise::_instance = GPUNoisePtr();

int GPUNoise::perm[256] = { 
    151, 160, 137,  91,  90,  15, 131,  13, 201,  95,  96,  53, 194, 233,   7, 225, 
    140,  36, 103,  30,  69, 142,   8,  99,  37, 240,  21,  10,  23, 190,   6, 148, 
    247, 120, 234,  75,   0,  26, 197,  62,  94, 252, 219, 203, 117,  35,  11,  32, 
    57, 177,  33,  88, 237, 149,  56,  87, 174,  20, 125, 136, 171, 168,  68, 175, 
    74, 165,  71, 134, 139,  48,  27, 166,  77, 146, 158, 231,  83, 111, 229, 122, 
    60, 211, 133, 230, 220, 105,  92,  41,  55,  46, 245,  40, 244, 102, 143,  54, 
    65,  25,  63, 161,   1, 216,  80,  73, 209,  76, 132, 187, 208,  89,  18, 169, 
    200, 196, 135, 130, 116, 188, 159,  86, 164, 100, 109, 198, 173, 186,   3,  64, 
    52, 217, 226, 250, 124, 123,   5, 202,  38, 147, 118, 126, 255,  82,  85, 212, 
    207, 206,  59, 227,  47,  16,  58,  17, 182, 189,  28,  42, 223, 183, 170, 213, 
    119, 248, 152,   2,  44, 154, 163,  70, 221, 153, 101, 155, 167,  43, 172,   9, 
    129,  22,  39, 253,  19,  98, 108, 110,  79, 113, 224, 232, 178, 185, 112, 104, 
    218, 246,  97, 228, 251,  34, 242, 193, 238, 210, 144,  12, 191, 179, 162, 241, 
    81,  51, 145, 235, 249,  14, 239, 107,  49, 192, 214,  31, 181, 199, 106, 157, 
    184,  84, 204, 176, 115, 121,  50,  45, 127,   4, 150, 254, 138, 236, 205,  93, 
    222, 114,  67,  29,  24,  72, 243, 141, 128, 195,  78,  66, 215,  61, 156, 180 
};

int GPUNoise::grad3[16][3] = {
    { 0, 1,  1 }, {  0,  1, -1 }, {  0, -1, 1 }, {  0, -1, -1 }, 
    { 1, 0,  1 }, {  1,  0, -1 }, { -1,  0, 1 }, { -1,  0, -1 }, 
    { 1, 1,  0 }, {  1, -1,  0 }, { -1,  1, 0 }, { -1, -1,  0 }, // 12 cube edges
    { 1, 0, -1 }, { -1,  0, -1 }, {  0, -1, 1 }, {  0,  1,  1 } // 4 more to make 16
}; 

int GPUNoise::grad4[32][4] = { 
    {  0,  1, 1, 1 }, {  0,  1,  1, -1 }, {  0,  1, -1, 1 }, {  0,  1, -1, -1 }, // 32 tesseract edges
    {  0, -1, 1, 1 }, {  0, -1,  1, -1 }, {  0, -1, -1, 1 }, {  0, -1, -1, -1 }, 
    {  1,  0, 1, 1 }, {  1,  0,  1, -1 }, {  1,  0, -1, 1 }, {  1,  0, -1, -1 }, 
    { -1,  0, 1, 1 }, { -1,  0,  1, -1 }, { -1,  0, -1, 1 }, { -1,  0, -1, -1 }, 
    {  1,  1, 0, 1 }, {  1,  1,  0, -1 }, {  1, -1,  0, 1 }, {  1, -1,  0, -1 }, 
    { -1,  1, 0, 1 }, { -1,  1,  0, -1 }, { -1, -1,  0, 1 }, { -1, -1,  0, -1 }, 
    {  1,  1, 1, 0 }, {  1,  1, -1,  0 }, {  1, -1,  1, 0 }, {  1, -1, -1,  0 }, 
    { -1,  1, 1, 0 }, { -1,  1, -1,  0 }, { -1, -1,  1, 0 }, { -1, -1, -1,  0 } 
};

int GPUNoise::simplex4[64][4] = {
    {   0,  64, 128, 192 }, {   0, 64, 192, 128 }, {   0,   0,  0,   0 }, {   0, 128, 192, 64 }, 
    {   0,   0,   0,   0 }, {   0,  0,   0,   0 }, {   0,   0,  0,   0 }, {  64, 128, 192,  0 },
    {   0, 128,  64, 192 }, {   0,  0,   0,   0 }, {   0, 192, 64, 128 }, {   0, 192, 128, 64 }, 
    {   0,   0,   0,   0 }, {   0,  0,   0,   0 }, {   0,   0,  0,   0 }, {  64, 192, 128,  0 }, 
    {   0,   0,   0,   0 }, {   0,  0,   0,   0 }, {   0,   0,  0,   0 }, {   0,   0,   0,  0 }, 
    {   0,   0,   0,   0 }, {   0,  0,   0,   0 }, {   0,   0,  0,   0 }, {   0,   0,   0,  0 }, 
    {  64, 128,   0, 192 }, {   0,  0,   0,   0 }, {  64, 192,  0, 128 }, {   0,   0,   0,  0 }, 
    {   0,   0,   0,   0 }, {   0,  0,   0,   0 }, { 128, 192,  0,  64 }, { 128, 192,  64,  0 }, 
    {  64,   0, 128, 192 }, {  64,  0, 192, 128 }, {   0,   0,  0,   0 }, {   0,   0,   0,  0 }, 
    {   0,   0,   0,   0 }, { 128,  0, 192,  64 }, {   0,   0,  0,   0 }, { 128,  64, 192,  0 }, 
    {   0,   0,   0,   0 }, {   0,  0,   0,   0 }, {   0,   0,  0,   0 }, {   0,   0,   0,  0 }, 
    {   0,   0,   0,   0 }, {   0,  0,   0,   0 }, {   0,   0,  0,   0 }, {   0,   0,   0,  0 }, 
    { 128,   0,  64, 192 }, {   0,  0,   0,   0 }, {   0,   0,  0,   0 }, {   0,   0,   0,  0 },
    { 192,   0,  64, 128 }, { 192,  0, 128,  64 }, {   0,   0,  0,   0 }, { 192,  64, 128,  0 }, 
    { 128,  64,   0, 192 }, {   0,  0,   0,   0 }, {   0,   0,  0,   0 }, {   0,   0,   0,  0 },
    { 192,  64,   0, 128 }, {   0,  0,   0,   0 }, { 192, 128,  0,  64 }, { 192, 128,  64,  0 }
};

GPUNoise::GPUNoise() : 
    permTextureID(0), simplexTextureID(0), gradTextureID(0) 
{
    initPermTexture();
    initSimplexTexture();
    initGradTexture();
}


GPUNoisePtr
GPUNoise::get() {
    if (!GPUNoise::_instance) {
        GPUNoise::_instance = GPUNoisePtr(new GPUNoise()); 
    }

    return GPUNoise::_instance;
}

void 
GPUNoise::attachFragmentNoise(CGShader * theShader) {
    CGparameter myPermTextureParam = theShader->fragmentParameter("permTexture");
    CGparameter mySimplexTextureParam = theShader->fragmentParameter("simplexTexture");
    CGparameter myGradTextureParam = theShader->fragmentParameter("gradTexture");

    GPUNoisePtr noise = GPUNoise::get();

    theShader->texture(myPermTextureParam, noise->permTextureID);
    theShader->texture(mySimplexTextureParam, noise->simplexTextureID);
    theShader->texture(myGradTextureParam, noise->gradTextureID);
    glBindTexture(GL_TEXTURE_2D, 0);

}

void 
GPUNoise::attachVertexNoise(CGShader * theShader) {
    CGparameter myPermTextureParam = theShader->vertexParameter("permTexture");
    CGparameter mySimplexTextureParam = theShader->vertexParameter("simplexTexture");
    CGparameter myGradTextureParam = theShader->vertexParameter("gradTexture");

    GPUNoisePtr noise = GPUNoise::get();

    theShader->texture(myPermTextureParam, noise->permTextureID);
    theShader->texture(mySimplexTextureParam, noise->simplexTextureID);
    theShader->texture(myGradTextureParam, noise->gradTextureID);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void 
GPUNoise::initPermTexture() {
    int i, j;

    glGenTextures(1, &(permTextureID)); // Generate a unique texture ID
    glBindTexture(GL_TEXTURE_2D, permTextureID); // Bind the texture to texture unit 0

    std::vector<char> pixels = std::vector<char>(256 * 256 * 4, 0);
    for (i = 0; i < 256; i++) {
        for (j = 0; j < 256; j++) {
            int offset = (i * 256 + j) * 4;
            char value = perm[(j + perm[i]) & 0xFF];
            pixels[offset]     = GPUNoise::grad3[value & 0x0F][0] * 64 + 64; // Gradient x
            pixels[offset + 1] = GPUNoise::grad3[value & 0x0F][1] * 64 + 64; // Gradient y
            pixels[offset + 2] = GPUNoise::grad3[value & 0x0F][2] * 64 + 64; // Gradient z
            pixels[offset + 3] = value; // Permuted index
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(pixels[0]));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

/**
 * initSimplexTexture(GLuinttexID) - create and load a 1D texture for a simplex traversal order lookup table. This
 * is used for simplex noise only, and only for 3D and 4D noise where there are more than 2 simplices. (3D simplex
 * noise has 6 cases to sort out, 4D simplex noise has 24 cases.)
 */
void 
GPUNoise::initSimplexTexture() {
    glGenTextures(1, &(simplexTextureID)); // Generate a unique texture ID
    glBindTexture(GL_TEXTURE_1D, simplexTextureID); // Bind the texture to texture unit 1

    std::vector<char> simplexBuffer;
    for (int x=0; x<64; x++) {
        for (int y=0; y<4; y++) {
            simplexBuffer.push_back(GPUNoise::simplex4[x][y]);
        }
    }

    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(simplexBuffer[0]));
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

/**
 * initGradTexture(GLuinttexID) - create and load a 2D texture for a 4D gradient lookup table. This is used for 4D
 * noise only.
 */
void 
GPUNoise::initGradTexture() {
    int i, j;

    glGenTextures(1, &(gradTextureID)); // Generate a unique texture ID
    glBindTexture(GL_TEXTURE_2D, gradTextureID); // Bind the texture to texture unit 2

    std::vector<char> pixels = std::vector<char>(256 * 256 * 4, 0);
    for (i = 0; i < 256; i++)
        for (j = 0; j < 256; j++) {
            int offset = (i * 256 + j) * 4;
            int value = GPUNoise::perm[(j + GPUNoise::perm[i]) & 0xFF];
            pixels[offset] =     GPUNoise::grad4[value & 0x1F][0] * 64 + 64; // Gradient x
            pixels[offset + 1] = GPUNoise::grad4[value & 0x1F][1] * 64 + 64; // Gradient y
            pixels[offset + 2] = GPUNoise::grad4[value & 0x1F][2] * 64 + 64; // Gradient z
            pixels[offset + 3] = GPUNoise::grad4[value & 0x1F][3] * 64 + 64; // Gradient
        }

    // GLFW texture loading functions won't work here - we need GL_NEAREST lookup.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(pixels[0]));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

