#include "graphics.hpp"
#include "dxerr/dxerr.hpp"
#include <d3dcompiler.h>
#include <sstream>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#define GRAPHICS_EXCEPT_NO_INFO(hr) Graphics::HrException(__LINE__, __FILE__, (hr))
#define GRAPHICS_THROW_NO_INFO(hr_call) if(FAILED(hr = (hr_call))) throw Graphics::HrException(__LINE__, __FILE__, hr)

#define GRAPHICS_EXCEPTION(hr) Graphics::HrException(__LINE__, __FILE__, (hr), _info_manager.messages())
#define GRAPHICS_THROW_INFO(hrcall) _info_manager.set(); if(FAILED(hr = (hrcall))) throw GRAPHICS_EXCEPTION(hr)
#define GRAPHICS_THROW_INFO_ONLY(call) _info_manager.set(); (call); {auto v = _info_manager.messages(); if(!v.empty()) {throw Graphics::InfoException(__LINE__, __FILE__, v);}}

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

	HRESULT hr;
	GRAPHICS_THROW_INFO(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&_swap_chain,
		&_device,
		nullptr,
		&_context
	));

	Microsoft::WRL::ComPtr<ID3D11Resource>back_buffer;
	GRAPHICS_THROW_INFO(_swap_chain->GetBuffer(0, __uuidof(ID3D11Resource), &back_buffer));
	GRAPHICS_THROW_INFO(_device->CreateRenderTargetView(back_buffer.Get(), nullptr, &_target_view));
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

Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> info_msgs) 
noexcept : GraphicsException(line, file)
{
	Exception(line, file);
	_hr = hr;

	for(const auto &i : info_msgs) {
		_info += i;
		_info.push_back('\n');
	}

	if(!_info.empty())
		_info.pop_back();
}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << type() << "\n" << "[Error Code] 0x" << std::hex << std::uppercase
	    << error_code() << std::dec << " (" << (unsigned long)error_code()
	    << " )" << "\n" << "[Error String] " << error_string()
	    << std::endl << "[Description] " << error_description() << "\n";

	if(!_info.empty())
		oss << "\n[Error Info]\n" << error_info() << "\n\n";

	oss << origin_string();
	
	return (what_buffer = oss.str()).c_str();
}

const char* Graphics::HrException::type() const noexcept
{
	return "Loosehead Exception";
}

HRESULT Graphics::HrException::error_code() const noexcept
{
	return _hr;
}

std::string Graphics::HrException::error_string() const noexcept
{
	return DXGetErrorString(_hr);
}

std::string Graphics::HrException::error_description() const noexcept
{
	char buffer[512];
	DXGetErrorDescription(_hr, buffer, sizeof(buffer));
	return buffer;
}

std::string Graphics::HrException::error_info() const noexcept
{
	return _info;
}


const char* Graphics::DeviceRemovedException::type() const noexcept
{
	return "Loosehead Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> info_msgs)
noexcept : GraphicsException(line, file)
{
	for(const auto &i : info_msgs) {
		_info += i;
		_info.push_back('\n');
	}

	if(!_info.empty())
		_info.pop_back();
}

const char* Graphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << type() << "\n" << "\n[Error Info]\n" << error_info() << "\n\n"
	    << origin_string();

	return (what_buffer = oss.str()).c_str();
}

const char* Graphics::InfoException::type() const noexcept
{
	return "Loosehead Graphics Info Exception";
}

std::string Graphics::InfoException::error_info() const noexcept
{
	return _info;
}