
#include "stdafx.h"
#include <Covellite\Gui\System.hpp>
#ifndef __USING_GTEST
#include <alicorn\logger.hpp>
#endif

namespace covellite
{

namespace gui
{

System::System(const StringTranslatorPtr_t & _pStringTranslator) :
  m_pStringTranslator(_pStringTranslator)
{
  if (_pStringTranslator == nullptr)
  {
    throw STD_EXCEPTION << "String translator null pointer.";
  }
}

/**
* \brief
*  Функция преобразования строк.
*/
int System::TranslateString(String_t & _Translated,
  const String_t & _Source) /*override*/
{
  _Translated = m_pStringTranslator->Translate(CovelliteGuiStringToUtf8(_Source)).c_str();
  return 0;
}

/**
* \brief
*  Функция записи в лог сообщений/предупреждений/ошибок, произошедших во время 
*  работы используемой библиотки GUI.
*/
bool System::LogMessage(CovelliteGui::Core::Log::Type _Type,
  const String_t & _Message) /*override*/
{
  if (_Type == CovelliteGui::Core::Log::LT_ERROR)
  {
    LOGGER(Error) << CovelliteGuiStringToUtf8(_Message);
  }
  else if (_Type == CovelliteGui::Core::Log::LT_WARNING)
  {
    LOGGER(Warning) << CovelliteGuiStringToUtf8(_Message);
  }
  else if (_Type == CovelliteGui::Core::Log::LT_INFO)
  {
    LOGGER(Info) << CovelliteGuiStringToUtf8(_Message);
  }
  else if (_Type == CovelliteGui::Core::Log::LT_ASSERT ||
    _Type == CovelliteGui::Core::Log::LT_ALWAYS)
  {
    LOGGER(Trace) << CovelliteGuiStringToUtf8(_Message);
  }
  else if (_Type == CovelliteGui::Core::Log::LT_DEBUG)
  {
    LOGGER_DEBUG(Trace) << CovelliteGuiStringToUtf8(_Message);
  }

  return true;
}

} // namespace gui

} // namespace covallite
