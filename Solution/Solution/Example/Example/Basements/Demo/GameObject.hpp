
#pragma once
#include <alicorn/boost/filesystem.forward.hpp>
#include <Covellite/Api/Vertex.hpp>
#include "IGameObject.hpp"


namespace basement
{

namespace model
{

class CubeCoords;
class IGameWorld;

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Общий базовый класс игрового объекта.
*
* \version
*  1.0.0.0        \n
* \date
*  28 Март 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class GameObject :
  public IGameObject
{
public:
  using IGameObjectPtr_t = ::std::shared_ptr<IGameObject>;

protected:
  using Path_t = ::boost::filesystem::path;
  using Vertex_t = ::covellite::api::Vertex::Polyhedron;

public:
  // Интерфейс IGameObject:
  Type::Value GetType(void) const final;

public:
  /**
  * \brief
  *  Функция создания объекта игрового объекта указанного типа.
  */
  static IGameObjectPtr_t Create(const Type::Value, const IGameWorld &);

protected:
  Object_t LoadTexture(const Path_t &) const;
  Object_t GetShaderObject(void) const;
  Object_t GetMeshObject(const ::std::vector<Vertex_t> &,
    const ::std::vector<int> &) const;
  Object_t GetCommonObject(const ::std::vector<Vertex_t> &, 
    const ::std::vector<int> &) const;

private:
  class Image;

private:
  const Type::Value m_Type;
  mutable ::std::shared_ptr<Image> m_pTextureImage;

protected:
  explicit GameObject(const Type::Value);
};

} // namespace model

} // namespace basement
