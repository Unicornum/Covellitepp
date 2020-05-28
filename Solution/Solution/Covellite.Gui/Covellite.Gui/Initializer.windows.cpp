
#include "stdafx.h"
#include <Covellite\Gui\Initializer.hpp>

using namespace covellite::gui;

Initializer::Initializer(const Data & _Data) :
  m_System(_Data.m_pStringTranslator),
  m_pRenderInterface(_Data.m_pRenderInterface)
{
  CovelliteGui::Core::SetFileInterface(&m_File);
  CovelliteGui::Core::SetSystemInterface(&m_System);
  CovelliteGui::Core::SetRenderInterface(m_pRenderInterface.get());

  CovelliteGui::Core::Initialise();
  CovelliteGui::Plugin::Initialise();
}

Initializer::~Initializer(void)
{
  CovelliteGui::Core::SetFileInterface(nullptr);
  CovelliteGui::Core::SetSystemInterface(nullptr);
  CovelliteGui::Core::SetRenderInterface(nullptr);

  CovelliteGui::Core::Shutdown();
}
