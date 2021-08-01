#pragma once

#include <d3d11.h>
#include <wrl.h>

class Graphics
{
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