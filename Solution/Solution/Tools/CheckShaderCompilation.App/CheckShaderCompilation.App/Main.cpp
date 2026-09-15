
#include "stdafx.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <alicorn/std/exception.hpp>
#include <alicorn/std/regex.hpp>
#include <alicorn/boost/lexical-cast.hpp>
#include <alicorn/document.hpp>
#include <Covellite/App/Settings.hpp>
#include <Covellite/Api.hpp>
#include "Initial.hpp"
#include "Serializator.inl"
#include "Shell\Shell.h"

class OpenGLEmptyWindow final
{
private:
  ShellRenderInterfaceOpenGL m_OpenGLRenderer;

public:
  OpenGLEmptyWindow(void)
  {
    // Generic OS initialisation, creates a window and attaches OpenGL.
    if (!Shell::Initialise("") ||
      !Shell::OpenWindow(L"CheckShaderCompilation", &m_OpenGLRenderer,
      10, 10, true, false))
    {
      throw STD_EXCEPTION << "Failed to open window";
    }
  }
  ~OpenGLEmptyWindow(void)
  {
    Shell::CloseWindow();
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
      const auto PathToFile = Options["file"].as<Path_t>();

      if (!::boost::filesystem::exists(PathToFile))
      {
        throw EXCEPTION_NO_FILE_LINE(::std::runtime_error) <<
          PathToFile.string() << "(): error C0000: not exists file";
      }

      using BinaryData_t = ::alicorn::extension::std::memory::BinaryData_t;

      const auto LoadFile = [](const Path_t & _Path) -> BinaryData_t
      {
        namespace fs = ::boost::filesystem;

        if (!fs::exists(_Path))
        {
          throw EXCEPTION_NO_FILE_LINE(::std::runtime_error) <<
            _Path.string() << "(): error C0000: not exists file.";
        }

        return fs::load_binary_file(_Path);
      };

      using namespace ::game::repository;

      const auto pShader = Serializator<initial::Shader_t>::Read(
        ::boost::filesystem::load_binary_file(PathToFile))
        .BuildComponent(PathToFile.parent_path(), LoadFile);

      if (Options.count("hlsl"))
      {
        ::covellite::api::CompileShader::AsHLSL(pShader);
        return 0;
      }

      if (Options.count("glsl"))
      {
        OpenGLEmptyWindow oOpenGLEmptyWindow;
        ::covellite::api::CompileShader::AsGLSL(pShader);
        return 0;
      }
    }
    catch (const ::std::exception & _Ex)
    {
      ::std::string sErrorMessage;

      for (auto * pError = _Ex.what(); *pError != 0x00; pError++)
      {
        if (*pError == '\r') continue;
        if (*pError == '\n') continue;

        sErrorMessage += *pError;
      }

      using namespace ::alicorn::extension::std;

      const auto ErrorMessage = 
        string_cast<String, Encoding::UTF8>(sErrorMessage);
        //.Replace(uT("\r\n"), uT("")).Trim(); // ??? не работает ???

      const auto ParseErrorMessage = [&](const String & _Match)
      {
        regex::Match XLSLMatch(_Match);
        if (!XLSLMatch.IsMatch(ErrorMessage)) return false;

        const auto Groups = XLSLMatch.GetGroups();

        const auto Line = ::boost::lexical_cast<int>(Groups[1]) -
          ::boost::lexical_cast<int>(Groups[0]);
        const auto HLSLErrorMessage =
          string_cast<::std::string, Encoding::UTF8>(Groups[2]);

        const auto LoadFile = [](const Path_t & _Path)
        {
          return ::boost::filesystem::load_binary_file(_Path);
        };

        const auto PathToFile = Options["file"].as<Path_t>();

        using namespace ::game::repository;

        const auto [ErrorFile, ErrorLine] = Serializator<initial::Shader_t>::Read(
          ::boost::filesystem::load_binary_file(PathToFile))
          .GetErrorFileLine(PathToFile.parent_path(), LoadFile, Line);

        ::std::cout << ErrorFile << "(" << ErrorLine << ")"
          << HLSLErrorMessage << "." << ::std::endl;
        return true;
      };

      if (
        !ParseErrorMessage(uT(".+\\[header line: ([0-9]+).+\\(([0-9]+),\\d+-\\d+\\)(.+)\\]\\.")) &&
        !ParseErrorMessage(uT(".+\\[header line: ([0-9]+)\\]: (?:\\d\\(([0-9]+)\\) (.+?))(?:\\d\\([0-9]+\\) .+?)*?")))
      {
        ::std::cout << string_cast<::std::string, Encoding::UTF8>(ErrorMessage) << ::std::endl;
      }

      return -1;
    }
  }

  ::std::cout << Description << ::std::endl;
  return -1;
}

namespace alicorn::extension::std
{

using namespace ::covellite::app;

/*static*/ SectionPtr_t Singleton<Section_t>::Make(void)
{
  throw STD_EXCEPTION << "Not needed";
}

} // namespace alicorn::extension::std
