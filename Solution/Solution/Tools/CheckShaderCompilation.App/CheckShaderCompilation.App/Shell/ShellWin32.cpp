/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "stdafx.h"
#include "Shell.h"
#include <windows.h>
#include <memory>
//#include "ShellFileInterface.h"

static LRESULT CALLBACK WindowProcedure(HWND , UINT, WPARAM, LPARAM);

static bool running = false;
static const TCHAR * instance_name = nullptr;
static HWND window_handle = nullptr;
static HINSTANCE instance_handle = nullptr;

static double time_frequency;
static LARGE_INTEGER time_startup;

//static ::std::unique_ptr<ShellFileInterface> file_interface;

bool Shell::Initialise(const ::std::string & /*path*/)
{
  instance_handle = GetModuleHandle(nullptr);

  LARGE_INTEGER time_ticks_per_second;
  QueryPerformanceFrequency(&time_ticks_per_second);
  QueryPerformanceCounter(&time_startup);

  time_frequency = 1.0 / (double) time_ticks_per_second.QuadPart;

  // Fetch the path of the executable, append the path onto that.
  char executable_file_name[MAX_PATH];

  if (GetModuleFileNameA(instance_handle, executable_file_name, MAX_PATH) >= MAX_PATH &&
      GetLastError() == ERROR_INSUFFICIENT_BUFFER)
    executable_file_name[0] = 0;

  //auto executable_path = GuiLib::String(executable_file_name);
  //executable_path = executable_path.substr(0, executable_path.rfind('\\') + 1);
  //file_interface = ::std::make_unique<ShellFileInterface>(executable_path + path);
  //GuiLib::SetFileInterface(file_interface.get());

  return true;
}

//void Shell::Shutdown()
//{
//  file_interface.reset();
//}

static ShellRenderInterfaceExtensions * shell_renderer = nullptr;

bool Shell::OpenWindow(const TCHAR * name,
                       ShellRenderInterfaceExtensions * _shell_renderer,
                       unsigned int width,
                       unsigned int height,
                       bool allow_resize,
                       bool _IsShowWindow)
{
  WNDCLASS window_class;

  // Fill out the window class struct.
  window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  window_class.lpfnWndProc = WindowProcedure;
  window_class.cbClsExtra = 0;
  window_class.cbWndExtra = 0;
  window_class.hInstance = instance_handle;
  window_class.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
  window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
  window_class.hbrBackground = nullptr;
  window_class.lpszMenuName = nullptr;
  window_class.lpszClassName = name;

  if (!RegisterClass(&window_class))
  {
    DisplayError("Could not register window class.");

    CloseWindow();
    return false;
  }

  window_handle = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
                                 name,	// Window class name.
                                 name,
                                 WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
                                 0, 0,	// Window position.
                                 width, height,// Window size.
                                 nullptr,
                                 nullptr,
                                 instance_handle,
                                 nullptr);

  if (!window_handle)
  {
    DisplayError("Could not create window.");
    CloseWindow();

    return false;
  }

  instance_name = name;

  const DWORD style = (allow_resize ?
                       WS_OVERLAPPEDWINDOW :
                       (WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX));
  const DWORD extended_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

  // Adjust the window size to take into account the edges
  RECT window_rect;
  window_rect.top = 0;
  window_rect.left = 0;
  window_rect.right = width;
  window_rect.bottom = height;
  AdjustWindowRectEx(&window_rect, style, FALSE, extended_style);

  SetWindowLong(window_handle, GWL_EXSTYLE, extended_style);
  SetWindowLong(window_handle, GWL_STYLE, style);

  if (_shell_renderer != nullptr)
  {
    shell_renderer = _shell_renderer;

    if (!shell_renderer->AttachToNative(window_handle))
    {
      CloseWindow();
      return false;
    }
  }

  // Resize the window.
  SetWindowPos(window_handle, HWND_TOP, 0, 0, window_rect.right - window_rect.left,
               window_rect.bottom - window_rect.top, SWP_NOACTIVATE);

  // Display the new window
  ShowWindow(window_handle, (_IsShowWindow) ? SW_SHOW : SW_HIDE);
  //SetForegroundWindow(window_handle);
  //SetFocus(window_handle);

  return true;
}

void Shell::CloseWindow()
{
  if (shell_renderer)
  {
    shell_renderer->DetachFromNative();
  }

  DestroyWindow(window_handle);
  UnregisterClass(instance_name, instance_handle);
}

void Shell::DisplayError(const char * fmt, ...)
{
  const int buffer_size = 1024;
  char buffer[buffer_size];
  va_list argument_list;

  // Print the message to the buffer.
  va_start(argument_list, fmt);
  int len = vsnprintf(buffer, buffer_size - 2, fmt, argument_list);

  if (len < 0 || len > buffer_size - 2)
  {
    len = buffer_size - 2;
  }

  buffer[len] = '\n';
  buffer[len + 1] = '\0';
  va_end(argument_list);

  MessageBoxA(window_handle, buffer, "Shell Error", MB_OK);
}

//void Shell::Log(const char * fmt, ...)
//{
//  const int buffer_size = 1024;
//  char buffer[buffer_size];
//  va_list argument_list;
//
//  // Print the message to the buffer.
//  va_start(argument_list, fmt);
//  int len = vsnprintf(buffer, buffer_size - 2, fmt, argument_list);
//
//  if (len < 0 || len > buffer_size - 2)
//  {
//    len = buffer_size - 2;
//  }
//
//  buffer[len] = '\n';
//  buffer[len + 1] = '\0';
//  va_end(argument_list);
//
//  OutputDebugStringA(buffer);
//}

//float Shell::GetElapsedTime()
//{
//  LARGE_INTEGER counter;
//  QueryPerformanceCounter(&counter);
//
//  return (float)((counter.QuadPart - time_startup.QuadPart) * time_frequency);
//}

//void Shell::EventLoop(ShellIdleFunction idle_function)
//{
//  MSG message;
//  running = true;
//
//  while (running)
//  {
//    if (PeekMessage(&message, NULL, 0, 0, PM_NOREMOVE))
//    {
//      GetMessage(&message, NULL, 0, 0);
//
//      TranslateMessage(&message);
//      DispatchMessage(&message);
//    }
//
//    idle_function();
//  }
//}

static LRESULT CALLBACK WindowProcedure(HWND _hWnd, UINT message, WPARAM w_param, LPARAM l_param)
{
  if (message == WM_CLOSE)
  {
    running = false;
    return 0;
  }

  return DefWindowProc(_hWnd, message, w_param, l_param);
}
