
#include "stdafx.h"
#include <Covellite\Gui\Initializer.hpp>
#include <RmlUi/Core/RenderInterfaceCompatibility.h>

using namespace covellite::gui;

Initializer::Initializer(const Data & _Data) :
  m_System(_Data.m_pStringTranslator),
  m_pRenderInterface(_Data.m_pRenderInterface)
{
  CovelliteGui::SetFileInterface(&m_File);
  CovelliteGui::SetSystemInterface(&m_System);
  CovelliteGui::SetRenderInterface(m_pRenderInterface->GetAdaptedInterface());

  CovelliteGui::Initialise();
}

Initializer::~Initializer(void)
{
  CovelliteGui::SetFileInterface(nullptr);
  CovelliteGui::SetSystemInterface(nullptr);
  CovelliteGui::SetRenderInterface(nullptr);

  CovelliteGui::Shutdown();
}
