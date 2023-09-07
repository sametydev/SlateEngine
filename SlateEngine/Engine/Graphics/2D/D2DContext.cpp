#include <SlateEngine/Engine/Graphics/2D/D2DContext.h>
#include <SlateEngine/Engine/Game/Game.h>

D2DContext* D2DContext::Instance = nullptr;

D2DContext::D2DContext()
{
}

D2DContext::~D2DContext()
{
}

bool D2DContext::CreateDevices()
{
	HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_d2dFactory.GetAddressOf()));
	HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(m_d2dWriteFactory.GetAddressOf())));
	return true;
}

void D2DContext::BeginResize()
{
    assert(m_d2dFactory);
    assert(m_d2dWriteFactory);
    m_solidColorBrush.Reset();
    m_d2dRenderTarget.Reset();
}

void D2DContext::OnResize()
{
    //Direct2D
    ComPtr<IDXGISurface> surface;
    HR(Game::Instance->GetDXSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(surface.GetAddressOf())));
    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
    HRESULT hr = m_d2dFactory->CreateDxgiSurfaceRenderTarget(surface.Get(), &props, m_d2dRenderTarget.GetAddressOf());
    surface.Reset();

    if (hr == S_OK)
    {
        HR(m_d2dRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Orange),
            m_solidColorBrush.GetAddressOf()));

        HR(m_d2dWriteFactory->CreateTextFormat(L"Helvetica", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"en-us",
            m_textFormat.GetAddressOf()));
    }
    else
    {
        assert(m_d2dRenderTarget);
    }
}

void D2DContext::OnRender()
{
    if (m_d2dRenderTarget != nullptr)
    {
        m_d2dRenderTarget->BeginDraw();

        auto view = r2registar.view<C2DText>();
        for (auto entity : view)
        {
            auto RenderableGeometry = view.get<C2DText>(entity);

            RenderableGeometry.OnRender();
        }

        HR(m_d2dRenderTarget->EndDraw());
    }

}

void D2DContext::AddTextForRender(LPCWSTR t, float x, float y, float w, float h)
{
    entt::entity dummy = r2registar.create();
    r2registar.emplace<C2DText>(dummy,t,x,y,w,h);
}
