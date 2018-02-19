
#include "stdafx.h"
#include <Covellite\Rocket\Layer.hpp>
#include <alicorn\boost\string-cast.hpp>

using namespace covellite::rocket;

/**
* \brief
*  Функция преобразования пути к файлу в строку libRocket.
* \details
*  - Понадобилась из-за того, что libRocket ожидает для каждой операционной
*  системы путь в определенном формате, а boost::filesystem::path может
*  содержать смешанный набор слешей.
*
* \param [in] _Path
*  Исходный путь.
*
* \return
*  Преобразованный путь.
*/
/*static*/ auto Layer::Convert(const Path_t & _Path) -> String_t
{
  using namespace ::alicorn::extension::std;

  return string_cast<::std::string, Locale::System>(
    string_cast<String>(_Path).Replace(uT("/"), uT("\\"))).c_str();
}
