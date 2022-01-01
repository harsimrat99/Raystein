#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>
#include <string>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <Objbase.h>

template <class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

class Graphics
{
public:

    Graphics(HWND handle);
    virtual ~Graphics();
    void drawLine(float,float,float,float );
    void drawText(const WCHAR* text, float x1, float y1, float size);
    void drawRect(float, float, float, float);
    void drawFillRect(float, float, float, float);
    void drawEllipse(float, float, float);
    void drawFillEllipse(float, float, float);
    void drawBitmap( ID2D1Bitmap* bitmap , float x, float y, int w, int h);
    void fill (float, float, float, float a = 1.0f);
    void stroke(float w = 1.0f);
    void beginDraw();
    void endDraw();
    void clear(float r = 255, float g =  255, float b = 255);
    float getTargetWidth();
    float getTargetHeight();
    ID2D1Bitmap* loadImage(LPCWSTR imageFile);


protected:

private:

    HRESULT hr = S_OK;
    HWND m_hwnd;
    ID2D1Factory* m_pDirect2dFactory;
    ID2D1HwndRenderTarget* m_pRenderTarget;
    ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
    IDWriteTextFormat* m_pTextFormat;
    IDWriteFactory* m_pDWriteFactory;

    float m_stroke_width = 1.0f;

};
#endif // GRAPHICS_H
