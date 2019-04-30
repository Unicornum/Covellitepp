
#include "stdafx.h"
#include "Landscape.hpp"
#include <random>
#include <boost/any.hpp>
#include <alicorn/std/exception.hpp>
#include <alicorn/std/vector.hpp>
#include <Covellite/Api/Component.inl>
#include "Constants.hpp"
#include "IGameWorld.hpp"

namespace basement
{

namespace model
{

namespace math = ::alicorn::extension::cpp::math;

Landscape::Landscape(const Type::Value _Type, const IGameWorld & _GameWorld) :
  GameObject(_Type),
  m_GameWorld(_GameWorld)
{
  if ((_Type & Type::LandscapeMask) == 0)
  {
    throw STD_EXCEPTION << "Unexpected landscape type: " << _Type;
  }

  const auto AddAloneMesh = 
    [this](const auto & _Data, 
      const size_t _TextureId, 
      const Rect & _TextureCoord = Rect{ })
  {
    const auto TextureRatioXY = GetTexture(_TextureId).GetRatioXY();

    m_Models.push_back(
    {
      { AddMesh<Mesh>(_Data, TextureRatioXY, _TextureCoord), _TextureId },
    });
  };

  if (_Type == Type::Grass)
  {
    const auto TextureId = AddTexture("demo.grass.png");
    const auto TextureId2 = AddTexture("demo.grass2.png");
    const auto TextureId3 = AddTexture("demo.grass3.png");

    //AddAloneMesh("Bush1.obj"), AddTexture("demo.bush1.jpg");
    //AddAloneMesh("Grass1.obj"), TextureId, Rect{ 0.5f, 0.75f,  1.0f, 1.0f };
    //AddAloneMesh("Grass2.obj"), TextureId, Rect{ 0.5f, 0.75f,  1.0f, 1.0f };
    //AddAloneMesh("Grass4.obj"), TextureId, Rect{ 0.5f, 0.75f,  1.0f, 1.0f };
    //AddAloneMesh("Grass5.obj"), TextureId, Rect{ 0.5f, 0.75f,  1.0f, 1.0f };
    //AddAloneMesh("Grass7.obj"), TextureId, Rect{ 0.5f, 0.75f,  1.0f, 1.0f };
    //AddAloneMesh("Grass8.obj"), TextureId, Rect{ 0.5f, 0.75f,  1.0f, 1.0f };

    const auto AddGrassLodMesh = [=](const Rect & _TextureCoord)
    {
      const auto TextureRatioXY = GetTexture(TextureId).GetRatioXY();

      m_Models.push_back(
        {
          { AddMesh<Mesh>(4, TextureRatioXY, _TextureCoord), TextureId },
          { AddMesh<Mesh>(3, TextureRatioXY, _TextureCoord), TextureId2 },
          { AddMesh<Mesh>(2, TextureRatioXY, _TextureCoord), TextureId3 },
        });
    };

    AddGrassLodMesh(Rect{ 0.501f, 0.001f,  0.609f, 0.0366f });
  }
  else if (_Type == Type::Rock)
  {
    const auto TextureId = AddTexture("demo.stone.jpg");

    AddAloneMesh("Boulder1.obj", TextureId, Rect{ 0.5f, 0.25f,  1.0f, 0.0f });
    AddAloneMesh("Boulder2.obj", TextureId, Rect{ 0.5f, 0.25f,  1.0f, 0.0f });
    AddAloneMesh("Boulder3.obj", TextureId, Rect{ 0.5f, 0.25f,  1.0f, 0.0f });
    AddAloneMesh("Stone1.obj", TextureId, Rect{ 0.5f, 0.5f, 1.0f, 0.25f });

    AddAloneMesh("Branch1.obj", TextureId, Rect{ 0.5f, 0.75f, 1.0f, 0.5f });
    AddAloneMesh("Branch2.obj", TextureId, Rect{ 0.5f, 0.75f, 1.0f, 0.5f });
    AddAloneMesh("Branch3.obj", TextureId, Rect{ 0.5f, 0.75f, 1.0f, 0.5f });

    AddAloneMesh("Shrooms.obj", TextureId, Rect{ 0.5f, 1.0f, 1.0f, 0.75f });
  }
  else if (_Type == Type::Bush)
  {
    const auto TextureId = AddTexture("demo.bush.png");

    AddAloneMesh("Bush2.obj", TextureId, Rect{ 0.5f, 0.2f,  1.0f, 0.0f });
    AddAloneMesh("Bush3.obj", TextureId, Rect{ 0.5f, 0.4f,  1.0f, 0.2f });
    AddAloneMesh("Bush4.obj", TextureId, Rect{ 0.5f, 0.6f,  1.0f, 0.4f });
    AddAloneMesh("bush5.obj", TextureId, Rect{ 0.5f, 0.8f,  1.0f, 0.6f });
    AddAloneMesh("Bush6.obj", TextureId, Rect{ 0.5f, 1.0f,  1.0f, 0.8f });
    AddAloneMesh("Bush7.obj", TextureId, Rect{ 0.5f, 1.0f,  1.0f, 0.8f });

    const auto TextureId2 = AddTexture("demo.grass.png");

    AddAloneMesh("Grass3.obj", TextureId2, Rect{ 0.5f, 1.0f,  1.0f, 0.75f });
    AddAloneMesh("Grass6.obj", TextureId2, Rect{ 0.5f, 1.0f,  1.0f, 0.75f });
    AddAloneMesh("Flower1.obj", TextureId2, Rect{ 0.5f, 0.165f,  0.75f, 0.04f });
    AddAloneMesh("Flower1.obj", TextureId2, Rect{ 0.75f, 0.165f,  1.0f, 0.04f });
    AddAloneMesh("Flower3.obj", TextureId2, Rect{ 0.5f, 0.75f,  1.0f, 0.5f });
  }
  else if (_Type == Type::Tree)
  {
    const auto TextureId = AddTexture("demo.tree.png");

    AddAloneMesh("Tree1.obj", TextureId, Rect{ 0.5f, 0.2f,  1.0f, 0.0f });
    AddAloneMesh("Tree2.obj", TextureId, Rect{ 0.5f, 0.2f,  1.0f, 0.0f });
    AddAloneMesh("Tree3.obj", TextureId, Rect{ 0.5f, 0.4f,  1.0f, 0.2f });
    AddAloneMesh("Tree6.obj", TextureId, Rect{ 0.5f, 0.6f,  1.0f, 0.4f });
    AddAloneMesh("Tree7.obj", TextureId, Rect{ 0.5f, 0.8f,  1.0f, 0.6f });
    AddAloneMesh("Tree8.obj", TextureId, Rect{ 0.5f, 1.0f,  1.0f, 0.8f });

    AddAloneMesh("Tree5.obj", AddTexture("demo.tree.jpg"), 
      Rect{ 0.5f, 0.25f,  1.0f, 0.0f });
  }
  else if (_Type == Type::Well)
  {
    AddAloneMesh("Well.obj", AddTexture("demo.well.jpg"),
      Rect{ 0.0f, 1.0f,  1.0f, 0.0f });
  }
  else
  {
    const auto GetTexturePath = [this]()
    {
      static const ::std::map<Type::Value, Path_t> LandscapeTextureFiles =
      {
        { Type::None, "demo.none.jpg" },
        { Type::Sand, "demo.sand.jpg" },
      };

      const auto itPath = LandscapeTextureFiles.find(GetType());
      return (itPath != LandscapeTextureFiles.end()) ? 
        itPath->second : Path_t{ "demo.dummy.png" };
    };

    AddAloneMesh(GetType(), AddTexture(GetTexturePath()));
  }
}

auto Landscape::GetObject(const Any_t & _Value) const /*override*/ -> Objects_t
{
  const auto Random = [](const size_t _From, const size_t _To)
  {
    static ::std::mt19937 Generator{ ::std::random_device{}() };
    return static_cast<size_t>(::std::uniform_int_distribution<>{ 
      static_cast<int>(_From), static_cast<int>(_To) }(Generator));
  };

  using namespace ::alicorn::extension::std;

  if (_Value.empty()) // Формирование единственного общего объекта.
  {
    return
    {
      // Установка шейдеров каждому объекту не сказывается на производительности
      // Release версии, но сильно роняет fps Debug версии программы.
      GetShaderObject()
      // При рендеринге объекта при помощи OpenGL текстура и буферы отключаются
      // при вызове Present.Geometry, поэтому их следует добавлять каждому
      // объекту.
      //+ m_Texture + m_MeshObject
    };
  }

  auto & CellPosition = ::boost::any_cast<const CubeCoords &>(_Value);

  const auto Bright = GetBright(CellPosition);

  const uint32_t MaterialColor = 0xFF000000
    | (Bright << 16)
    | (Bright << 8)
    | (Bright << 0);

  const auto WorldPosition = CellPosition.ToPlane();

  Objects_t Result;

  const auto Rotate = m_IsUsingRotate ?
    Random(0, 5) * math::Constant<float>::Pi / 3.0f : 0.0f;
  const auto Scale = 1.0f + (m_ScaleFactor - 1.0f) * Random(0, 100) / 100.0f;

  for (const auto & Model : m_Models[Random(0, m_Models.size() - 1)])
  {
    static size_t Index = 0;
    Index++;

    // Формирование полного набора компонентов для каждого объекта никак
    // не сказывается на производительности Release версии.
    Result.push_back(
      GetTexture(Model.second).GetObject() +
      Object_t
      {
        Component_t::Make(
          {
            { uT("id"), uT("Demo.Material.Tile.%ID%").Replace(uT("%ID%"), Index) },
            { uT("type"), uT("Material") },
            { uT("ambient"), MaterialColor },
            { uT("diffuse"), MaterialColor },
          }),
      } +
      GetMesh(Model.first).GetObject() +
      Object_t
      {
        Component_t::Make(
          {
            { uT("type"), uT("Data") },
            { uT("kind"), uT("Rotation") },
            { uT("z"), Rotate },
          }),
        Component_t::Make(
          {
            { uT("type"), uT("Data") },
            { uT("kind"), uT("Scale") },
            { uT("z"), Scale },
          }),
        Component_t::Make(
          {
            { uT("type"), uT("Data") },
            { uT("kind"), uT("Position") },
            { uT("x"), WorldPosition.first },
            { uT("y"), WorldPosition.second },
            { uT("z"), m_GameWorld.GetLandscapeHeight(CellPosition) },
          }),
        Component_t::Make(
          {
            { uT("id"), uT("Demo.Present.Tile.%ID%").Replace(uT("%ID%"), Index) },
            { uT("type"), uT("Present") },
            { uT("kind"), uT("Geometry") },
            { uT("static"), uT("true") },
          })
      });
  }

  return Result;
}

uint32_t Landscape::GetBright(const CubeCoords & _CellPosition) const
{
  const auto GetBright = [&](void)
  {
    const auto CurrentCellHeight = 
      m_GameWorld.GetLandscapeHeight(_CellPosition);

    for (int i = 1; i < 2 * Constant::CellRadius; i++)
    {
      const auto LastHeight = 
        m_GameWorld.GetLandscapeHeight(_CellPosition + CubeCoords(0, -(i - 1)));
      const auto Height = 
        m_GameWorld.GetLandscapeHeight(_CellPosition + CubeCoords(0, -i));

      if (LastHeight > Height + 0.1f) // Верхняя точка холма: (i - 1)
      {
        const auto Delta = LastHeight - CurrentCellHeight;

        if (CurrentCellHeight >= Height ||
          0.33f * Delta > static_cast<float>(i - 1)) return 1.0f;

        const auto Bright = 1.0f - 0.75f * Delta;
        if (Bright < 0.75f) return 0.75f;
        if (Bright > 1.0f) return 1.0f;
        return Bright;
      }
    }

    return 1.0f;
  };

  return static_cast<uint32_t>(0xFF * GetBright());
}

} // namespace model

} // namespace basement
