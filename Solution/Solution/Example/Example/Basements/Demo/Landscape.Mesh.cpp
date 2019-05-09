
#include "stdafx.h"
#include "Landscape.hpp"
#include <random>
#include <alicorn/cpp/math.hpp>
#include <alicorn/std/vector.hpp>
#include <alicorn/std/exception.hpp>
#include <alicorn/boost/filesystem.hpp>
#include <alicorn/logger.hpp>
#include <Covellite/App/Settings.hpp>
#include <Covellite/Gui/Vfs.hpp>

#if BOOST_COMP_MSVC
# pragma warning(push)
# pragma warning(disable: 26498)
# pragma warning(disable: 26495)
# pragma warning(disable: 26451)
#endif

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobj/tiny_obj_loader.h>

#if BOOST_COMP_MSVC
# pragma warning(pop)
# endif

namespace basement
{

namespace model
{

namespace math = ::alicorn::extension::cpp::math;

class Landscape::Mesh::MeshStreamBuffer :
  public ::std::streambuf
{
public:
  pos_type seekoff(::std::streamoff _Offset, ::std::ios_base::seekdir _Dir,
    ::std::ios_base::openmode /*_Which*/ = ::std::ios_base::in) override
  {
    if (_Dir == ::std::ios_base::cur)
    {
      setg(eback(), gptr() + _Offset, egptr());
    }
    else if (_Dir == ::std::ios_base::end)
    {
      setg(eback(), egptr() + _Offset, egptr());
    }
    else if (_Dir == ::std::ios_base::beg)
    {
      setg(eback(), eback() + _Offset, egptr());
    }

    return gptr() - eback();
  }

  // cppcheck-suppress unusedFunction
  pos_type seekpos(::std::streampos _Pos, ::std::ios_base::openmode _Mode) override
  {
    return seekoff(_Pos - pos_type(off_type(0)), ::std::ios_base::beg, _Mode);
  }

private:
  ::boost::filesystem::binary::Data_t m_Data;

public:
  explicit MeshStreamBuffer(const Path_t & _PathToFile) :
    m_Data(::covellite::gui::Vfs_t::GetInstance().GetData(_PathToFile))
  {
    setg(
      reinterpret_cast<char *>(m_Data.data()), 
      reinterpret_cast<char *>(m_Data.data()), 
      reinterpret_cast<char *>(m_Data.data() + m_Data.size()));
  }
};

Landscape::Mesh::Mesh(
  const size_t _Type, 
  const float _TextureRatioXY,
  const Rect & _TextureCoord)
{
  BuildBasementObject(_TextureRatioXY);
  BuildMesh(_Type, 1, _TextureRatioXY, _TextureCoord);
}

Landscape::Mesh::Mesh(
  const int _TriplexCount,
  const float _TextureRatioXY,
  const Rect & _TextureCoord)
{
  BuildBasementObject(_TextureRatioXY);
  BuildMesh(GameObject::Landscape::Grass, _TriplexCount, _TextureRatioXY, _TextureCoord);
}

Landscape::Mesh::Mesh(
  const Path_t & _ObjFileName, 
  const float _TextureRatioXY,
  const Rect & _TextureCoord)
{
  BuildBasementObject(_TextureRatioXY);
  LoadMesh(_ObjFileName, _TextureCoord);
}

void Landscape::Mesh::LoadMesh(
  const Path_t & _MeshFileName,
  const Rect & _TextureCoord)
{
  using ::covellite::app::Settings_t;

  const auto PathToMeshDirectory =
    Settings_t::GetInstance().Get<Path_t>(uT("PathToMeshDirectory"));

  const auto PathToMeshFile = PathToMeshDirectory / "demo" / _MeshFileName;

  class MeshData
  {
  public:
    Mesh & m_Mesh;
    const Rect & TextureCoord;
    size_t PolygonCount;
    ::std::vector<Point> Vertex;
    ::std::vector<Point> Normal;
    ::std::vector<Point> TexCoord;
    float MinTexCoordX, MinTexCoordY, MaxTexCoordX, MaxTexCoordY;
  };

  const auto cbVertex = [](void * _pMeshData, float _X, float _Y, float _Z, float)
  {
    static const auto Scale = 1.0f;

    auto & Data = reinterpret_cast<MeshData *>(_pMeshData)->Vertex;
    Data.push_back(Point{ _X * Scale, _Y * Scale, _Z * Scale });
  };

  const auto cbNormal = [](void * _pMeshData, float _X, float _Y, float _Z)
  {
    auto & Data = reinterpret_cast<MeshData *>(_pMeshData)->Normal;
    Data.push_back(Point{ _X, _Y, _Z });
  };

  const auto cbTexCoord = [](void * _pMeshData, float _X, float _Y, float)
  {
    auto & Data = *reinterpret_cast<MeshData *>(_pMeshData);
    Data.TexCoord.push_back(Point{ _X, _Y, 0.0f });

    Data.MinTexCoordX = math::Min(_X, Data.MinTexCoordX);
    Data.MinTexCoordY = math::Min(_Y, Data.MinTexCoordY);
    Data.MaxTexCoordX = math::Max(_X, Data.MaxTexCoordX);
    Data.MaxTexCoordY = math::Max(_Y, Data.MaxTexCoordY);
  };

  const auto cbIndex = [](void * _pMeshData, 
    ::tinyobj::index_t * _pIndices, int _Count)
  {
    // NOTE: the value of each index is raw value.
    // For example, the application must manually adjust the index with offset
    // (e.g. v_indices.size()) when the value is negative(whic means relative
    // index).
    // Also, the first index starts with 1, not 0.
    // See fixIndex() function in tiny_obj_loader.h for details.
    // Also, 0 is set for the index value which
    // does not exist in .obj

    if (_Count != 3)
    {
      LOGGER(Warning) << "Unexpected index count: " << _Count;
      return;
    }

    auto & Data = *reinterpret_cast<MeshData *>(_pMeshData);

    for (int i = 0; i < _Count; i++)
    {
      if (_pIndices[i].vertex_index == 0)
      {
        LOGGER(Warning) << "Vertex index == 0";
        return;
      }

      if (_pIndices[i].normal_index == 0)
      {
        LOGGER(Warning) << "Normal index == 0";
        return;
      }

      if (_pIndices[i].texcoord_index == 0)
      {
        LOGGER(Warning) << "Texcoord index == 0";
        return;
      }
    }

    Triangle_t Triangle;

    for (int i = 0; i < _Count; i++)
    {
      const size_t iVertex = _pIndices[i].vertex_index - 1;
      Triangle[i].x = Data.Vertex[iVertex].x;
      Triangle[i].y = Data.Vertex[iVertex].y;
      Triangle[i].z = Data.Vertex[iVertex].z;

      const size_t iNormal = _pIndices[i].normal_index - 1;
      Triangle[i].nx = Data.Normal[iNormal].x;
      Triangle[i].ny = Data.Normal[iNormal].y;
      Triangle[i].nz = Data.Normal[iNormal].z;

      const size_t iTexCoords = _pIndices[i].texcoord_index - 1;
      Triangle[i].tu = Data.TexCoord[iTexCoords].x;
      Triangle[i].tv = Data.TexCoord[iTexCoords].y;

      // Предполагается, что модель изначально использует текстурные координаты
      // диапазоне 0...1, а повторяется текстура в диапазонах 1...2, 2...3
      // и т.д. Эти текстурные координаты будут преобразованы в диапазон,
      // указанный в параметрах, т.е. если задать диапазон 0.5...1, то
      // текстурные координаты будут преобразованы:
      // -1...0 -> 0.5...1
      // 0...1 -> 0.5...1
      // 1...2 -> 0.5...1
      // и т.д.

      const auto Convert = 
        [](const float _Source, const float _Min, const float _Max)
      {
        const float Source = 0.999999f * _Source;
        const auto Fractional = Source - math::Floor(Source);
        const auto Result = Fractional * (_Max - _Min) + _Min;
        return Result;
      };

      Triangle[i].tu = Convert(Triangle[i].tu, 
        Data.TextureCoord.left, Data.TextureCoord.right);
      Triangle[i].tv = Convert(Triangle[i].tv, 
        Data.TextureCoord.top, Data.TextureCoord.bottom);
    }

    Data.m_Mesh.Add(Triangle);
    Data.PolygonCount++;
  };

  ::tinyobj::callback_t cbLoader;
  cbLoader.vertex_cb = cbVertex;
  cbLoader.normal_cb = cbNormal;
  cbLoader.texcoord_cb = cbTexCoord;
  cbLoader.index_cb = cbIndex;

  MeshStreamBuffer StreamBuffer(PathToMeshFile);
  ::std::istream StreamData(&StreamBuffer);

  MeshData Data = { *this, _TextureCoord, 0 };

  ::std::string Warnings;
  ::std::string Errors;

  LOGGER(Trace) << "Load mesh file: " << PathToMeshFile;

  const auto IsSuccess = ::tinyobj::LoadObjWithCallback(StreamData, 
    cbLoader, &Data, nullptr, &Warnings, &Errors);

  LOGGER(Trace) << "Polygon count: " << Data.PolygonCount;
  LOGGER(Trace) << "Raw texture coords: " << 
    Data.MinTexCoordX << ", " << Data.MinTexCoordY << ", " << 
    Data.MaxTexCoordX << ", " << Data.MaxTexCoordY << ".";

  if (!Warnings.empty())
  {
    LOGGER(Warning) << Warnings;
  }

  if (!Errors.empty()) 
  {
    LOGGER(Error) << Errors;
  }

  if (!IsSuccess) 
  {
    throw STD_EXCEPTION << "Failed to parse: " << _MeshFileName;
  }
}

void Landscape::Mesh::BuildMesh(
  const size_t _Type,
  const int _TriplexCount,
  const float _TextureRatioXY,
  const Rect & _TextureCoord)
{
  const auto Random = [](const float _From, const float _To)
  {
    static ::std::mt19937 Generator{ ::std::random_device{}() };
    return ::std::uniform_real_distribution<float>{ _From, _To }(Generator);
  };

  if (_Type == GameObject::Landscape::Grass && _TriplexCount > 1)
  {
    const auto Step = -0.001f + 0.7f / (_TriplexCount - 1);

    const auto RatioXY = _TextureRatioXY * 
      (_TextureCoord.right - _TextureCoord.left) /
      (_TextureCoord.bottom - _TextureCoord.top);

    for (float y = -0.35f; y < 0.35f; y += Step)
    {
      for (float x = -0.35f; x < 0.35f; x += Step)
      {
        const auto Height = Random(0.2f, 0.5f);

        // Использование триплексов, у которых плоскости состоят из одного
        // треугольника существенно (в 2.5 раза!) роняет fps (вероятно,
        // производительность обратно пропорциональна количеству пикселей, которые
        // треугольник занимает на экране).
        //BuildTriplex6Object(
        BuildTriplex12Object(
          Point{
            Random(x - 0.375f * Step, x + 0.375f * Step),
            Random(y - 0.375f * Step, y + 0.375f * Step),
            Height
          },
          -0.33f * Height,
          RatioXY * 1.33f * Height / 2.0f,
          Random(0.75f, 1.25f),
          _TextureCoord);
      }
    }
  }
}

void Landscape::Mesh::BuildBasementObject(const float _TextureRatioXY)
{
  const auto kX = 1.0f / (4.0f * _TextureRatioXY);

  ::std::vector<Vertex_t> Points;

  for (float i = 0.0f; i < 6.0f; i += 1.0f)
  {
    const auto HexPoint = GetPoint(i);

    Points.push_back(
      {
        HexPoint.x, HexPoint.y, 0.0f,
        0.0f, 0.0f, 1.0f,
        kX * (0.5f + HexPoint.x), 0.25f * (0.5f + HexPoint.y),
      });
  }

  for (float i = 0.0f; i < 6.0f; i += 1.0f)
  {
    const auto HexPoint = GetPoint(i);
    const auto HexPointNext = GetPoint(i + 1);
    const auto Normal = GetPoint(i, 1.0f);

    Points.push_back(
      {
        HexPoint.x, HexPoint.y, 0.0f,
        Normal.x, Normal.y, 0.0f,
        0.0f, 0.25f,
      });

    Points.push_back(
      {
        HexPoint.x, HexPoint.y, -3.0f,
        Normal.x, Normal.y, 0.0f,
        0.0f, 1.0f,
      });

    Points.push_back(
      {
        HexPointNext.x, HexPointNext.y, 0.0f,
        Normal.x, Normal.y, 0.0f,
        kX, 0.25f,
      });

    Points.push_back(
      {
        HexPointNext.x, HexPointNext.y, -3.0f,
        Normal.x, Normal.y, 0.0f,
        kX, 1.0f,
      });
  }

  const auto AddTriangle =
    [this, &Points](const size_t _i1, const size_t _i2, const size_t _i3)
  {
    Add({ Points[_i1], Points[_i2], Points[_i3] });
  };

  AddTriangle(0, 1, 2);
  AddTriangle(0, 2, 3);
  AddTriangle(0, 3, 4);
  AddTriangle(0, 4, 5);

  for (size_t i = 0; i < 6; i++)
  {
    AddTriangle(6 + 4 * i, 7 + 4 * i, 9 + 4 * i);
    AddTriangle(9 + 4 * i, 8 + 4 * i, 6 + 4 * i);
  }
}

// cppcheck-suppress unusedFunction
void Landscape::Mesh::BuildTriplex6Object(
  const Point & /*_PositionAndHeight*/,
  const float /*_OffsetZ*/,
  const float /*_Radius*/,
  const float /*_AngleCorrector*/,
  const Rect & /*_TextureCoords*/)
{
  //constexpr int PlaneCount = 6;
  //const int StartIndex = static_cast<int>(_MeshData.Vertex.size());

  //const auto Radius = _Radius + 0.25f * _PositionAndHeight.z;
  //const auto Bottom = 4.0f * _Radius;
  //const auto TextureCenter = 0.5f * (_TextureCoords.right + _TextureCoords.left);
  //const auto TextureBottom = _TextureCoords.bottom +
  //  (_TextureCoords.bottom - _TextureCoords.top) * (Bottom / _PositionAndHeight.z);

  //const auto CalculateVertexData = [&](void)
  //{
  //  ::std::vector<Vertex_t> Result;

  //  for (int i = 0; i < PlaneCount; i++)
  //  {
  //    const auto Index = i * _AngleCorrector;

  //    const auto HexPoint1 = _PositionAndHeight + GetPoint(Index, Radius);
  //    const auto HexPoint2 = _PositionAndHeight + GetPoint(Index + 3, Radius);

  //    const auto Normal1 = GetPoint(Index - 0.5f, 1.0f);
  //    const auto Normal2 = GetPoint(Index - 1.5f, 1.0f);
  //    const auto Normal3 = GetPoint(Index - 2.5f, 1.0f);

  //    Result.push_back(
  //      {
  //        HexPoint1.x, HexPoint1.y, HexPoint1.z + _OffsetZ,
  //        Normal1.x, Normal1.y, 0.0f,
  //        _TextureCoords.right, _TextureCoords.top,
  //      });

  //    Result.push_back(
  //      {
  //        HexPoint2.x, HexPoint2.y, HexPoint2.z + _OffsetZ,
  //        Normal2.x, Normal2.y, 0.0f,
  //        _TextureCoords.left, _TextureCoords.top,
  //      });

  //    Result.push_back(
  //      {
  //        _PositionAndHeight.x, _PositionAndHeight.y, -Bottom + _OffsetZ,
  //        Normal3.x, Normal3.y, 0.0f,
  //        TextureCenter, TextureBottom
  //      });
  //  }

  //  return Result;
  //};

  //const auto CalculateIndexData = [&](void)
  //{
  //  ::std::vector<int> Result;

  //  for (int i = 0; i < PlaneCount; i++)
  //  {
  //    const auto BuildIndex = [=](const int _Io)
  //    {
  //      return StartIndex + 3 * i + _Io;
  //    };

  //    using namespace ::alicorn::extension::std;

  //    Result += ::std::vector<int>{ BuildIndex(0), BuildIndex(1), BuildIndex(2) };
  //    //Result += ::std::vector<int>{ BuildIndex(1), BuildIndex(0), BuildIndex(2) };
  //  }

  //  return Result;
  //};

  //using namespace ::alicorn::extension::std;

  //_MeshData.Vertex += CalculateVertexData();
  //_MeshData.Index += CalculateIndexData();
}

void Landscape::Mesh::BuildTriplex12Object(
  const Point & _PositionAndHeight,
  const float _OffsetZ,
  const float _Radius,
  const float _AngleCorrector,
  const Rect & _TextureCoords)
{
  constexpr int PlaneCount = 6;

  for (int i = 0; i < PlaneCount; i++)
  {
    const auto Index = i * _AngleCorrector;

    const auto HexPoint1 = _PositionAndHeight + GetPoint(Index, _Radius);
    const auto HexPoint2 = _PositionAndHeight + GetPoint(Index + 3, _Radius);

    const auto Normal1 = GetPoint(Index - 0.5f, 1.0f);
    const auto Normal2 = GetPoint(Index - 2.5f, 1.0f);

    Add(
    {
      {
        HexPoint1.x, HexPoint1.y, HexPoint1.z + _OffsetZ,
        Normal1.x, Normal1.y, 0.75f,
        _TextureCoords.right, _TextureCoords.top,
      },
      {
        HexPoint2.x, HexPoint2.y, HexPoint2.z + _OffsetZ,
        Normal2.x, Normal2.y, 0.75f,
        _TextureCoords.left, _TextureCoords.top,
      },
      {
        HexPoint1.x, HexPoint1.y, 0.0f + _OffsetZ,
        Normal1.x, Normal1.y, 0.0f,
        _TextureCoords.right, _TextureCoords.bottom,
      },
    });

    Add(
    {
      {
        HexPoint1.x, HexPoint1.y, 0.0f + _OffsetZ,
        Normal1.x, Normal1.y, 0.0f,
        _TextureCoords.right, _TextureCoords.bottom,
      },
      {
        HexPoint2.x, HexPoint2.y, HexPoint2.z + _OffsetZ,
        Normal2.x, Normal2.y, 0.75f,
        _TextureCoords.left, _TextureCoords.top,
      },
      {
        HexPoint2.x, HexPoint2.y, 0.0f + _OffsetZ,
        Normal2.x, Normal2.y, 0.0f,
        _TextureCoords.left, _TextureCoords.bottom,
      }
    });
  }
}

/*static*/ auto Landscape::Mesh::GetPoint(const float _Index, const float _Radius) -> Point
{
  namespace math = ::alicorn::extension::cpp::math;

  return Point
  {
    _Radius * math::degree::Cos(60.0f * _Index),
    _Radius * math::degree::Sin(60.0f * _Index)
  };
}

} // namespace model

} // namespace basement
