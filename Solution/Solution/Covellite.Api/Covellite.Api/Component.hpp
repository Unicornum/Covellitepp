
#pragma once
#include <map>
#include <alicorn/std/string.hpp>
#include <Covellite/Predefined.hpp>

namespace covellite
{

namespace api
{

/**
* \ingroup CovelliteApiGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Класс компонента.
* \details
*  - Содержит и предоставляет доступ к исходной информации для создания 
*  компонентов рендеров, из которых формируются объекты 3D сцены.
*  - Предполагается, что исходная информация о компоненте будет хранится
*  в узлах xml файла, из которых параметры будут загружаться как атрибуты, 
*  у которых имя атрибута будет названием параметра, а значение атрибута - 
*  значением параметра.
*  - Первоначальная информация может загружаться в виде строк, в дальнейшем
*  значения параметров могут устанавливаться любого типа (без промежуточного
*  преобразования в строки и обратно).
*  - Параметры (кроме обязательных) могут отсутствовать, для таких параметров 
*  предусмотрено задание значений по умолчанию.
*  - Обязательный параметр \b id - идентификатор компонента. Для компонентов
*  с одним и тем же идентификатором будет использоваться один и тот же рендер,
*  поэтому объекты, содержащие один и тот же меш, могут содержать компоненты,
*  у которых задан только (один и тот же) идентификатор, а развернутое
*  описание этого компонента можно сделать в другом месте и создать его рендер
*  заранее.
*  - Обязательный параметр \b type - тип компонента, который определяет способ
*  отрисовки этого компонента. Допустимые значения:
*   + Camera.
*   + State (\b kind - Blend/Sampler/Scissor; для Scissor - \b is_enabled, 
*  \b left, \b right, \b top, \b bottom).
*   + Position (\b x, \b y, \b z).
*   + Buffer (\b kind - Vertex/Index, \b data, \b count).
*   + DrawCall.
*   + Texture (\b data, \b count, \b width, \b height).
*   + Shader (\b kind - Vertex/Pixel, \b data, \b count, \b version, \b entry).
*  
* \version
*  1.0.0.0        \n
* \date
*  24 Октябрь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class Component final
{
  template<bool>
  class Convertor;
  class ConstructorTag {};

public:
  using String_t = ::alicorn::extension::std::String;
  using Name_t = String_t;
  using Params_t = ::std::map<Name_t, ::covellite::Any_t>;
  using Type_t = String_t;
  using Id_t = String_t;
  using ComponentPtr_t = ::std::shared_ptr<Component>;

public:
  class Renders;

public:
  static ComponentPtr_t Make(const Params_t &);

public:
  template<class T>
  T GetValue(const Name_t &, const T &) const;
  template<class T>
  void SetValue(const Name_t &, const T &);

private:
  Params_t m_Params;

public:
  const Type_t Type;
  const Id_t Id;

public:
  Component(const Params_t &, ConstructorTag);
};

} // namespace api

} // namespace covellite
