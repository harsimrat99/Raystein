#ifndef GRAPHICS2D_H
#define GRAPHICS2D_H
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <vector>
#include <stdlib.h>

using namespace Gdiplus;

class Graphics2D
{
    public:

        Graphics2D(HDC handle);
        virtual ~Graphics2D();
        void drawLine(float,float,float,float );
        void drawText(const WCHAR* text, int x1, int y1, int size);
        void drawRect(int, int, int, int);
        int drawImage(Bitmap&, int, int, double scale = 1, bool cache = false);
        void drawCachedImage(int, int, int, double scale = 1);
        void drawFillRect(int, int, int, int);
        void fill (int , int , int , int a = 255);
        void stroke(float w = 1.0f);

    protected:

    private:

        ULONG_PTR m_gdiPlustoken;
        GdiplusStartupInput m_startupToken;
        Graphics* m_graphics;
        Pen* m_pen;
        SolidBrush* m_brush;
        const static int defaultWidth = 2;
        std::vector <CachedBitmap*> m_cachemap;
        CachedBitmap* bmpc;

};
#endif // GRAPHICS2D_H
