#include "graphics.hpp"

#pragma comment(lib, "d3d11.lib")

Graphics::Graphics(void* window)
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));

	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = (HWND)window;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&_swap_chain,
		&_device,
		nullptr,
		&_context
	);

	ID3D11Resource* back_buffer = nullptr;
	_swap_chain->GetBuffer(0, __uuidof(ID3D11Resource),reinterpret_cast<void**>(&back_buffer));
	_device->CreateRenderTargetView(back_buffer, nullptr, &_target_view);

	back_buffer->Release();
}

void Graphics::end_frame()
{
	_swap_chain->Present(1u, 0u);
}

void Graphics::clear_buffer(float r, float g, float b)
{
	const float color[] = { r, g, b, 1.0f };
	_context->ClearRenderTargetView(_target_view.Get(), color);
}