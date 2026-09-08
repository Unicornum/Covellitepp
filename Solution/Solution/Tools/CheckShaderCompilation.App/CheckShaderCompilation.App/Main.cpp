
#include "stdafx.h"
#include <iostream>
#include <boost/program_options.hpp>
#include <alicorn/std/exception.hpp>

class ShaderFiles
{
  using Path_t = ::boost::filesystem::path;

public:
  void CompileAsHLSL(void) const
  {
    throw ::std::runtime_error("CompileAsHLSL(): not implemented");
  }

  void CompileAsGLSL(void) const
  {
    throw ::std::runtime_error("CompileAsGLSL(): not implemented");
  }

public:
  ShaderFiles(const Path_t & _PathToFile)
  {
    if (!::boost::filesystem::exists(_PathToFile))
    {
      throw EXCEPTION_NO_FILE_LINE(::std::runtime_error) <<
        _PathToFile.string() << "(): error C0000: not exists file";
    }
  }
};

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
      const ShaderFiles Files(Options["file"].as<Path_t>());

      if (Options.count("hlsl"))
      {
        Files.CompileAsHLSL();
        return 0;
      }

      if (Options.count("glsl"))
      {
        Files.CompileAsGLSL();
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
