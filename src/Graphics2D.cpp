#include "Graphics2D.h"

Graphics2D::Graphics2D(HDC handle)
{

    GdiplusStartup(&m_gdiPlustoken, &m_startupToken, NULL);

    this->m_graphics = new Graphics(handle);

    this->m_brush = new SolidBrush(Color(255, 0, 0, 255));

    this->m_pen = new Pen(m_brush, defaultWidth);

    m_graphics->SetCompositingMode( CompositingModeSourceCopy );

    m_graphics->SetCompositingQuality( CompositingQualityHighSpeed );

    m_graphics->SetPixelOffsetMode( PixelOffsetModeNone );

    m_graphics->SetSmoothingMode( SmoothingModeNone );

    m_graphics->SetInterpolationMode( InterpolationModeDefault );


}

Graphics2D::~Graphics2D()
{
    GdiplusShutdown(m_gdiPlustoken);

    delete this->m_graphics;

    delete this->m_pen;

    delete this->m_brush;

    m_cachemap.clear();

    delete bmpc;
}

void Graphics2D::drawLine(float x1, float y1, float x2, float y2){

    m_graphics->DrawLine(m_pen, x1, y1, x2, y2);

}

void Graphics2D::drawText(const WCHAR* text, int x1, int y1, int size){

    Font        font(L"Arial", size, FontStyleRegular, UnitPixel, NULL);
    PointF p(x1,y1);
    m_graphics->DrawString(text, -1, &font, p , m_brush);

}

void Graphics2D::drawRect(int x, int y, int w, int h){

    m_graphics->DrawRectangle(m_pen, x, y, w, h);
}

void Graphics2D::drawFillRect(int x, int y, int w, int h){

    m_graphics->FillRectangle(m_brush, x, y, w, h);
}


void Graphics2D::fill(int r, int g, int b, int a){

    m_brush->SetColor(Color(a,r,g,b));

    //m_pen->SetBrush(m_brush);

}

void Graphics2D::stroke(float w){
    m_pen->SetWidth(w);
}

int Graphics2D::drawImage(Bitmap& image, int x, int y, double scale, bool cache){

    m_graphics->DrawImage(&image, x,y,  scale *image.GetWidth(),  scale * image.GetHeight());

    bmpc = new CachedBitmap(&image, m_graphics);

    return 0;
}

void Graphics2D::drawCachedImage(int pos, int x, int y, double scale ){

    m_graphics->DrawCachedBitmap(bmpc, x,y);

}

