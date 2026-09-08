
#include "stdafx.h"
#include <iostream>
#include <boost/program_options.hpp>

int main(int _Argc, char * _ppArgv[])
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
    //::std::cout << Description << ::std::endl;
    //return -1;
  }

  if (Options.count("help"))
  {
    ::std::cout << Description << ::std::endl;
    return 0;
  }

  if (Options.count("file"))
  {
    // Process the source file
  }

  if (Options.count("hlsl"))
  {
    try
    {
      //CompileAsHLSL(...);
      return 0;
    }
    catch (const ::std::exception & _Ex)
    {
      ::std::cout << _Ex.what() << ::std::endl;
      return -1;
    }
  }

  if (Options.count("glsl"))
  {
    try
    {
      //CompileAsGLSL(...);
      return 0;
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
