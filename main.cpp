#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <cassert>
#include <immintrin.h>
#include "./Include/AppUtils.h"
#include "./Include/IntrinsicsPrintf.h"

// #define DRAW_MODE
#define MOVE_MODE

#define SSE_MODE
// #define AVX2_MODE

using namespace sf;

#define CYCLE_MAX 10
#define N_MAX     255
#define ZERO 0x80

const int width = 800;
const int height = 600;
const int num_pixels = width * height;

int imp_x = 280;
int imp_y = 220;

inline void AlphaBlend(const Color* Back, const Color* Front, Color* Screen, const int n_pixels);

#define MAX_FILE_LEN 150000

#ifdef DRAW_MODE

int main()
{
    RenderWindow window(VideoMode(width, height), "Mandelbrot");

    PixelImage table_img;
    PixelImageCtor(&table_img, "./Pictures/Table.bmp");
    // PixelImageDump(&table_img);

    PixelImage cat_img;
    PixelImageCtor(&cat_img, "./Pictures/AskhatCat.bmp");
    // PixelImageDump(&cat_img);

    const Uint8 cat_alligned_img_pixels[table_img.width*table_img.height*4] = {};
    PixelImage cat_alligned_img;
    PixelImageCreateFromPixels(&cat_alligned_img, table_img.width, table_img.height, cat_alligned_img_pixels);
    // PixelImageDump(&cat_alligned_img);

    const Uint8 screen_img_pixels[table_img.width*table_img.height*4] = {};
    PixelImage screen_img;
    PixelImageCreateFromPixels(&screen_img, table_img.width, table_img.height, screen_img_pixels);
    // PixelImageDump(&screen_img);

    AllignPixelImage(&cat_alligned_img, &cat_img, imp_x, imp_y);

    Texture texture;
    texture.loadFromImage(table_img.image);
    Sprite sprite(texture);

    // float fps_num = 0.0;
    // char fps_str[20] = {};
    // Font* font = new Font;
    // font->loadFromFile("./fonts/arial.ttf");
    // Text* fps_obj  = CreateTextObject(font, fps_str, 28, Color::Blue, Text::Style::Bold, 0.0, 0.0);
    // Clock clock;
    // float current_time = 0.0;

    while (window.isOpen())
    {
        window.clear();

        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case Event::Closed:
                {
                    window.close();
                    break;
                }
            }
        }

        // clock.restart();

        AlphaBlend((Color*) table_img.pixels, (Color*) cat_alligned_img.pixels, (Color*) screen_img.pixels, table_img.n_pixels);

        // current_time = clock.restart().asSeconds();
        // fps_num = 1.0 / (current_time);
        // sprintf(fps_str, "fps: %f", fps_num);
        // fps_obj->setString(fps_str);

        texture.update(screen_img.pixels, screen_img.width, screen_img.height, 0, 0);
        sprite.setTexture(texture);

        window.draw(sprite);
        // window.draw(*fps_obj);
        window.display();
    }

    return 1;
}

#else
#ifdef MOVE_MODE

int main()
{
    RenderWindow window(VideoMode(width, height), "Mandelbrot");

    // float fps_num = 0.0;
    // char fps_str[20] = {};
    // Font* font = new Font;
    // font->loadFromFile("./fonts/arial.ttf");
    // Text* fps_obj  = CreateTextObject(font, fps_str, 28, Color::Blue, Text::Style::Bold, 0.0, 0.0);
    // Clock clock;
    // float current_time = 0.0;

    while (window.isOpen())
    {
        window.clear();

        PixelImage table_img;
        PixelImageCtor(&table_img, "./Pictures/Table.bmp");
        // PixelImageDump(&table_img);

        PixelImage cat_img;
        PixelImageCtor(&cat_img, "./Pictures/AskhatCat.bmp");
        // PixelImageDump(&cat_img);

        const Uint8 cat_alligned_img_pixels[table_img.width*table_img.height*4] = {};
        PixelImage cat_alligned_img;
        PixelImageCreateFromPixels(&cat_alligned_img, table_img.width, table_img.height, cat_alligned_img_pixels);
        // PixelImageDump(&cat_alligned_img);

        const Uint8 screen_img_pixels[table_img.width*table_img.height*4] = {};
        PixelImage screen_img;
        PixelImageCreateFromPixels(&screen_img, table_img.width, table_img.height, screen_img_pixels);
        // PixelImageDump(&screen_img);

        AllignPixelImage(&cat_alligned_img, &cat_img, imp_x, imp_y);

        Texture texture;
        texture.loadFromImage(table_img.image);
        Sprite sprite(texture);

        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case Event::Closed:
                {
                    window.close();
                    break;
                }

                case Event::KeyPressed:
                {
                    MoveScreen(event.key.code, &imp_x, &imp_y);
                    break;
                }
            }
        }

        // clock.restart();

        AlphaBlend((Color*) table_img.pixels, (Color*) cat_alligned_img.pixels, (Color*) screen_img.pixels, table_img.n_pixels);

        // current_time = clock.restart().asSeconds();
        // fps_num = 1.0 / (current_time);
        // sprintf(fps_str, "fps: %f", fps_num);
        // fps_obj->setString(fps_str);

        texture.update(screen_img.pixels, screen_img.width, screen_img.height, 0, 0);
        sprite.setTexture(texture);

        window.draw(sprite);
        // window.draw(*fps_obj);
        window.display();
    }

    return 1;
}

#else

int main()
{
    PixelImage table_img;
    PixelImageCtor(&table_img, "./Pictures/Table.bmp");
    // PixelImageDump(&table_img);

    PixelImage cat_img;
    PixelImageCtor(&cat_img, "./Pictures/AskhatCat.bmp");
    // PixelImageDump(&cat_img);

    const Uint8 cat_alligned_img_pixels[table_img.width*table_img.height*4] = {};
    PixelImage cat_alligned_img;
    PixelImageCreateFromPixels(&cat_alligned_img, table_img.width, table_img.height, cat_alligned_img_pixels);
    // PixelImageDump(&cat_alligned_img);

    const Uint8 screen_img_pixels[table_img.width*table_img.height*4] = {};
    PixelImage screen_img;
    PixelImageCreateFromPixels(&screen_img, table_img.width, table_img.height, screen_img_pixels);
    // PixelImageDump(&screen_img);

    AllignPixelImage(&cat_alligned_img, &cat_img, 280, 220);

    Clock clock;
    double fps = 0.0;
    float sum_fps = 0.0;
    int n_fps = 0;
    float avg_fps = 0.0;

    while (true)
    {
        clock.restart();

        for (int cycle = 0; cycle < CYCLE_MAX; cycle++)
        {
            AlphaBlend((Color*) table_img.pixels, (Color*) cat_alligned_img.pixels, (Color*) screen_img.pixels, table_img.n_pixels);
        }

        float working_time = clock.restart().asSeconds();
        fps = 1.f / working_time;
        n_fps++;
        sum_fps += fps;
        avg_fps = sum_fps / n_fps;

        if (n_fps % 10 == 0) printf("FPS: %.2f => average FPS: %.2f\n", fps, avg_fps);
    }

    return 1;
}

#endif
#endif

#ifdef SSE_MODE

const __m128i _0_m128i   = _mm_set1_epi8(0);
const __m128i _255_m128i = _mm_set1_epi16(255);

inline void AlphaBlend(const Color* Back, const Color* Front, Color* Screen, const int n_pixels)
{
    for (int pixel_i = 0; pixel_i < n_pixels; pixel_i+=4)
    {
        //================================================================================
        // Loading data of 4 pixels
        //================================================================================
        // frontL
        // [15] [14] [13] [12] [11] [10] [09] [08] [07] [06] [05] [04] [03] [02] [01] [00]
        // fr1  fg1  fb1  fa1  fr2  fg2  fb2  fa2   ??   ??   ??   ??   ??   ??   ??   ??
        //================================================================================
        // frontH
        // [15] [14] [13] [12] [11] [10] [09] [08] [07] [06] [05] [04] [03] [02] [01] [00]
        // fr3  fg3  fb3  fa3  fr4  fg4  fb4  fa4   00   00   00   00   00   00   00   00
        //================================================================================
        // backL
        // [15] [14] [13] [12] [11] [10] [09] [08] [07] [06] [05] [04] [03] [02] [01] [00]
        // br1  bg1  bb1  ba1  br2  bg2  bb2  ba2   ??   ??   ??   ??   ??   ??   ??   ??
        //================================================================================
        // backH
        // [15] [14] [13] [12] [11] [10] [09] [08] [07] [06] [05] [04] [03] [02] [01] [00]
        // br3  bg3  bb3  ba3  br4  bg4  bb4  ba4   00   00   00   00   00   00   00   00
        //================================================================================

        Pixel_128i front = { _mm_load_si128((__m128i*)(Front + pixel_i)),
                            (__m128i) _mm_movehl_ps((__m128) _0_m128i, (__m128) front.pixelsL)};

        Pixel_128i back = { _mm_load_si128((__m128i*) (Back  + pixel_i)),
                           (__m128i) _mm_movehl_ps((__m128) _0_m128i, (__m128) back.pixelsL)  };

        // printf_m128i(frontL);
        // printf_m128i(frontH);
        // printf_m128i(backL);
        // printf_m128i(backH);
        // printf("\n");

        //================================================================================
        // Separating bytes
        //================================================================================
        // frontL
        // [15] [14] [13] [12] [11] [10] [09] [08] [07] [06] [05] [04] [03] [02] [01] [00]
        // fr1   00  fg1   00  fb1   00  fa1   00  fr2   00  fg2   00  fb2   00  fa2   00
        //================================================================================
        // frontH
        // [15] [14] [13] [12] [11] [10] [09] [08] [07] [06] [05] [04] [03] [02] [01] [00]
        // fr3   00  fg3   00  fb3   00  fa3   00  fr4   00  fg4   00  fb4   00  fa4   00
        //================================================================================
        // backL
        // [15] [14] [13] [12] [11] [10] [09] [08] [07] [06] [05] [04] [03] [02] [01] [00]
        // br1   00  bg1   00  bb1   00  ba1   00  br2   00  bg2   00  bb2   00  ba2   00
        //================================================================================
        // backH
        // [15] [14] [13] [12] [11] [10] [09] [08] [07] [06] [05] [04] [03] [02] [01] [00]
        // br3   00  bg3   00  bb3   00  ba3   00  br4   00  bg4   00  bb4   00  ba4   00
        //================================================================================

        front = {_mm_cvtepu8_epi16(front.pixelsL),
                 _mm_cvtepu8_epi16(front.pixelsH)};

        back  = {_mm_cvtepu8_epi16(back.pixelsL),
                 _mm_cvtepu8_epi16(back.pixelsH)};

        // printf_m128i(frontL);
        // printf_m128i(frontH);
        // printf_m128i(backL);
        // printf_m128i(backH);
        // printf("\n");

        //================================================================================
        // Getting alpha parameter from front pixels (a1, a2, a3, a4)
        //================================================================================
        // alphaL
        // [15] [14] [13] [12] [11] [10] [09] [08] [07] [06] [05] [04] [03] [02] [01] [00]
        // fa1   00  fa1   00  fa1   00  fa1   00  fa2   00  fa2   00  fa2   00  fa2   00
        //================================================================================
        // alphaH
        // [15] [14] [13] [12] [11] [10] [09] [08] [07] [06] [05] [04] [03] [02] [01] [00]
        // fa3   00  fa3   00  fa3   00  fa3   00  fa4   00  fa4   00  fa4   00  fa4   00
        //================================================================================

        __m128i alpha_shuffle_mask = _mm_set_epi8(ZERO, 14, ZERO, 14, ZERO, 14, ZERO, 14,
                                                  ZERO, 6,  ZERO, 6,  ZERO, 6,  ZERO, 6  );

        Pixel_128i alpha = {_mm_shuffle_epi8(front.pixelsL, alpha_shuffle_mask),
                            _mm_shuffle_epi8(front.pixelsH, alpha_shuffle_mask)};

        // __m128i alphaL = _mm_shuffle_epi8(frontL, alpha_shuffle_mask);  // a1, a2
        // __m128i alphaH = _mm_shuffle_epi8(frontH, alpha_shuffle_mask);  // a3, a4

        // printf_m128i(alpha_shuffle_mask);
        // printf_m128i(alphaL);
        // printf_m128i(alphaH);
        // printf("\n");

        //========================================================================================
        // Mulling colors on alpha (front*a, back*(1-a))
        //========================================================================================
        // frontL
        // [15] [14] [13] [12] [11] [10] [09] [08] [07] [06] [05] [04] [03] [02] [01] [00]
        // fr1 * a1  fg1 * a1  fb1 * a1  fa1 * a1  fr2 * a2  fg2 * a2  fb2 * a2  fa2 * a2
        //========================================================================================
        // frontH
        // [15] [14] [13] [12] [11] [10] [09] [08] [07] [06] [05] [04] [03] [02] [01] [00]
        // fr3 * a3  fg3 * a3  fb3 * a3  fa3 * a3  fr4 * a4  fg4 * a4  fb4 * a4  fa4 * a4
        //========================================================================================
        // backL
        // [15] [14]  [13] [12]  [11] [10]  [09] [08]  [07] [06]  [05] [04]  [03] [02]  [01] [00]
        // br1*(1-a1) bg1*(1-a1) bb1*(1-a1) ba1*(1-a1) br2*(1-a2) bg2*(1-a2) bb2*(1-a2) ba2*(1-a2)
        //========================================================================================
        // backH
        // [15] [14]  [13] [12]  [11] [10]  [09] [08]  [07] [06]  [05] [04]  [03] [02]  [01] [00]
        // br3*(1-a3) bg3*(1-a3) bb3*(1-a3) ba3*(1-a3) br4*(1-a4) bg4*(1-a4) bb4*(1-a4) ba4*(1-a4)
        //========================================================================================

        front = {_mm_mullo_epi16(front.pixelsL, alpha.pixelsL),                             // front_color1 *= a1;      front_color2 *= a2;
                 _mm_mullo_epi16(front.pixelsH, alpha.pixelsH)};                            // front_color3 *= a3;      front_color4 *= a4;

        back  = {_mm_mullo_epi16(back.pixelsL, _mm_sub_epi16(_255_m128i, alpha.pixelsL)),   // back_color1 *= (255-a1); back_color2 *= (255-a2);
                 _mm_mullo_epi16(back.pixelsH, _mm_sub_epi16(_255_m128i, alpha.pixelsH))};  // back_color3 *= (255-a3); back_color4 *= (255-a4);

        // printf_m128i(frontL);
        // printf_m128i(frontH);
        // printf_m128i(backL);
        // printf_m128i(backH);
        // printf("\n");

        //================================================================================
        // Summing mulled on alpha back and front colors (front*a+back*(1-a))
        //================================================================================
        // sumL
        // [15] [14] [13] [12] [11] [10] [09] [08] [07] [06] [05] [04] [03] [02] [01] [00]
        // sr1       sg1       sb1       sa1       sr2       sg2       sb2       sa2
        //================================================================================
        // sumH
        // [15] [14] [13] [12] [11] [10] [09] [08] [07] [06] [05] [04] [03] [02] [01] [00]
        // sr3       sg3       sb3       sa3       sr4       sg4       sb4       sa4
        //================================================================================

        Pixel_128i sum = {_mm_add_epi16(front.pixelsL, back.pixelsL),   // screen_color1 = front_color1 + back_color1;  screen_color2 = front_color2 + back_color2;
                          _mm_add_epi16(front.pixelsH, back.pixelsH)};  // screen_color3 = front_color3 + back_color3;  screen_color4 = front_color4 + back_color4;

        // printf_m128i(sumL);
        // printf_m128i(sumH);
        // printf("\n");

        //=======================================================================================================
        // normalizing results ((front*a+back*(1-a))>>8)
        //=======================================================================================================
        // sumL
        //  [15]   [14]   [13]   [12]   [11]   [10]   [09]   [08]  [07]  [06]  [05]  [04]  [03]  [02]  [01]  [00]
        // sr1>>8 sg1>>8 sb1>>8 sa1>>8 sr2>>8 sg2>>8 sb2>>8 sa2>>8  ??    ??    ??    ??    ??    ??    ??    ??
        //=======================================================================================================
        // sumH
        //  [15]   [14]   [13]   [12]   [11]   [10]   [09]   [08]  [07]  [06]  [05]  [04]  [03]  [02]  [01]  [00]
        // sr3>>8 sg3>>8 sb3>>8 sa3>>8 sr4>>8 sg4>>8 sb4>>8 sa4>>8  ??    ??    ??    ??    ??    ??    ??    ??
        //=======================================================================================================

        __m128i sum_shuffle_mask = _mm_set_epi8(ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO,
                                                15,   13,   11,   9,    7,    5,    3,    1    );

        sum = {_mm_shuffle_epi8(sum.pixelsL, sum_shuffle_mask),     // screen_color1 >> 8;  screen_color2 >> 8;
               _mm_shuffle_epi8(sum.pixelsH, sum_shuffle_mask)};    // screen_color3 >> 8;  screen_color4 >> 8;

        // printf_m128i(sumL);
        // printf_m128i(sumH);
        // printf("\n");

        //================================================================================
        // Storing all 4 pixels in __m128i var and loading it to the screen array
        //================================================================================
        // screen_color
        // [15] [14] [13] [12] [11] [10] [09] [08] [07] [06] [05] [04] [03] [02] [01] [00]
        // sr1  sg1  sb1  sa1  sr2  sg2  sb2  sa2  sr3  sg3  sb3  sa3  sr4  sg4  sb4  sa4
        //================================================================================

        __m128i screen = (__m128i) _mm_movelh_ps((__m128) sum.pixelsL , (__m128) sum.pixelsH);

        // printf_m128i(screen_color);
        // printf("\n");

        _mm_store_si128((__m128i*)(Screen + pixel_i), screen);
    }
}

#else
#ifdef AVX2_MODE

inline void AlphaBlend(const Color* Back, const Color* Front, Color* Screen, const int n_pixels)
{

}

#else

inline void AlphaBlend(const Color* Back, const Color* Front, Color* Screen, const int n_pixels)
{
    // color = front_color * front_alpha + back_color * (255 - front_alpha) >> 8

    for (int pixel_i = 0; pixel_i < n_pixels; pixel_i++)
    {
        Color front_color = Front[pixel_i];
        Color back_color = Back[pixel_i];
        Uint8 a = front_color.a;

        Screen[pixel_i] = {(Uint8)(((front_color.r) * a + back_color.r * (255 - a)) >> 8),
                           (Uint8)(((front_color.g) * a + back_color.g * (255 - a)) >> 8),
                           (Uint8)(((front_color.b) * a + back_color.b * (255 - a)) >> 8),
                           (Uint8)(((front_color.a) * a + back_color.a * (255 - a)) >> 8)};
    }
}

#endif
#endif
