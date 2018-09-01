
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
  _Translated = m_pStringTranslator->Translate(_Source.CString()).c_str();
  return 0;
}

/**
* \brief
*  Функция записи в лог сообщений/предупреждений/ошибок, произошедших во время 
*  работы libRocket.
*/
bool System::LogMessage(Rocket::Core::Log::Type _Type,
  const String_t & _Message) /*override*/
{
  if (_Type == Rocket::Core::Log::LT_ERROR)
  {
    LOGGER(Error) << _Message.CString();
  }
  else if (_Type == Rocket::Core::Log::LT_WARNING)
  {
    LOGGER(Warning) << _Message.CString();
  }
  else if (_Type == Rocket::Core::Log::LT_INFO)
  {
    LOGGER(Info) << _Message.CString();
  }
  else if (_Type == Rocket::Core::Log::LT_ASSERT ||
    _Type == Rocket::Core::Log::LT_ALWAYS)
  {
    LOGGER(Trace) << _Message.CString();
  }
  else if (_Type == Rocket::Core::Log::LT_DEBUG)
  {
    LOGGER_DEBUG(Trace) << _Message.CString();
  }

  return true;
}

} // namespace gui

} // namespace covallite
