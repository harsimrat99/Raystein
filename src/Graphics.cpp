#include "Graphics.h"

Graphics::Graphics(HWND handle)
{

    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

    m_hwnd = handle;

    RECT rc;
    GetClientRect(m_hwnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(
                           rc.right - rc.left,
                           rc.bottom - rc.top
                       );

    // Create a Direct2D render target.
    hr = m_pDirect2dFactory->CreateHwndRenderTarget(
             D2D1::RenderTargetProperties(),
             D2D1::HwndRenderTargetProperties(m_hwnd, size),
             &m_pRenderTarget
         );

    if (SUCCEEDED(hr))
    {
        // Create a blue brush.
        hr = m_pRenderTarget->CreateSolidColorBrush(
                 D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
                 &m_pLightSlateGrayBrush
             );

    }

    if (SUCCEEDED(hr))
    {

        static const WCHAR msc_fontName[] = L"Verdana";
        static const FLOAT msc_fontSize = 12;

        // Create a DirectWrite factory.
        hr = DWriteCreateFactory(
                 DWRITE_FACTORY_TYPE_SHARED,
                 __uuidof(m_pDWriteFactory),
                 reinterpret_cast<IUnknown **>(&m_pDWriteFactory)
             );

        hr = m_pDWriteFactory->CreateTextFormat(
                 msc_fontName,
                 NULL,
                 DWRITE_FONT_WEIGHT_NORMAL,
                 DWRITE_FONT_STYLE_NORMAL,
                 DWRITE_FONT_STRETCH_NORMAL,
                 msc_fontSize,
                 L"", //locale
                 &m_pTextFormat
             );

    }

    if (SUCCEEDED(hr))
    {
        // Center the text horizontally and vertically.
        m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

        m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    }


}

Graphics::~Graphics()
{

    SafeRelease(&m_pDirect2dFactory);
    SafeRelease(&m_pRenderTarget);
    SafeRelease(&m_pLightSlateGrayBrush);
    SafeRelease(&m_pDWriteFactory);
    SafeRelease(&m_pTextFormat);

}

void Graphics::beginDraw()
{

    m_pRenderTarget->BeginDraw();
    m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

}

void Graphics::endDraw()
{

    m_pRenderTarget->EndDraw();

}

void Graphics::clear(float r, float g, float b)
{

    m_pRenderTarget->Clear(D2D1::ColorF(r,g,b));

}

void Graphics::drawLine(float x1, float y1, float x2, float y2)
{

    //m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(0, getTargetHeight()));

    m_pRenderTarget->DrawLine(
        D2D1::Point2F(x1, getTargetHeight() - y1),
        D2D1::Point2F(x2, getTargetHeight() - y2),
        m_pLightSlateGrayBrush,
        m_stroke_width
    );

    //m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());


}

void Graphics::drawText(const WCHAR* texts, float x1, float y1, float size)
{

    m_pRenderTarget->DrawText(
        texts,
        wcslen(texts),
        m_pTextFormat,
        D2D1::RectF(x1, y1, getTargetWidth(), getTargetHeight()),
        m_pLightSlateGrayBrush
    );

}

void Graphics::drawRect(float x, float y, float w, float h)
{

    D2D1_RECT_F rectangle = D2D1::RectF(
                                x,
                                getTargetHeight() - y - h,
                                x+w,
                                getTargetHeight() - y
                            );

    m_pRenderTarget->DrawRectangle(&rectangle, m_pLightSlateGrayBrush);

}

void Graphics::drawFillRect(float x, float y, float w, float h)
{

    D2D1_RECT_F rectangle = D2D1::RectF(
                                x,
                                getTargetHeight() - y - h,
                                x+w,
                                getTargetHeight() - y
                            );

    m_pRenderTarget->FillRectangle(&rectangle, m_pLightSlateGrayBrush);

}

void Graphics::fill(float r, float g, float b, float a)
{

    m_pLightSlateGrayBrush->SetColor(D2D1::ColorF(r,g,b,a));

}

void Graphics::stroke(float w)
{

    this->m_stroke_width = w;

}

float Graphics::getTargetHeight()
{

    RECT rc;
    GetClientRect(m_hwnd, &rc);
    return rc.bottom - rc.top;

}

float Graphics::getTargetWidth()
{

    RECT rc;
    GetClientRect(m_hwnd, &rc);
    return rc.right - rc.left;

}

ID2D1Bitmap* Graphics::loadImage(LPCWSTR imageFile)
{

    IWICBitmapDecoder* pDecoder = NULL;
    IWICBitmapFrameDecode* pSource = NULL;
    IWICFormatConverter* pConverter = NULL;
    IWICImagingFactory* pIWICFactory = NULL;

    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void**>(&pIWICFactory));

    HRESULT hr = pIWICFactory->CreateDecoderFromFilename(
                     imageFile,
                     NULL,
                     GENERIC_READ,
                     WICDecodeMetadataCacheOnLoad,
                     &pDecoder
                 );

    if (SUCCEEDED(hr))
    {
        // Create the initial frame.
        hr = pDecoder->GetFrame(0, &pSource);
    }

    if (SUCCEEDED(hr))
    {
        // Convert the image format to 32bppPBGRA
        // (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
        hr = pIWICFactory->CreateFormatConverter(&pConverter);
    }

    if (SUCCEEDED(hr))
    {
        hr = pConverter->Initialize(
                 pSource,
                 GUID_WICPixelFormat32bppPBGRA,
                 WICBitmapDitherTypeNone,
                 NULL,
                 0.f,
                 WICBitmapPaletteTypeMedianCut
             );
    }

    ID2D1Bitmap* toReturn = NULL;

    if (SUCCEEDED(hr))
    {

        // Create a Direct2D bitmap from the WIC bitmap.
        hr = m_pRenderTarget->CreateBitmapFromWicBitmap(
                 pConverter,
                 NULL,
                 &toReturn
             );
    }

    SafeRelease(&pIWICFactory);
    SafeRelease(&pDecoder);
    SafeRelease(&pSource);
    SafeRelease(&pConverter);

    return toReturn;


}

void Graphics::drawBitmap( ID2D1Bitmap* bitmap, float x, float y, int w, int h )
{

    D2D1_POINT_2F upperLeftCorner = D2D1::Point2F(x, getTargetHeight() - y);

    // Draw a bitmap.
    m_pRenderTarget->DrawBitmap(
        bitmap,
        D2D1::RectF(
            upperLeftCorner.x,
            upperLeftCorner.y,
            upperLeftCorner.x + w,
            upperLeftCorner.y - h)
    );

}

void Graphics::drawFillEllipse(float x, float y, float r)
{

    m_pRenderTarget->FillEllipse(D2D1::Ellipse(
        D2D1::Point2F(x, getTargetHeight() - y),
        r,
        r
    ), m_pLightSlateGrayBrush);

}

void Graphics::drawEllipse(float x, float y, float r)
{

    m_pRenderTarget->DrawEllipse(D2D1::Ellipse(
        D2D1::Point2F(x, getTargetHeight() - y),
        r,
        r
    ), m_pLightSlateGrayBrush, m_stroke_width);

}

