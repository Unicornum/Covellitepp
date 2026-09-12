
#pragma once
#include "Serializator.hpp"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/filesystem/fstream.hpp>

namespace game
{

namespace repository
{

/**
* \brief
*  Функция сохранения данных в файл.
* \details
*  - Данные указанной классу структуры будут сохранены в файл в формате,
*  который в дальнейшем позволит прочитать их функцией Read().
*  - Существующий файл будет перезаписан.
*  
* \param [in] _PathToFile
*  Путь к файлу.
* \param [in] _Data
*  Сохраняемые данные.
*  
* \exception std::exception
*  - Действие невозможно (подробнее см. описание исключения).
*/
template<class TData>
/*static*/ void Serializator<TData>::Write(
  const Path_t & _PathToFile, 
  const TData & _Data)
{
  ::boost::filesystem::ofstream Stream{ _PathToFile };
  ::boost::archive::text_oarchive Archive{ Stream };
  Archive & _Data;
}

/**
* \brief
*  Функция чтения данных из блока памяти.
* \details
*  - Функция предназначена для восстановления данных, записанных в файл
*  функцией Write().
*  
* \param [in] _Data
*  Блок памяти, хранящий сериализованные данные.
*  
* \return
*  Объект, содержащий данные, прочитанные из блока памяти.
*  
* \exception std::exception
*  - Действие невозможно (подробнее см. описание исключения).
*/
template<class TData>
/*static*/ TData Serializator<TData>::Read(const Data_t & _Data)
{
  ::std::string sData{ ::std::begin(_Data), ::std::end(_Data) };
  ::std::stringstream Stream{ sData };
  ::boost::archive::text_iarchive Archive{ Stream };

  TData Result;
  Archive & Result;
  return Result;
}

} // namespace repository

} // namespace game
