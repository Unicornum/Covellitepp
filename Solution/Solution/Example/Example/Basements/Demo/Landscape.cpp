
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
  m_GameWorld(_GameWorld),
  m_Texture(LoadTexture(GetTexturePath()))
{
  if (_Type < Type::None || 
    _Type > Type::Sand)
  {
    throw STD_EXCEPTION << "Unexpected landscape type: " << _Type;
  }
}

Object_t Landscape::GetObject(const Any_t & _Value) const /*override*/
{
  if (_Value.empty())
  {
    using namespace ::alicorn::extension::std;

    auto Result = GetShaderObject();

    if (GetType() != Type::Bush && 
      GetType() != Type::Tree &&
      GetType() != Type::Grass)
    {
      BuildLowerObject();
      BuildUpperObject();

      Result += GetMeshObject(m_VertexData, m_IndexData);
    }

    // При рендеринге объекта при помощи OpenGL текстура отключается, поэтому
    // ее следует добавлять каждому объекту.
    //Result += m_Texture;

    return Result;
  }

  auto & CellPosition = ::boost::any_cast<const CubeCoords &>(_Value);

  const auto Bright = GetBright(CellPosition);

  const uint32_t MaterialColor = 0xFF000000
    | (Bright << 16)
    | (Bright << 8)
    | (Bright << 0);

  const auto WorldPosition = CellPosition.ToPlane();

  using namespace ::alicorn::extension::std;

  Object_t Result;

  if (GetType() == Type::Bush || 
    GetType() == Type::Tree ||
    GetType() == Type::Grass)
  {
    BuildLowerObject();
    BuildUpperObject();

    Result += GetMeshObject(m_VertexData, m_IndexData);
  }

  static size_t Index = 0;
  Index++;

  return Result + m_Texture + Object_t
  {
    Component_t::Make(
      {
        { uT("id"), uT("Demo.Material.Hexagon.%ID%").Replace(uT("%ID%"), Index) },
        { uT("type"), uT("Material") },
        { uT("ambient"), MaterialColor },
        { uT("diffuse"), MaterialColor },
      }),
    Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Position") },
        { uT("x"), 0.5f * WorldPosition.first },
        { uT("y"), 0.5f * WorldPosition.second },
        { uT("z"), m_GameWorld.GetLandscapeHeight(CellPosition) },
      }),
    Component_t::Make(
      {
        { uT("id"), uT("Demo.Present.Hexagon.%ID%").Replace(uT("%ID%"), Index) },
        { uT("type"), uT("Present") },
        { uT("kind"), uT("Geometry") },
      })
  };
}

void Landscape::BuildLowerObject(void) const
{
  const auto CalculateVertexData = [&](void)
  {
    ::std::vector<Vertex_t> Result;

    for (int i = 0; i < 6; i++)
    {
      const auto HexPoint = GetPoint(i);

      Result.push_back(
        {
          HexPoint.x, HexPoint.y, 0.0f,
          0.0f, 0.0f, 1.0f,
          0.5f * (0.5f + HexPoint.x), 0.5f + 0.5f * (0.5f + HexPoint.y),
        });
    }

    for (int i = 0; i < 6; i++)
    {
      const auto HexPoint = GetPoint(i);
      const auto HexPointNext = GetPoint(i + 1);
      const auto Normal = GetPoint(i, 1.0f);

      Result.push_back(
        {
          HexPoint.x, HexPoint.y, 0.0f,
          Normal.x, Normal.y, 0.0f,
          0.5f + 0.25f * 0.5f, 0.0f,
        });

      Result.push_back(
        {
          HexPoint.x, HexPoint.y, -1.0f,
          Normal.x, Normal.y, 0.0f,
          0.5f + 0.25f * 0.5f, 0.5f,
        });

      Result.push_back(
        {
          HexPointNext.x, HexPointNext.y, 0.0f,
          Normal.x, Normal.y, 0.0f,
          0.5f + 0.75f * 0.5f, 0.0f,
        });

      Result.push_back(
        {
          HexPointNext.x, HexPointNext.y, -1.0f,
          Normal.x, Normal.y, 0.0f,
          0.5f + 0.75f * 0.5f, 0.5f,
        });
    }

    return Result;
  };

  const auto CalculateIndexData = [&](void)
  {
    ::std::vector<int> Result =
    {
       0, 1, 2,
       0, 2, 3,
       0, 3, 4,
       0, 4, 5,
    };

    for (int i = 0; i < 6; i++)
    {
      using namespace ::alicorn::extension::std;

      Result += ::std::vector<int> 
      { 
        6 + 4 * i, 7 + 4 * i, 9 + 4 * i,
        9 + 4 * i, 8 + 4 * i, 6 + 4 * i
      };
    }

    return Result;
  };

  static const auto VertexData = CalculateVertexData();
  static const auto IndexData = CalculateIndexData();

  using namespace ::alicorn::extension::std;

  m_VertexData = VertexData;
  m_IndexData = IndexData;
}

void Landscape::BuildUpperObject(void) const
{
  const auto Random = [](const float _From, const float _To)
  {
    static ::std::mt19937 Generator{ ::std::random_device{}() };
    return ::std::uniform_real_distribution<float>{ _From, _To }(Generator);
  };

  if (GetType() == Type::None)
  {
    return;
  }
  else if (GetType() == Type::Sand)
  {
    return;
  }
  else if (GetType() == Type::Grass)
  {
    for (float y = -0.5f; y < 0.5f; y += 0.2f)
    {
      for (float x = -0.5f; x < 0.5f; x += 0.2f)
      {
        BuildTriplexObject(Point
          {
            Random(x - 0.05f, x + 0.05f),
            Random(y - 0.05f, y + 0.05f),
            Random(0.2f, 0.3f)
          },
          Random(0.4f, 0.8f),
          Random(0.5f, 1.5f),
          Rect{ 0.01f, 0.01f,  0.49f, 0.11f });
      }
    }
  }
  else if (GetType() == Type::Bush)
  {
    for (float y = -0.3f; y < 0.3f; y += 0.2f)
    {
      for (float x = -0.3f; x < 0.3f; x += 0.2f)
      {
        BuildTriplexObject(Point
          {
            Random(x - 0.05f, x + 0.05f),
            Random(y - 0.05f, y + 0.05f),
            Random(0.8f, 1.5f)
          },
          Random(0.75f, 1.5f),
          Random(0.5f, 1.5f),
          Rect{ 0.01f, 0.01f,  0.49f, 0.3f });
      }
    }
  }
  else if (GetType() == Type::Rock)
  {
    const auto Height = Random(0.5f, 0.8f);

    BuildTriplexObject(Point
      {
        0.0f,
        0.0f,
        Height
      },
      2.56f * Height / 2.0f,
      Random(0.9f, 1.1f),
      Rect{ 0.01f, 0.01f,  0.49f, 0.17f });
  }
  else if (GetType() == Type::Tree)
  {
    const auto TreeCount = Random(1.0f, 4.0f);

    for (auto i = 0.0f; i < TreeCount; i += 1.0f)
    {
      const auto Height = Random(0.5f + i, 2.0f + i);

      BuildTriplexObject(Point
        {
          Random(-0.3f, 0.3f),
          Random(-0.3f, 0.3f),
          Height
        },
        0.71f * Height / 2.0f,
        1.0f,
        Rect{ 0.06f, 0.01f,  0.44f, 0.49f });
    }
  }
}

void Landscape::BuildTriplexObject(
  const Point & _PositionAndHeight,
  const float _Radius,
  const float _AngleCorrector,
  const Rect & _TextureCoords) const
{
  const int StartIndex = static_cast<int>(m_VertexData.size());

  const auto CalculateVertexData = [&](void)
  {
    ::std::vector<Vertex_t> Result;

    for (int i = 0; i < 6; i++)
    {
      const auto Index = i * _AngleCorrector;

      const auto HexPoint = _PositionAndHeight + GetPoint(Index, _Radius);
      const auto Normal = 
        //Point{ 0.0f, -1.0f };
        //GetPoint(1.5f + i, 1.0f);
        GetPoint(Index + 0.5f, 1.0f);

      const auto HexPoint2 = _PositionAndHeight + GetPoint(Index + 3, _Radius);
      const auto Normal2 =
        //Point{ 0.0f, -1.0f };
        //GetPoint(1.5f + i, 1.0f);
        GetPoint(Index + 2.5f, 1.0f);

      Result.push_back(
        {
          HexPoint.x, HexPoint.y, HexPoint.z,
          Normal.x, Normal.y, 0.0f,
          _TextureCoords.left, _TextureCoords.top,
        });

      Result.push_back(
        {
          HexPoint.x, HexPoint.y, -0.2f,
          Normal.x, Normal.y, 0.0f,
          _TextureCoords.left, _TextureCoords.bottom,
        });

      Result.push_back(
        {
          HexPoint2.x, HexPoint2.y, HexPoint2.z,
          Normal2.x, Normal2.y, 0.0f,
          _TextureCoords.right, _TextureCoords.top,
        });

      Result.push_back(
        {
          HexPoint2.x, HexPoint2.y, -0.2f,
          Normal2.x, Normal2.y, 0.0f,
          _TextureCoords.right, _TextureCoords.bottom,
        });
    }

    return Result;
  };

  const auto CalculateIndexData = [&](void)
  {
    ::std::vector<int> Result;

    for (int i = 0; i < 6; i++)
    {
      const auto BuildIndex = [=](const int _Io)
      {
        return _Io + 4 * i + StartIndex;
      };

      using namespace ::alicorn::extension::std;

      Result += ::std::vector<int>
      {
        BuildIndex(0), BuildIndex(1), BuildIndex(2),
        BuildIndex(1), BuildIndex(3), BuildIndex(2),
      };
    }

    return Result;
  };

  using namespace ::alicorn::extension::std;

  m_VertexData += CalculateVertexData();
  m_IndexData += CalculateIndexData();
}

auto Landscape::GetTexturePath(void) const -> Path_t
{
  static const ::std::map<Type::Value, Path_t> LandscapeTextureFiles =
  {
    { Type::None, "demo.none.png" },
    { Type::Grass, "demo.grass.png" },
    { Type::Sand, "demo.sand.png" },
    { Type::Bush, "demo.bush.png" },
    { Type::Rock, "demo.rock.png" },
    { Type::Tree, "demo.tree.png" },
  };

  const auto itPath = LandscapeTextureFiles.find(GetType());
  if (itPath != LandscapeTextureFiles.end()) return itPath->second;

  return "demo.dummy.png";
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

/*static*/ auto Landscape::GetPoint(const float _Index, const float _Radius) -> Point
{
  return Point
  {
    _Radius * math::degree::Cos(60.0f * _Index),
    _Radius * math::degree::Sin(60.0f * _Index)
  };

}

} // namespace model

} // namespace basement
