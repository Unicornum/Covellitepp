
#pragma once

#include <alicorn\windows.hpp>

class Tools_test
{
protected:
  using Path_t = ::boost::filesystem::path;
  using Process_t = ::alicorn::system::platform::Process;
  using String_t = ::alicorn::extension::std::String;
  using Args_t = ::std::vector<String_t>;

public:
  class Output
  {
  public:
    int ReturnCode;
    ::std::string ConsoleOutput;
  };

public:
  Output RunProcess(const Args_t & _Params) const
  {
    Output Result;

    auto Receiver = [&](const ::std::string & _Line)
    {
      Result.ConsoleOutput += _Line + "\r\n";
    };

    Result.ReturnCode = Process_t{ m_PathToToolExe }
      .StdOut(Receiver)
      .Run(_Params);

    return Result;
  }

private:
  const Path_t m_PathToRootSolutionDirectory =
    ::boost::filesystem::current_path().parent_path().parent_path();
  const Path_t m_PathToToolExe;

protected:
  explicit Tools_test(const Path_t & _ToolExe) :
    m_PathToToolExe(m_PathToRootSolutionDirectory / "Bin" / "x64" / _ToolExe)
  {

  }
};
