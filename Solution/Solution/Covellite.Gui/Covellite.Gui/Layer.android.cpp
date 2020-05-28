
#include "stdafx.h"
#include <Covellite\Gui\Layer.hpp>
#include <alicorn\std\string.hpp>
#include <alicorn\std\string\encoding.hpp>
#include <alicorn\boost\string-cast.hpp>

using namespace covellite::gui;

/**
* \brief
*  Функция преобразования пути к файлу в строку библиотеки GUI.
* \details
*  - Понадобилась из-за того, что для каждой операционной системы ожидается
*  путь в определенном формате, а boost::filesystem::path может содержать
*  смешанный набор слешей.
*
* \param [in] _Path
*  Исходный путь.
*
* \return
*  Преобразованный путь.
*/
/*static*/ ::std::string Layer::Convert(const Path_t & _Path)
{
  using namespace ::alicorn::extension::std;

  return string_cast<::std::string, Encoding::UTF8>(
    string_cast<String>(_Path).Replace(uT("\\"), uT("/")));
}
