
#include "stdafx.h"
#include "Landscape.hpp"
#include <random>
#include <alicorn/std/exception.hpp>
#include <alicorn/std/vector.hpp>
#include <Covellite/Api/Component.inl>
#include <Covellite/Api/Constant.hpp>
#include "Constants.hpp"
#include "IGameWorld.hpp"
#include "Lights.hpp"
#include "PointLights.hpp"

namespace basement
{

namespace model
{

namespace math = ::alicorn::extension::cpp::math;

Landscape::Landscape(const GameObject::Landscape::Value _Type, 
  const IGameWorld & _GameWorld) :
  GameObject(_Type),
  m_GameWorld(_GameWorld)
{
  if (_Type < GameObject::Landscape::None ||
    _Type >= GameObject::Landscape::Invalid)
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

  if (_Type == GameObject::Landscape::Grass)
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
  else if (_Type == GameObject::Landscape::Rock)
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
  else if (_Type == GameObject::Landscape::Bush)
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
  else if (_Type == GameObject::Landscape::Tree)
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
  else if (_Type == GameObject::Landscape::Well)
  {
    AddAloneMesh("Well.obj", AddTexture("demo.well.jpg"),
      Rect{ 0.0f, 1.0f,  1.0f, 0.0f });
  }
  else
  {
    const auto GetTexturePath = [this]()
    {
      static const ::std::map<size_t, Path_t> LandscapeTextureFiles =
      {
        { GameObject::Landscape::None, "demo.none.jpg" },
        { GameObject::Landscape::Sand, "demo.sand.jpg" },
      };

      const auto itPath = LandscapeTextureFiles.find(GetType());
      return (itPath != LandscapeTextureFiles.end()) ? 
        itPath->second : Path_t{ "demo.dummy.png" };
    };

    AddAloneMesh(GetType(), AddTexture(GetTexturePath()));
  }
}

static const auto RandomLandscape = [](const size_t _From, const size_t _To)
{
  static ::std::mt19937 Generator{ ::std::random_device{}() };
  return static_cast<size_t>(::std::uniform_int_distribution<>{
    static_cast<int>(_From), static_cast<int>(_To) }(Generator));
};

auto Landscape::GetObject(const Any_t & _Value) const /*override*/ -> Objects_t
{
  using namespace ::alicorn::extension::std;

  if (_Value.empty()) // ������������ ������������� ������ �������.
  {
    return
    {
      // ��������� �������� ������� ������� �� ����������� �� ������������������
      // Release ������, �� ������ ������ fps Debug ������ ���������.
      GetShaderObject()
      // ��� ���������� ������� ��� ������ OpenGL �������� � ������ �����������
      // ��� ������ Present, ������� �� ������� ��������� �������
      // �������.
    };
  }

  const auto & CellPosition = ::boost::any_cast<const CubeCoords &>(_Value);
  //const auto Material = GetMaterial(CellPosition);
  const auto PointLights = 
    //GetPointLight(CellPosition);
    GetUserConstantBuffer(CellPosition);
  String_t TransformId;
  const auto & LodModels = m_Models[RandomLandscape(0, m_Models.size() - 1)];

  Objects_t Result =
  {
    // ������������ ������� ������ ����������� ��� ������� ������� �����
    // �� ����������� �� ������������������ Release ������.
    GetTexture(LodModels[0].second).GetObject() +
    //Material +
    PointLights +
    GetTransform(CellPosition, TransformId) +
    GetMesh(LodModels[0].first).GetObject()
  };

  for (::std::size_t i = 1; i < LodModels.size(); i++)
  {
    // ������������ ������� ������ ����������� ��� ������� ������� �����
    // �� ����������� �� ������������������ Release ������.
    Result.push_back(
      GetTexture(LodModels[i].second).GetObject() +
      //Material +
      PointLights +
      Object_t { Component_t::Make({ { uT("id"), TransformId } }) } +
      GetMesh(LodModels[i].first).GetObject());
  }

  return Result;
}

Object_t Landscape::GetTransform(
  const CubeCoords & _CellPosition,
  String_t & _TransformComponentId) const
{
  static size_t Index = 0;

  _TransformComponentId = String_t{ uT("Demo.Transform.Tile.%ID%") }
    .Replace(uT("%ID%"), Index++);

  const auto WorldPosition = _CellPosition.ToPlane();

  const auto Rotate = m_IsUsingRotate ?
    RandomLandscape(0, 5) * math::Constant<float>::Pi / 3.0f : 0.0f;
  const auto Scale = 1.0f + (m_ScaleFactor - 1.0f) * RandomLandscape(0, 100) / 100.0f;

  const Object_t Transform =
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
        { uT("z"), m_GameWorld.GetLandscapeHeight(_CellPosition) },
      }),
  };

  return 
  {
    Component_t::Make(
      {
        { uT("id"), _TransformComponentId },
        { uT("type"), uT("Transform") },
        { uT("kind"), uT("Static") },
        { uT("service"), Transform },
      })
  };
}

Object_t Landscape::GetUserConstantBuffer(const CubeCoords & _CellPosition) const
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<void>;

  struct Fog
  {
    float Color[4];
    float Near;
    float Far;
    float Density;
    // cppcheck-suppress unusedStructMember
    float Dummy;
  };

  struct UserConstantBuffer
  {
    Fog Fog;
    Lights_t Lights;
  };

  const auto Far = 0.5f * math::Root<2>(Constant::Camera::FarClippingPlane);

  static size_t Index = 0;

  const auto Id = String_t{ uT("Demo.Light.Point.Tile.User.%ID%") }
    .Replace(uT("%ID%"), Index++);

  const auto WorldHeight = m_GameWorld.GetLandscapeHeight(_CellPosition);
  const auto IsNightMode = Constant::GetSettings<bool>(uT("IsNightMode"));

  const auto ARGBtoFloat4 = [](uint32_t _HexColor)
  {
    return ::glm::vec4
    {
      ((_HexColor & 0x00FF0000) >> 16) / 255.0f,
      ((_HexColor & 0x0000FF00) >> 8) / 255.0f,
      ((_HexColor & 0x000000FF) >> 0) / 255.0f,
      ((_HexColor & 0xFF000000) >> 24) / 255.0f
    };
  };

  const BufferMapper_t Mapper = [=](void * _pUserConstBuffer)
  {
    // ����������� ����� ::glm::vec4 ������, ��������� ������ � Release ������

    auto * pUserConstBuffer = 
      reinterpret_cast<UserConstantBuffer *>(_pUserConstBuffer);

    pUserConstBuffer->Fog.Color[0] = 135.0f / 255.0f;
    pUserConstBuffer->Fog.Color[1] = 206.0f / 255.0f;
    pUserConstBuffer->Fog.Color[2] = 250.0f / 255.0f;
    pUserConstBuffer->Fog.Color[3] = 1.0f;
    pUserConstBuffer->Fog.Near = 0.75f * Far;
    pUserConstBuffer->Fog.Far = Far;
    pUserConstBuffer->Fog.Density = 0.1f;

    auto & Ambient = pUserConstBuffer->Lights.Ambient;
    auto & Direction = pUserConstBuffer->Lights.Direction;
    auto & Points = pUserConstBuffer->Lights.Points;

    if (IsNightMode)
    {
      Ambient.IsValid = 1;
      Ambient.Color = ARGBtoFloat4(0xFF202050);

      Direction.IsValid = 0;
      Points = m_GameWorld.GetPointLights()
        .GetUserConstantBuffer(_CellPosition, WorldHeight);
    }
    else
    {
      Ambient.IsValid = 1;
      Ambient.Color = ARGBtoFloat4(0xFF8080A0);

      Direction.IsValid = 1;
      Direction.Color = ARGBtoFloat4(0xFFF0F0B0);
      Direction.Direction = { 0.0f, -1.0f, 0.33f, 1.0f };
      Points.UsedSlotCount = 0;
    }

    return false;
  };

  return
  {
    Component_t::Make(
    {
      { uT("id"), Id },
      { uT("type"), uT("Buffer") },
      { uT("mapper"), Mapper },
      { uT("size"), sizeof(UserConstantBuffer) },
    }),
  };
}

} // namespace model

} // namespace basement
