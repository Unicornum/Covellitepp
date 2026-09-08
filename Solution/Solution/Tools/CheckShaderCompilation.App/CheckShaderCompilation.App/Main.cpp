
#include "stdafx.h"
#include <iostream>
#include <boost/program_options.hpp>

int main(int argc, char * argv[])
{
  using Path_t = ::boost::filesystem::path;
  using namespace ::boost::program_options;

  options_description Description("Allowed options");

  Description.add_options()
    ("help", "Produce help message.")
    ("file", value<Path_t>(), "Path to source .rml file.")
    ("hlsl", value<Path_t>(), "Path to result .png file.")
    ("glsl", value<Path_t>(), "Path to result .png file.")
    ;

  variables_map Options;

  try
  {
    store(command_line_parser(argc, argv).options(Description).run(), Options);
    notify(Options);
  }
  catch (const ::std::exception &)
  {
    ::std::cout << Description << ::std::endl;
    return -1;
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
      //CompileHLSL(...);
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
      //CompileGLSL(...);
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
