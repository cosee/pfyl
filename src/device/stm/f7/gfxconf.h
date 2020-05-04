//
// Created by mohamad on 29.01.20.
//

#ifndef F7_DEVICE_GFXCONF_H
#define F7_DEVICE_GFXCONF_H

#define GFX_OS_NO_INIT		TRUE
#define GFX_USE_OS_FREERTOS		TRUE
#define GFX_USE_GDISP                   GFXON

/* Features for the GDISP sub-system. */
#ifdef SINE_WAVE_BENCHMARK
#define GDISP_NEED_VALIDATION           GFXOFF
#define GDISP_NEED_CLIP                 GFXOFF
#define GDISP_NEED_TEXT                 GFXOFF
#define GDISP_NEED_CIRCLE               GFXON
#define GDISP_NEED_ELLIPSE              GFXOFF
#define GDISP_NEED_ARC                  GFXOFF
#define GDISP_NEED_SCROLL               GFXOFF
#define GDISP_NEED_PIXELREAD            GFXOFF
#define GDISP_NEED_CONTROL              GFXOFF
#define GDISP_NEED_MULTITHREAD          GFXOFF
#endif
#endif //F7_DEVICE_GFXCONF_H
