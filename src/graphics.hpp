#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include "exception.hpp"

class Graphics
{
public:
	class HrException : public Exception
	{
		public:
			HrException(int line, const char* file,
			HRESULT hr, std::vector<std::string> info_msgs = {});
			~HrException() = default;

			const char* what();
			const char* type();
			HRESULT error_code();
			std::string error_string();
			std::string error_description();
			std::string error_info();

		private:
			HRESULT _hr;
			std::string _info;
	};

	class InfoException : public Exception
	{
		public:
			InfoException(int line, const char* file, 
			std::vector<std::string> info_msgs = {});
			~InfoException() = default;

			const char* what();
			const char* type();
			std::string error_info();

		private:
			std::string _info;
	};

	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;

		public:
			const char* type() override;
		
		private:
			std::string _reason;
	};

public:
	Graphics(void* window);
	~Graphics() = default;

	void end_frame();
	void clear_buffer(float r, float g, float b);

	void draw_triangle_test();

private:
	Microsoft::WRL::ComPtr<ID3D11Device>_device;
	Microsoft::WRL::ComPtr<IDXGISwapChain>_swap_chain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>_context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>_target_view;
};