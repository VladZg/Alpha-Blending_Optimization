#ifndef APP_UTILS_CPP
#define APP_UTILS_CPP

#include <stdlib.h>
#include <cassert>
#include <math.h>
#include "./AppUtils.h"

void PixelImageCtor(PixelImage* img, const char* img_filename)
{
	assert(img_filename != nullptr);

	img->image.loadFromFile(img_filename);
	img->pixels	  = img->image.getPixelsPtr();
	img->width    = img->image.getSize().x;
	img->height   = img->image.getSize().y;
	img->n_pixels = img->width * img->height;
}

void PixelImageCreateFromPixels(PixelImage* img, int width, int height, const Uint8* pixels)
{
	assert(img && pixels);

    img->height = height;
    img->width  = width ;
    img->n_pixels = width*height;
    img->pixels = pixels;
}

void PixelImageDump(PixelImage* img)
{
	assert(img);

	fprintf(stderr, "width: %d, height: %d, n_pixels: %d\n", img->width, img->height, img->n_pixels);
	// const Color* pixels = (const Color*) img->pixels;

	// for (int i = 0; i < 20; i++)
	// {
	// 	fprintf(stderr, "(%X %X %X %X) ", pixels[i].r,  pixels[i].g, pixels[i].b, pixels[i].a);
	// }

	// fprintf(stderr, "\n");
}

void AllignPixelImage(PixelImage* alligned_img, PixelImage* source_img, int x, int y)
{
    assert(alligned_img && source_img);

    // PixelImageDump(alligned_img);
    // PixelImageDump(source_img);

    if ((x < 0) || (y < 0) ||
        (x > alligned_img->width) || (y > alligned_img->height) ||
        (alligned_img->width - x < source_img->width) ||
        (alligned_img->height - y < source_img->height))
    {
        printf("CAN'T ALLIGN IMAGES(((\n");
        return;
    }

    Color* alligned_pixels = (Color*) alligned_img->pixels;
    Color* source_pixels = (Color*) source_img->pixels;

    const int max_yi = y + source_img->height - 1;
    const int max_xi = x + source_img->width  - 1;
    int alligned_pixel_i = y * alligned_img->width + x;
    int source_pixel_i = 0;

    for (int yi = y - 1; yi < max_yi; yi++)
    {
        for (int xi = x - 1; xi < max_xi; xi++, alligned_pixel_i++, source_pixel_i++)
        {
            alligned_pixels[alligned_pixel_i] = source_pixels[source_pixel_i];
        }

        alligned_pixel_i += alligned_img->width - source_img->width;
    }
}

Music* CreateMusicObject(const char* source)
{
    Music* music = new Music;
    music->openFromFile(source);
    assert(music != nullptr);

    return music;
}

Text* CreateTextObject(Font* font, const char* msg, int fontsize, Color color, Text::Style style, float x_position, float y_position)
{
    assert(msg != nullptr);
    Text* text = new Text;
    assert(text != nullptr);
    text->setFont(*font);
    text->setString(msg);
    text->setCharacterSize(fontsize);
    text->setFillColor(color);
    text->setStyle(style);
    text->setPosition(x_position, y_position);

    return text;
}

Sprite* CreateSpriteObject(const char* source, float x_position, float y_position)
{
    assert(source != nullptr);

    Image* image = new Image;
    assert(image != nullptr);
    image->loadFromFile(source);

    Texture* texture = new Texture;
    assert(texture != nullptr);
    texture->loadFromImage(*image);

    Sprite* sprite = new Sprite;
    assert(sprite != nullptr);
    sprite->setTexture(*texture);
    sprite->setPosition(x_position, y_position);

    return sprite;
}

int StretchSprite(Sprite* sprite, float width, float height)
{
    Vector2f targetSize(width, height);

    sprite->setScale(targetSize.x / sprite->getLocalBounds().width,
                     targetSize.y / sprite->getLocalBounds().height);

    return 1;
}

int ReadFile(FILE* file, Uint8** buf)
{
    assert(buf != nullptr);
    assert(*buf != nullptr);
    assert(file != nullptr);

    fseek(file, 0L, SEEK_END);
    int file_len = ftell(file);
    fseek(file, 0L, SEEK_SET);

    return fread(*buf, sizeof(Uint8), file_len, file);
}

#endif
