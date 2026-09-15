
#pragma once
#include <boost/serialization/vector.hpp>
#include <alicorn/std/string.hpp>
#include <alicorn/boost/filesystem.hpp>
#include <Covellite/Api/Component.hpp>

namespace game::repository::initial
{

class Shader_t final
{
  using String_t = ::alicorn::extension::std::String;
  using Entry_t = ::alicorn::extension::std::String;
  using Instance_t = ::alicorn::extension::std::String;
  using Path_t = ::boost::filesystem::path;
  using BinaryData_t = ::alicorn::extension::std::memory::BinaryData_t;
  using LoadFile_t = ::std::function<BinaryData_t(const Path_t &)>;
  using Component_t = ::covellite::api::Component;
  using ComponentPtr_t = ::std::shared_ptr<Component_t>;

public:
  using PathToFile_t = ::std::string;
  using PathToFiles_t = ::std::vector <::std::string>;

public:
  PathToFiles_t PathToFiles;
  Entry_t       Entry;
  Instance_t    Instance;

private:
  friend class ::boost::serialization::access;
  template<class TArchive>
  void serialize(TArchive & _Archive, const unsigned int)
  {
    _Archive & PathToFiles & Entry & Instance;
  }

public:
  uint32_t GetHash(void) const
  {
    // Нужна реализация функции получения уникального хэша объекта на основе
    // содержащихся в нем данных.
    //return ::covellite::api::Component::GetHash(Entry);
    return 0;
  }

  ComponentPtr_t BuildComponent(
    const Path_t & _PathToRoot,
    const LoadFile_t & _LoadFile,
    const String_t & _ShaderId = uT("")) const
  {
    using namespace ::alicorn::extension::std;

    BinaryData_t ShaderData;

    for (const auto & PathToFile : PathToFiles)
    {
      ShaderData += _LoadFile(_PathToRoot / PathToFile);
    }

    const auto pShader = Component_t::Make(
      {
        { uT("id"), uT("Game.Shader.") + _ShaderId },
        { uT("type"), uT("Shader") },
        { uT("entry"), Entry },
        { uT("content"), ShaderData },
      });

    if (Instance != String_t::Empty{})
    {
      (*pShader)[uT("instance")] = Instance;
    }

    return pShader;
  }

  ::std::pair<::std::string, int> GetErrorFileLine(
    const Path_t & _PathToRoot,
    const LoadFile_t & _LoadFile,
    const int _Line) const
  {
    int Line = _Line;
    ::std::string ResultPathToFile;

    const auto GetLineCount = [](const BinaryData_t & _Data)
    {
      int Result = 0;

      for (const auto & Char : _Data)
      {
        if (Char == '\n') Result++;
      }

      return Result;
    };

    for (const auto & PathToFile : PathToFiles)
    {
      const auto LineCount = GetLineCount(_LoadFile(_PathToRoot / PathToFile));

      if (Line <= LineCount)
      {
        ResultPathToFile = PathToFile;
        break;
      }

      Line -= LineCount;
    }

    return { ResultPathToFile, Line };
  }
};

} // namespace game::repository::initial
