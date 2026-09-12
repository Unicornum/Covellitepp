
#pragma once
#include <vector>
#include <alicorn/boost/filesystem.forward.hpp>

namespace game
{

namespace repository
{

/**
* \ingroup GameRepositoryGroup
* \brief
*  Класс входит в проект \ref GameRepositoryPage \n
*  Класс сериализации данных указанного класса.
* \details
*  - Вспомогательный класс для сохранения и чтения данных, используемых игрой.
*  - Предполагается, что данные будут записываться специальным проектом во время
*  сборки проектов решения игры в файлы, содержимое которых в дальнейшем будет
*  читаться посредством виртуальной файловой системы.
*  
* \tparam TData
*  Класс, данные которого будут записаны или прочитаны; должен включать
*  статическую функцию сериализации полей класса в соотвествии с требованиями
*  библиотеки Boost.Serialization.
*  
* \version
*  1.0.0.0        \n
* \date
*  22 Август 2020    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2020
*/
template<class TData>
class Serializator final
{
  using Path_t = ::boost::filesystem::path;
  using Data_t = ::std::vector<uint8_t>;

public:
  static void Write(const Path_t &, const TData &);
  static TData Read(const Data_t &);
};

} // namespace repository

} // namespace game

#include "Serializator.inl"
