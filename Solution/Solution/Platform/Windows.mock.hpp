
#pragma once

#if BOOST_OS_WINDOWS

// Без Windows.h не компилируется
#include <alicorn/platform/windows.hpp>
//#include <windef.h>
//#include <winuser.h>

#define NOGDICAPMASKS
//#define NOVIRTUALKEYCODES
//#define NOWINMESSAGES
//#define NOWINSTYLES
//#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
//#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
//#define NOGDI
#define NOKERNEL
//#define NOUSER
#define NONLS
//#define NOMB
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
//#define NOMSG
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
//#define NOWINOFFSETS
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX

#include <Windows.h>
#undef GetCommandLine

inline bool operator== (const MSG & _Left, const MSG & _Right)
{
  if (_Left.message != _Right.message) return false;
  if (_Left.hwnd != _Right.hwnd) return false;
  if (_Left.wParam != _Right.wParam) return false;
  if (_Left.lParam != _Right.lParam) return false;
  if (_Left.time != _Right.time) return false;
  if (_Left.pt.x != _Right.pt.x) return false;
  if (_Left.pt.y != _Right.pt.y) return false;
  return true;
}

namespace mock
{

class WindowsProxy :
  public ::alicorn::extension::testing::Proxy<WindowsProxy>
{
public:
  MOCK_METHOD4(MessageBoxA, int (HWND, ::std::string, ::std::string, UINT));
  MOCK_METHOD1(PostQuitMessage, void(int));
  MOCK_METHOD1(RegisterClassExW, ATOM(WNDCLASSEXW *));
  MOCK_METHOD1(GetModuleHandleW, HMODULE(LPCWSTR));
  MOCK_METHOD2(UnregisterClassW, BOOL(::std::wstring, HINSTANCE));
  MOCK_METHOD2(GetWindowLongPtrW, LONG_PTR (HWND, int));
  MOCK_METHOD2(SetWindowLongPtrW, LONG_PTR (HWND, int));
  MOCK_METHOD4(DefWindowProcW, LRESULT(HWND, UINT, WPARAM, LPARAM));
  MOCK_METHOD4(PeekMessageW, BOOL(HWND, UINT, UINT, UINT));
  MOCK_METHOD3(GetMessageW, BOOL(HWND, UINT, UINT));
  MOCK_METHOD0(BuildMessage, MSG(void));
  MOCK_METHOD1(TranslateMessage, BOOL(MSG));
  MOCK_METHOD1(DispatchMessageW, LRESULT(MSG));
  MOCK_METHOD1(Sleep, void(DWORD));
  MOCK_METHOD10(CreateWindowExW1, void(DWORD, ::std::wstring, ::std::wstring, 
    DWORD, int, int, int, int, HWND, HMENU));
  MOCK_METHOD2(CreateWindowExW2, HWND(HINSTANCE, LPVOID));
  MOCK_METHOD1(DestroyWindow, BOOL(HWND));
  MOCK_METHOD2(ShowWindow, BOOL(HWND, int));
  MOCK_METHOD1(GetUserDefaultLocaleName, int(int));
  MOCK_METHOD0(GetLocaleName, ::std::wstring(void));
  MOCK_METHOD2(LoadCursorW, HCURSOR (HINSTANCE, ULONG_PTR));
  MOCK_METHOD2(LoadIconW, HICON(HINSTANCE, ::std::wstring));
  MOCK_METHOD1(GetDC, HDC (HWND));
  MOCK_METHOD2(ReleaseDC, int (HWND, HDC));
  MOCK_METHOD2(ChoosePixelFormat, int (HDC, const PIXELFORMATDESCRIPTOR *));
  MOCK_METHOD3(SetPixelFormat, BOOL (HDC, int, const PIXELFORMATDESCRIPTOR *));
  MOCK_METHOD1(wglCreateContext, HGLRC (HDC));
  MOCK_METHOD2(wglMakeCurrent, BOOL (HDC, HGLRC));
  MOCK_METHOD1(wglDeleteContext, BOOL (HGLRC _hRc));
  MOCK_METHOD0(BuildRect, RECT(void));
  MOCK_METHOD1(GetClientRect, BOOL (HWND));
  MOCK_METHOD1(SwapBuffers, BOOL (HDC));
  MOCK_METHOD0(GetCommandLineA, LPSTR (VOID));
  MOCK_METHOD4(SetClientRect, void(LONG, LONG, LONG, LONG));
  MOCK_METHOD3(AdjustWindowRectEx, BOOL (DWORD, BOOL, DWORD));
  MOCK_METHOD1(GetSystemMetrics, int (int));

public:
  LONG_PTR m_NewLongPtr = NULL;
};

inline int MessageBoxA(HWND _hWnd, LPCSTR _Text, LPCSTR _Caption, UINT _Type)
{
  return WindowsProxy::GetInstance()->MessageBoxA(_hWnd, _Text, _Caption, _Type);
}

inline void PostQuitMessage(int _ExitCode)
{
  WindowsProxy::GetInstance()->PostQuitMessage(_ExitCode);
}

inline HMODULE GetModuleHandleW(LPCWSTR _ModuleName)
{
  return WindowsProxy::GetInstance()->GetModuleHandleW(_ModuleName);
}

inline ATOM RegisterClassExW(WNDCLASSEXW * _pWindowClass)
{
  return WindowsProxy::GetInstance()->RegisterClassExW(_pWindowClass);
}

inline BOOL UnregisterClassW(LPCWSTR _ClassName, HINSTANCE _hInstance)
{
  return WindowsProxy::GetInstance()->UnregisterClassW(_ClassName, _hInstance);
}

inline LONG_PTR GetWindowLongPtrW(HWND _hWnd, int _Index)
{
  return WindowsProxy::GetInstance()->GetWindowLongPtrW(_hWnd, _Index);
}

inline LONG_PTR SetWindowLongPtrW(HWND _hWnd, int _Index, LONG_PTR _NewLong)
{
  WindowsProxy::GetInstance()->m_NewLongPtr = _NewLong;
  return WindowsProxy::GetInstance()->SetWindowLongPtrW(_hWnd, _Index);
}

inline LRESULT DefWindowProcW(HWND _hWnd, UINT _Msg, WPARAM _wParam, LPARAM _lParam)
{
  return WindowsProxy::GetInstance()->DefWindowProcW(_hWnd, _Msg, _wParam, _lParam);
}

inline BOOL PeekMessageW(MSG * _pMsg, HWND _hWnd, UINT _MsgFilterMin,
  UINT _MsgFilterMax, UINT _RemoveMsg)
{
  const auto Result = WindowsProxy::GetInstance()->PeekMessageW(_hWnd,
    _MsgFilterMin, _MsgFilterMax, _RemoveMsg);

  if (Result != FALSE)
  {
    (*_pMsg) = WindowsProxy::GetInstance()->BuildMessage();
  }

  return Result;
}

inline BOOL GetMessageW(MSG * _pMsg, HWND _hWnd, 
  UINT _MsgFilterMin, UINT _MsgFilterMax)
{
  const auto Result = WindowsProxy::GetInstance()->GetMessageW(_hWnd,
    _MsgFilterMin, _MsgFilterMax);

  if (Result != -1)
  {
    (*_pMsg) = WindowsProxy::GetInstance()->BuildMessage();
  }

  return Result;
}

inline VOID Sleep(DWORD _Milliseconds)
{
  WindowsProxy::GetInstance()->Sleep(_Milliseconds);
}

inline BOOL TranslateMessage(const MSG * _pMsg)
{
  return WindowsProxy::GetInstance()->TranslateMessage(*_pMsg);
}

inline LRESULT DispatchMessageW(const MSG * _pMsg)
{
  return WindowsProxy::GetInstance()->DispatchMessageW(*_pMsg);
}

inline HWND CreateWindowExW(DWORD _ExStyle, LPCWSTR _ClassName, 
  LPCWSTR _WindowName, DWORD _Style, int _X, int _Y, int _Width, int _Height,
  HWND _hWndParent, HMENU _hMenu, HINSTANCE _hInstance, LPVOID _pParam)
{
  WindowsProxy::GetInstance()->CreateWindowExW1(_ExStyle, _ClassName,
    _WindowName, _Style, _X, _Y, _Width, _Height, _hWndParent, _hMenu);
  return WindowsProxy::GetInstance()->CreateWindowExW2(_hInstance, _pParam);
}

inline BOOL DestroyWindow(HWND _hWnd)
{
  return WindowsProxy::GetInstance()->DestroyWindow(_hWnd);
}

inline BOOL ShowWindow(HWND _hWnd, int _CmdShow)
{
  return WindowsProxy::GetInstance()->ShowWindow(_hWnd, _CmdShow);
}

inline int GetUserDefaultLocaleName(LPWSTR _LocaleName, int _LocaleNameCount)
{
  const auto LocaleName = WindowsProxy::GetInstance()->GetLocaleName();
  memcpy_s(_LocaleName, _LocaleNameCount * sizeof(wchar_t), 
    LocaleName.data(), LocaleName.size() * sizeof(wchar_t));

  return WindowsProxy::GetInstance()->GetUserDefaultLocaleName(_LocaleNameCount);
}

inline HCURSOR LoadCursorW(HINSTANCE _hInstance, LPCWSTR _pCursorName)
{
  return WindowsProxy::GetInstance()->LoadCursorW(_hInstance, (ULONG_PTR)_pCursorName);
}

inline HICON LoadIconW(HINSTANCE _hInstance, LPCWSTR _pIconName)
{
  return WindowsProxy::GetInstance()->LoadIconW(_hInstance, _pIconName);
}

inline HDC GetDC(HWND _hWnd)
{
  return WindowsProxy::GetInstance()->GetDC(_hWnd);
}

inline int ReleaseDC(HWND _hWnd, HDC _hDC)
{
  return WindowsProxy::GetInstance()->ReleaseDC(_hWnd, _hDC);
}

inline int ChoosePixelFormat(HDC _hDc, const PIXELFORMATDESCRIPTOR * _pPfd)
{
  return WindowsProxy::GetInstance()->ChoosePixelFormat(_hDc, _pPfd);
}

inline BOOL SetPixelFormat(HDC _hDc, int _PixelFormat,
  const PIXELFORMATDESCRIPTOR * _pPfd)
{
  return WindowsProxy::GetInstance()->SetPixelFormat(_hDc, _PixelFormat, _pPfd);
}

inline HGLRC wglCreateContext(HDC _hDc)
{
  return WindowsProxy::GetInstance()->wglCreateContext(_hDc);
}

inline BOOL wglMakeCurrent(HDC _hDc, HGLRC _hRc)
{
  return WindowsProxy::GetInstance()->wglMakeCurrent(_hDc, _hRc);
}

inline BOOL wglDeleteContext(HGLRC _hRc)
{
  return WindowsProxy::GetInstance()->wglDeleteContext(_hRc);
}

inline BOOL GetClientRect(HWND _hWnd, LPRECT _pRect)
{
  *_pRect = WindowsProxy::GetInstance()->BuildRect();
  return WindowsProxy::GetInstance()->GetClientRect(_hWnd);
}

inline BOOL SwapBuffers(HDC _hDc)
{
  return WindowsProxy::GetInstance()->SwapBuffers(_hDc);
}

inline LPSTR GetCommandLineA(VOID)
{
  return WindowsProxy::GetInstance()->GetCommandLineA();
}

inline BOOL AdjustWindowRectEx(RECT * _pRect, DWORD _Style, BOOL _IsMenu,
  DWORD _ExStyle)
{
  WindowsProxy::GetInstance()->SetClientRect(
    _pRect->left, _pRect->top, _pRect->right, _pRect->bottom);
  *_pRect = WindowsProxy::GetInstance()->BuildRect();
  return WindowsProxy::GetInstance()->AdjustWindowRectEx(_Style, _IsMenu, _ExStyle);
}

inline int GetSystemMetrics(int _Index)
{
  return WindowsProxy::GetInstance()->GetSystemMetrics(_Index);
}

} // namespace mock

namespace covellite
{

namespace app
{

using ::mock::MessageBoxA;
using ::mock::GetModuleHandleW;
using ::mock::RegisterClassExW;
using ::mock::UnregisterClassW;
using ::mock::GetWindowLongPtrW;
using ::mock::DefWindowProcW;
using ::mock::PeekMessageW;
using ::mock::GetMessageW;
using ::mock::Sleep;
using ::mock::TranslateMessage;
using ::mock::DispatchMessage;
using ::mock::LoadCursorW;
using ::mock::LoadIconW;
using ::mock::GetCommandLineA;
using ::mock::PostQuitMessage;

} // namespace app

namespace os
{

using ::mock::GetModuleHandleW;
using ::mock::CreateWindowEx;
using ::mock::ShowWindow;
using ::mock::DestroyWindow;
using ::mock::GetUserDefaultLocaleName;
using ::mock::AdjustWindowRectEx;
using ::mock::GetSystemMetrics;
using ::mock::PostQuitMessage;
using ::mock::SetWindowLongPtrW;
using ::mock::GetClientRect;

} // namespace os

namespace api
{

using ::mock::GetDC;
using ::mock::ReleaseDC;
using ::mock::ChoosePixelFormat;
using ::mock::SetPixelFormat;
using ::mock::wglCreateContext;
using ::mock::wglMakeCurrent;
using ::mock::wglDeleteContext;
using ::mock::GetClientRect;
using ::mock::SwapBuffers;

} // namespace api

} // namespace covellite

#endif
