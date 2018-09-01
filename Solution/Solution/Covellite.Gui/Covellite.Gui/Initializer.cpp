
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
    Rocket::Core::Initialise();
  }
  ~Helper(void) noexcept
  {
    Rocket::Core::Shutdown();
  }
};

Initializer::Initializer(const Data & _Data) :
  m_System(_Data.m_pStringTranslator),
  m_pRenderInterface(_Data.m_pRenderInterface)
{
  Rocket::Core::SetFileInterface(&m_File);
  Rocket::Core::SetSystemInterface(&m_System);
  Rocket::Core::SetRenderInterface(m_pRenderInterface.get());

  // Инициализация libRocket сделана через статический объект из-за того, что
  // Rocket::Core::Initialise() в рамках одного модуля можно запускать только
  // один раз (Rocket::Core::Shutdown() не поможет, повторная инициализация 
  // даже в этом случае приводит к падению программы), а android-приложение
  // загружает главный .so модуль только один раз при первом старте программы,
  // а затем при каждой активации программы просто вызывает функцию этого
  // модуля.
  Helper::GetInstance();
}

Initializer::~Initializer(void) noexcept
{
  Rocket::Core::SetFileInterface(nullptr);
  Rocket::Core::SetSystemInterface(nullptr);
  Rocket::Core::SetRenderInterface(nullptr);
}
