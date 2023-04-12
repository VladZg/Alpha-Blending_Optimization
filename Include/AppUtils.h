#ifndef APP_UTILS_H
#define APP_UTILS_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <cassert>
#include <immintrin.h>

using namespace sf;

struct PixelImage
{
    Image           image   ;
    const Uint8*    pixels  ;
    int             width   ;
    int             height  ;
    int             n_pixels;
};

struct Pixel_128i
{
    __m128i pixelsL;
    __m128i pixelsH;
};

struct Pixel_256i
{
    __m256i pixelsL;
    __m256i pixelsH;
};

int ReadFile(FILE* file, Uint8** buf);

void PixelImageCtor(PixelImage* img, const char* img_filename);
void PixelImageDump(PixelImage* img);
void AllignPixelImage(PixelImage* alligned_img, PixelImage* source_img, int x, int y);
void PixelImageCreateFromPixels(PixelImage* img, int width, int height, const Uint8* pixels);

Music* CreateMusicObject(const char* source);
Text* CreateTextObject(Font* font, const char* msg, int fontsize, Color color, Text::Style style, float x_position, float y_position);
Sprite* CreateSpriteObject(const char* source, float x_position, float y_position);
int StretchSprite(Sprite* sprite, float width, float height);
void MoveScreen(int keycode, int* imp_x, int* imp_y);

#endif
