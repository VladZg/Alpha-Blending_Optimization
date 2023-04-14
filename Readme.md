# Optimization of alpha-blending algorithm

## Introduction
In this work I tried to optimize algorithm of merging pictures by using available on my computer SIMD-instructions: SSE and AVX2.

**Goals**:
- to achieve maximum speed up the algorithm of alpha-blending using SIMD-instrucrtions
- compare perfomances of programms that use SSE and AVX2 instructions
- compare alligned and non-alligned instructions of loading data 

## Alpha-Blending algorithm
This algorithm is widely used for merging pictures. There are two pictures: back- and foreground, the second one should be imposed on the first. When two pictures of the same size given as arrays of pixels in RGB format, every pixel has 4 components.

``(Red, Green, Blue, Alpha)``

To merge pictures with the same size we use an algorithm that takes every pixel of both pictures (arrays ``front`` and ``back``) and calculates pixels of a resulting picture (array ``screen``) using simple rool.

``screen_clr = front_clr * front_alpha + back_clr * (1-front_alpha)``,

where ``front_alpha`` is normalized alpha coefficent of the front picture pixel

So perfomance of algorithm depends on amount of pixels in resulting picture

``t ~ width * height``

## Used data
In our case, there are two pictures given.

![Table](Pictures/Table.bmp)
![AskhatCat](Pictures/AskhatCat.bmp)

As a result of programm working cat has to appear on the table. Resulting picture has the same size as picture with table: ``800*600``, so number of operating pixels in algorithm is ``800*600 = 480000``. It actually takes time to process all of them, so it requires optimization.

## Optimization principles
I tried to increase the speed of blending by using SIMD-instruction SSE and AVX2 to be able to operate with more than one pixel at the same time. Ideas of optimzation that use SSE and AVX2 instructions are different.

### SSE optimization
We are able to process four pixels at the same time by using __m128i variables and SSE instructions. Here is mechanism of optimization described step-by-step.

**1) Loading data from Front and Back pixel arrays.**

    ``__m128i front = Front[i]``
    
    ``__m128i back  = Back[i] ``

   used commands: ``_mm_load_si128`` <-- I use instructions that work with alligned data, because it's faster

**2) Splitting pixels data on two variables (higher (1,2) and lower (3,4) pixels).**

    ``__m128i front -> __m128i frontH, __m128i frontL``
    
    ``__m128i back  -> __m128i backtH, __m128i backL ``

   used commands: ``_mm_movehl_ps``

**3) Converting all variables to the format for mulling.**
    Multiplying a color component that takes 1 byte by another one-byte value of the alpha component, a 2-byte number is obtained, so I prepare vectors with pixel components for multiplication, supplementing each component with zeros up to 2 bytes.
    
   After this step each of pixels vector looks so:
    
    |index|[15]|[14]|[13]|[12]|[11]|[10]|[09]|[08]|[07]|[06]|[05]|[04]|[03]|[02]|[01]|[00]|
    |:----|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
    |value| ri | 00 | gi | 00 | bi | 00 | ai | 00 | rj | 00 | gj | 00 | bj | 00 | aj | 00 |

   Where ri, gi, bi, ai are red, green, blue, alpha compomemts of one of the pixel

   used commands: ``_mm_cvtepu8_epi16``

**4) Getting alpha parameters in the same format.**
    To multiply pixel vectors on alpa we should prepare vectors, that contain alpha component of every front pixel. I create them at the same format for the next mulling.
    
   After this step there are 2 vector variables that look so:
    
    |index|[15]|[14]|[13]|[12]|[11]|[10]|[09]|[08]|[07]|[06]|[05]|[04]|[03]|[02]|[01]|[00]|
    |:----|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
    |value| ai | 00 | ai | 00 | ai | 00 | ai | 00 | aj | 00 | aj | 00 | aj | 00 | aj | 00 |

    ``__m128i frontL -> __m128i alphaL``
    
    ``__m128i frontH -> __m128i alphaH``

   used commands: ``_mm_shuffle_epi8`` with mask ``__m128i alpha_shuffle_mask = {128, 14, 128, 14, 128, 14, 128, 14, 128, 6, 128, 6, 128, 6, 128, 6}``

**5) Mulling front and back colors on alphas.**

    ``frontL *= alphaL        ``, ``frontH *= alphaH        ``
    ``backL  *= (255 - alphaL)``, ``backH  *= (255 - alphaH)``

   used commands: ``_mm_mullo_epi16``, ``_mm_sub_epi16``

**6) Summing front and back colors.**

    ``__m128i sumL = frontL + backL``
    ``__m128i sumH = frontH + backH``

   used commands: ``_mm_add_epi16``

**7) Normalizing sums.**
    In the blending formula it's said that alpha parameter have to be normalized, in our case we have to divide the previous results on 255 to do it, because it is in a range from 0 to 255. However, there is a simplier and faster way to do it with a small error, that almost don't affect the result. The solution that makes step faster is division on 256 to normalize the result, because it has simple byte-operation amalog ``>>8`` that is much faster. To do it, I take the most significant byte of every component and rewrite it to the high bytes of the vector.

    ``sumL = sumL >> 8``
    
    ``sumH = sumH >> 8``

   used commands: ``_mm_shuffle_epi8`` with mask ``__m128i sum_shuffle_mask = {128, 128, 128, 128, 128, 128, 128, 128, 15, 13, 11, 9, 7, 5, 3, 1}``

**8) Storing all four result pixels in result var.**
    I join two sum vector variables to a new variable to load it to the memory at the next step.

    ``sumH, sumL -> __m128i screen``

   used commands: ``_mm_movelh_ps``

**9) Loading result pixels to the Screen array.**

    ``Screen[i] = screen``

   used commands: ``_mm_store_si128``

### AVX2 optimization
in process...

## Perfomance
To evaluate and compare the speed of working algorithms I calculate fps value of 10 cycles of Alpha-Blending

``fps = 1 / blending_time``

It's important to evaluate the time correctly, so when I do it, app don't draw anything in SFML, just operates with arrays of pixels.

Evaluations made with precision ~ ``2%``

|optimization \ flag|None   |-O0 |-O1  |-O2  |-O3      |-Ofast|
|:------------------|:-----:|:--:|:---:|:---:|:-------:|:----:|
|no optimization    |**9.1**|9.7 |12.1 |12.6 |12.5     |12.5  |
|                   |       |    |     |     |         |      |
|SSE inctructions   |9.7    |9.9 |133.0|132.0|**147.0**|145.0 |
|speed increase     |1.1    |1.02|11.0 |15.2 |11.8     |11.6  |
|                   |       |    |     |     |         |      |
|AVX2 instructions  |       |    |     |     |         |      |
|speed increase     |       |    |     |     |         |      |

In the table speed increase calculated by formula

``speed_increase = (optimization_time / no_optimization_time)``

where times are taken with the same set of flags

## Results
The result of running programm in the DRAW_MODE
![Result](Pictures/Result.png)

## Conclusions
- Max speedup is **147,0/9.1 = 16.2**
- Optimizing alpha-blending alogithm by using SSE instructions, a speedup of **147.0/12.5 = 11.8** times was achieved.
