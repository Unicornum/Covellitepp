
#include "stdafx.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <alicorn/std/exception.hpp>
#include <alicorn/document.hpp>
#include "Initial.hpp"
#include "Serializator.inl"

int main(const int _Argc, const char * const _ppArgv[])
{
  using Path_t = ::boost::filesystem::path;
  using namespace ::boost::program_options;

  options_description Description("Allowed options");

  Description.add_options()
    ("help", "Produce help message.")
    ("file", value<Path_t>(), "Path to shader pack file.")
    ("hlsl", "Check shader as HLSL.")
    ("glsl", "Check shader as GLSL.")
    ;

  variables_map Options;

  try
  {
    store(command_line_parser(_Argc, _ppArgv).options(Description).run(), Options);
    notify(Options);
  }
  catch (const ::std::exception & _Ex)
  {
    ::std::cout << "(): error C0000: " << _Ex.what() << ::std::endl;
  }

  if (Options.count("help"))
  {
    ::std::cout << Description << ::std::endl;
    return 0;
  }

  if (Options.count("file"))
  {
    try
    {
      const auto PathToFile = Options["file"].as<Path_t>();

      if (!::boost::filesystem::exists(PathToFile))
      {
        throw EXCEPTION_NO_FILE_LINE(::std::runtime_error) <<
          PathToFile.string() << "(): error C0000: not exists file";
      }

      using namespace ::game::repository;

      const auto pShader = Serializator<initial::Shader_t>::Read(
        ::boost::filesystem::load_binary_file(PathToFile))
        .BuildComponent(PathToFile.parent_path());

      if (Options.count("hlsl"))
      {
        //DirectX11::Compile(pShader);
        return 0;
      }

      if (Options.count("glsl"))
      {
        //OpenGL::Compile(pShader);
        return 0;
      }
    }
    catch (const ::std::exception & _Ex)
    {
      ::std::cout << _Ex.what() << ::std::endl;
      return -1;
    }
  }

  ::std::cout << Description << ::std::endl;
  return -1;
}
