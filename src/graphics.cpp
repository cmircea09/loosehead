#include "graphics.hpp"
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#define GRAPHICS_EXCEPT_NO_INFO(hr) Graphics::HrException(__LINE__, __FILE__, (hr))
#define GRAPHICS_THROW_NO_INFO(hr_call) if(FAILED(hr = (hr_call))) throw Graphics::HrException(__LINE__, __FILE__, hr)

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

	Microsoft::WRL::ComPtr<ID3D11Resource>back_buffer;
	_swap_chain->GetBuffer(0, __uuidof(ID3D11Resource), &back_buffer);
	_device->CreateRenderTargetView(back_buffer.Get(), nullptr, &_target_view);
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

void Graphics::draw_triangle_test()
{
	struct Vertex
	{
		float x;
		float y;
	};

	const Vertex vertices[] = 
	{
		{ 0.0f, 0.5f },
		{ 0.5f, -0.5f },
		{ -0.5f, -0.5f }
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>vertex_buffer;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertices;

	_device->CreateBuffer(&bd, &sd, &vertex_buffer);

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	_context->IASetVertexBuffers(0u, 1u, vertex_buffer.GetAddressOf(), &stride, &offset);


	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	D3DReadFileToBlob(L"pixel_shader.cso", &blob);
	_device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixel_shader);	
	_context->PSSetShader(pixel_shader.Get(), nullptr, 0u);


	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	D3DReadFileToBlob(L"vertex_shader.cso", &blob);
	_device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertex_shader);	
	_context->VSSetShader(vertex_shader.Get(), nullptr, 0u);

	Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = 
	{
		{ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	_device->CreateInputLayout(
		ied,
		(UINT)std::size(ied),
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		&input_layout
	);

	_context->IASetInputLayout(input_layout.Get());
	_context->OMSetRenderTargets(1u, _target_view.GetAddressOf(), nullptr);
	_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT vp;
	vp.Width = 640;
	vp.Height = 480;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	_context->RSSetViewports(1u, &vp);

	_context->Draw((UINT)std::size(vertices), 0u);
}