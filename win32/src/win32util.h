#ifndef WIN32_UTIL_H
#define WIN32_UTIL_H

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define NOMINMAX

// Windows Header Files
#include <windows.h>
#include <windowsx.h>

#include <d2d1.h>
#pragma comment(lib, "d2d1")

#include <string>
#include <memory>
#include <expected>

namespace direct2d {

	std::unique_ptr<ID2D1Factory> create_factory();

	D2D1_SIZE_U get_size_of_window(HWND m_hwnd);

	std::unique_ptr<ID2D1HwndRenderTarget> create_render_target(ID2D1Factory* factory, HWND m_hwnd, D2D1_SIZE_U size);

	std::unique_ptr<ID2D1SolidColorBrush> create_brush(ID2D1HwndRenderTarget* renderTarget, D2D1::ColorF color);

};

class Win32Error {
public:
	Win32Error() : error_code_{GetLastError()} {}
	explicit Win32Error(DWORD error_code) : error_code_(error_code) {}

	[[nodiscard]] DWORD code() const { return error_code_; }

	[[nodiscard]] std::string message() const {

		auto localFreeDeleter = [](LPTSTR ptr) {
			if (ptr) LocalFree(ptr);
		};

		LPTSTR messageBuffer = nullptr;
		std::unique_ptr<TCHAR, decltype(localFreeDeleter)> guard{nullptr, localFreeDeleter};

		size_t size = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			error_code_,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPTSTR>(&messageBuffer),
			0,
			nullptr
		);

		guard.reset(messageBuffer);

		if (size == 0 || !guard) {
			return "Unknown error";
		}

		int narrow_size = WideCharToMultiByte(CP_UTF8, 0, guard.get(), static_cast<int>(size), nullptr, 0, nullptr, nullptr);
		std::string result(narrow_size, 0);
		WideCharToMultiByte(CP_UTF8, 0, guard.get(), static_cast<int>(size), result.data(), narrow_size, nullptr, nullptr);
		return result;
	}

private:
	DWORD error_code_;
};

#endif //WIN32_UTIL_H