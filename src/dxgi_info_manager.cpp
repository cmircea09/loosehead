#include "dxgi_info_manager.hpp"
#include "graphics.hpp"
#include <dxgidebug.h>
#include <memory>

#pragma comment(lib, "dxguid.lib")

#define GRAPHICS_THROW_NO_INFO(hr_call) if(FAILED(hr = (hr_call))) throw Graphics::HrException(__LINE__, __FILE__, hr)

DxgiInfoManager::DxgiInfoManager()
{
	typedef HRESULT (WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	const auto dxgi_debug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);

	const auto dxgi_debug_interface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(dxgi_debug, "DXGIGetDebugInterface" )));

	HRESULT hr;
	GRAPHICS_THROW_NO_INFO(dxgi_debug_interface(__uuidof(IDXGIInfoQueue), &_dxgi_info_queue));
}

void DxgiInfoManager::set()
{
	_next = _dxgi_info_queue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DxgiInfoManager::messages()
{
	std::vector<std::string> messages;
	const auto end = _dxgi_info_queue->GetNumStoredMessages(DXGI_DEBUG_ALL);

	for(auto i = _next; i < end; i++) {
		HRESULT hr;
		SIZE_T message_length;

		GRAPHICS_THROW_NO_INFO(_dxgi_info_queue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &message_length));

		auto bytes = std::make_unique<byte[]>(message_length);
		auto message = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

		GRAPHICS_THROW_NO_INFO(_dxgi_info_queue->GetMessage(DXGI_DEBUG_ALL, i, message, &message_length));
		messages.emplace_back(message->pDescription);
	}

	return messages;
}