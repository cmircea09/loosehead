#pragma once

#include "strict_win.hpp"
#include <wrl.h>
#include <vector>
#include <string>
#include <dxgidebug.h>

class DxgiInfoManager
{
public:
	DxgiInfoManager();
	~DxgiInfoManager() = default;

	void set();
	std::vector<std::string> messages();

private:
	unsigned long long _next = 0u;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> _dxgi_info_queue;
};