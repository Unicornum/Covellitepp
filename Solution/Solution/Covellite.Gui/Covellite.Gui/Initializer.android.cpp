
#include "stdafx.h"
#include <Covellite\Gui\Initializer.hpp>

using namespace covellite::gui;

class Initializer::Helper
{
public:
  static Helper & GetInstance(void)
  {
    static Helper Instance;
    return Instance;
  }

private:
  Helper(void)
  {
    CovelliteGui::Initialise();
  }
  ~Helper(void)
  {
    CovelliteGui::Shutdown();
  }
};

Initializer::Initializer(const Data & _Data) :
  m_System(_Data.m_pStringTranslator),
  m_pRenderInterface(_Data.m_pRenderInterface)
{
  CovelliteGui::SetFileInterface(&m_File);
  CovelliteGui::SetSystemInterface(&m_System);
  CovelliteGui::SetRenderInterface(m_pRenderInterface.get());

  // Инициализация сделана через статический объект из-за того, что
  // CovelliteGui::Core::Initialise() в рамках одного модуля можно запускать
  // только один раз (CovelliteGui::Core::Shutdown() не поможет, повторная
  // инициализация даже в этом случае приводит к падению программы),
  // а android-приложение загружает главный .so модуль только один раз при
  // первом старте программы, а затем при каждой активации программы просто
  // вызывает функцию этого модуля.
  Helper::GetInstance();
}

Initializer::~Initializer(void)
{
  CovelliteGui::SetFileInterface(nullptr);
  CovelliteGui::SetSystemInterface(nullptr);
  CovelliteGui::SetRenderInterface(nullptr);
}
