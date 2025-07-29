#ifndef WIN32_UTIL_H
#define WIN32_UTIL_H

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define NOMINMAX

// Windows Header Files
#include <windows.h>
#include <windowsx.h>

#include <string>
#include <memory>
#include <expected>

class Win32Error {
public:
	explicit Win32Error(const DWORD error_code) : error_code_(error_code) {}

	static auto fromLastError() -> std::unexpected<Win32Error>
	{
		return std::unexpected(Win32Error(GetLastError()));
	}

	[[nodiscard]] auto code() const { return error_code_; }

	[[nodiscard]] auto message() const -> std::wstring {

		LPTSTR messageBuffer = nullptr;

		const size_t size = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			error_code_,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPTSTR>(&messageBuffer),
			0,
			nullptr
		);

		if (size == 0 || !messageBuffer) {
			return L"Unknown error";
		}
		return std::wstring(messageBuffer, size);
	}

private:
	DWORD error_code_;
};

#endif //WIN32_UTIL_H