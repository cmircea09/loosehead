#pragma once

#include <d3d11.h>

class Graphics
{
public:
	Graphics(void* window);
	~Graphics();

private:
	ID3D11Device* device = nullptr;
	IDXGISwapChain* swap_chain = nullptr;
	ID3D11DeviceContext* context = nullptr;
	ID3D11RenderTargetView* target_view = nullptr;
};