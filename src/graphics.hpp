#pragma once

#include "strict_win.hpp"
#include "exception.hpp"
#include "info_manager.hpp"
#include <d3d11.h>
#include <wrl.h>
#include <vector>

class Graphics
{
public:
	class GraphicsException : public Exception
	{
		using Exception::Exception;
	};

	class HrException : public GraphicsException
	{
		public:
			HrException(int line, const char* file, HRESULT hr, std::vector<std::string> info_msgs = {}) noexcept;

			const char* what() const noexcept override;;
			const char* type() const noexcept override;;
			HRESULT error_code() const noexcept;;
			std::string error_string() const noexcept;;
			std::string error_description() const noexcept;;
			std::string error_info() const noexcept;;

		private:
			HRESULT _hr;
			std::string _info;
	};

	class InfoException : public GraphicsException
	{
		public:
			InfoException(int line, const char* file, std::vector<std::string> info_msgs = {}) noexcept;

			const char* what() const noexcept override;;
			const char* type() const noexcept override;;
			std::string error_info() const noexcept;;

		private:
			std::string _info;
	};

	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;

		public:
			const char* type() const noexcept override;
		
		private:
			std::string _reason;
	};

public:
	Graphics(void* window);
	~Graphics() = default;

	void end_frame();
	void clear_buffer(float r, float g, float b);

private:
	InfoManager _info_manager;

	Microsoft::WRL::ComPtr<ID3D11Device>_device;
	Microsoft::WRL::ComPtr<IDXGISwapChain>_swap_chain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>_context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>_target_view;
};