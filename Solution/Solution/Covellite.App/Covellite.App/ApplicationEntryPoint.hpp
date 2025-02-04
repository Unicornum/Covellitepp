
#pragma once
#include <Covellite\App\Application.hpp>

/**
* \ingroup CovelliteAppGroup
* \def APPLICATION_ENTRY_POINT
* \brief
*  ������ ������ � ������ \ref CovelliteAppPage \n
*  ������ ���������� ������� ����� ����� ��� ����������.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
*  2.0.0.0        \n
*  3.0.0.0        \n
* \date
*  27 ������� 2016    \n
*  01 ������� 2017    \n
*  14 ������� 2017    \n
*  17 ������� 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2016 - 2017
*/

#ifdef APPLICATION_ENTRY_POINT
#undef APPLICATION_ENTRY_POINT
#endif

#if BOOST_OS_WINDOWS

#include <alicorn/platform/windows.hpp>

#define APPLICATION_ENTRY_POINT(MainApplicationClass) \
  int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR _pParams, _In_ int) \
  { \
    covellite::app::Application::Main<MainApplicationClass>(_pParams); \
    return 0; \
  }

#elif BOOST_PLAT_ANDROID

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
    covellite::app::Application::Main<MainApplicationClass>(_pParams); \
  } \
  /* ��� ����� ���������� �� �������� ������� ANativeActivity_onCreate() */ \
  void CovelliteFakeFunction(void) { ANativeActivity_onCreate(nullptr, nullptr, 0); }

#else

#define APPLICATION_ENTRY_POINT(MainApplicationClass)

#endif
