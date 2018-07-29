
#include "stdafx.h"
#include <Covellite\Api\OpenGL.hpp>
#include <alicorn\std\string.hpp>
#include <alicorn\platform\winapi-check.hpp>
#include <Covellite\Core\EventHandler.hpp>
#include <Covellite\Events.hpp>
#include <Covellite\App\Events.hpp>
#include <Covellite\Os\IWindow.hpp>
#include <Covellite\Os\Events.hpp>
#include <Covellite\Api\Events.hpp>
#include <Covellite\Api\RenderOpenGL.hpp>

using namespace covellite::api;

static PIXELFORMATDESCRIPTOR PixelFormatDescriptor = {0};

OpenGL::OpenGL(const WindowOs_t & _Window) :
  m_Events(_Window),
  m_hWnd(_Window.GetHandle()),
  m_hDeviceContex(USING_MOCK ::GetDC(m_hWnd))
{
  WINAPI_CHECK (m_hDeviceContex != NULL);

  PixelFormatDescriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  PixelFormatDescriptor.nVersion = 1;
  PixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  PixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
  PixelFormatDescriptor.cColorBits = 32;
  PixelFormatDescriptor.cAlphaBits = 8;
  PixelFormatDescriptor.cDepthBits = 32;
  PixelFormatDescriptor.iLayerType = PFD_MAIN_PLANE;

  // Игнорирование предупреждений при анализе кода (анализатор не понимает,
  // что макрос WINAPI_CHECK вызывает исключение при нулевом значении 
  // m_hDeviceContex и последующий код не может быть выполнен).
# pragma warning(push)
# pragma warning(disable: 6387)

  const auto PixelFormat = USING_MOCK ::ChoosePixelFormat(m_hDeviceContex, 
    &PixelFormatDescriptor);
  WINAPI_CHECK (PixelFormat != 0);

  WINAPI_CHECK USING_MOCK ::SetPixelFormat(m_hDeviceContex, PixelFormat, 
    &PixelFormatDescriptor);

# pragma warning(pop)

  m_hRenderContex = USING_MOCK ::wglCreateContext(m_hDeviceContex);
  WINAPI_CHECK (m_hRenderContex != NULL);

  WINAPI_CHECK USING_MOCK ::wglMakeCurrent(m_hDeviceContex, m_hRenderContex);

  UpdateWindow(GetWidth(), GetHeight());

  using namespace ::covellite::events;

  m_Events[Application.Update].Connect([&](void)
  {
    ClearWindow();
    m_Events[Drawing.Do]();
    WINAPI_CHECK USING_MOCK ::SwapBuffers(m_hDeviceContex);
  });

  m_Events[Window.Resize].Connect([&](void) 
  {
    const auto Rect = GetClientRect();
    UpdateWindow(Rect.Width, Rect.Height);
  });
}

OpenGL::~OpenGL(void) noexcept
{
  USING_MOCK ::wglMakeCurrent(NULL, NULL);
  USING_MOCK ::wglDeleteContext(m_hRenderContex);

  USING_MOCK ::ReleaseDC(m_hWnd, m_hDeviceContex);
}

/**
* \brief
*  Функция подписки на события приложения.
*/
void OpenGL::Subscribe(const EventHandlerPtr_t & _pEvents) /*override*/
{
#pragma warning(push)
#pragma warning(disable: 4996)
  using namespace ::covellite::core;
#pragma warning(pop)

  (*_pEvents)[Event::Resize]
    .connect([&](const Params &) { UpdateWindow(GetWidth(), GetHeight()); });
  (*_pEvents)[Event::StartDrawing]
    .connect([&](const Params &) { ClearWindow(); });
  (*_pEvents)[Event::FinishDrawing]
    .connect([&](const params::Empty &) 
      { WINAPI_CHECK USING_MOCK ::SwapBuffers(m_hDeviceContex); });
}

/**
* \brief
*  Оператор доступа к объекту событий фреймворка.
*/
OpenGL::operator OpenGL::Events_t (void) const /*override*/
{
  return m_Events;
}

/**
* \brief
*  Функция получения строки текущей версии и типа используемого графического
*  API.
*/
auto OpenGL::GetUsingApi(void) const -> String_t /*override*/
{
  using namespace ::alicorn::extension::std;

  auto Version = reinterpret_cast<const char *>(glGetString(GL_VERSION));

  return uT("OpenGL %VERSION%").Replace(uT("%VERSION%"),
    string_cast<String, Locale::Default>(::std::string{ Version }));
}

/**
* \brief
*  Функция получения текущей ширины окна.
*/
int32_t OpenGL::GetWidth(void) const /*override*/
{
  RECT ClientRect;
  WINAPI_CHECK USING_MOCK ::GetClientRect(m_hWnd, &ClientRect);
  return (ClientRect.right - ClientRect.left);
}

/**
* \brief
*  Функция получения текущей высоты окна.
*/
int32_t OpenGL::GetHeight(void) const /*override*/
{
  RECT ClientRect;
  WINAPI_CHECK USING_MOCK ::GetClientRect(m_hWnd, &ClientRect);
  return (ClientRect.bottom - ClientRect.top);
}

/**
* \brief
*  Функция получения положения и размеров клинтской области окна программы.
*/
OpenGL::Rect OpenGL::GetClientRect(void) const /*override*/
{
  RECT ClientRect;
  WINAPI_CHECK USING_MOCK ::GetClientRect(m_hWnd, &ClientRect);
  return { 0, 0,
    ClientRect.right - ClientRect.left, ClientRect.bottom - ClientRect.top };
}

/**
* \brief
*  Функция создания объекта Rocket::Core::RenderInterface.
*/
OpenGL::RenderInterfacePtr_t OpenGL::MakeRenderInterface(void) const /*override*/
{
  // 22 Июль 2018 11:01 (unicornum.verum@gmail.com)
  TODO("Реализация совпадает с OpenGLES() - обобщить.");

  return ::std::make_shared<covellite::api::RenderOpenGL>(GetClientRect().Top);
}

void OpenGL::GlOrtho(int32_t _Width, int32_t _Height) /*override*/
{
  glOrtho(0, _Width, _Height, 0, -1, 1);
}
