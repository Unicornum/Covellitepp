
#pragma once
#include <Covellite\Core\Application.hpp>

/**
* \ingroup gCovelliteCoreClasses
* \def APPLICATION_ENTRY_POINT
* \brief
*  Макрос входит в проект \ref CovelliteCorePage \n
*  Макрос реализации функции точки входа для приложения.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
*  2.0.0.0        \n
*  3.0.0.0        \n
* \date
*  27 Декабрь 2016    \n
*  01 Октябрь 2017    \n
*  14 Октябрь 2017    \n
*  17 Октябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016 - 2017
*/

#if BOOST_OS_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define APPLICATION_ENTRY_POINT(MainApplicationClass) \
  int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR _pParams, _In_ int) \
  { \
    covellite::core::Application::Main<MainApplicationClass>(_pParams); \
    return 0; \
  }

#elif BOOST_OS_ANDROID

extern "C" 
{

#ifndef __USING_GTEST
struct ANativeWindow;
struct ANativeActivity;
extern void android_main(struct android_app *);
void ANativeActivity_onCreate(ANativeActivity *, void *, size_t);
#endif

}

#define APPLICATION_ENTRY_POINT(MainApplicationClass) \
  void android_main(struct android_app * _pParams) \
  { \
    covellite::core::Application::Main<MainApplicationClass>(_pParams); \
  } \
  /* Это чтобы компилятор не выбросил функцию ANativeActivity_onCreate() */ \
  void CovelliteFakeFunction(void) { ANativeActivity_onCreate(nullptr, nullptr, 0); }

#else

#define APPLICATION_ENTRY_POINT(MainApplicationClass)

#endif
