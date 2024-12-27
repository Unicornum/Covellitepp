
#pragma once
#include <alicorn\boost\filesystem\load-binary-file.hpp>

namespace covellite
{

namespace gui
{

/**
* \ingroup CovelliteGuiGroup
* \brief
*  Класс входит в проект \ref CovelliteGuiPage \n
*  Реализация интерфейса для чтения файлов.
* \todo
*  Класс используется для внутренней реализации и не должен попадать в Externals.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
* \date
*  07 Декабрь 2016    \n
*  18 Декабрь 2016    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016
*/
class File final :
  public CovelliteGui::FileInterface
{
  using Path_t = CovelliteGui::String;
  using Handle_t = CovelliteGui::FileHandle;
  using Data_t = ::alicorn::extension::std::memory::BinaryData_t;

  struct FileInfo
  {
    Data_t Data;
    size_t Position;
  };

public:
  // Интерфейс FileInterface:
  Handle_t Open(const Path_t &) override;
  void Close(Handle_t) override;

  size_t Read(void *, size_t, Handle_t) override;
  bool Seek(Handle_t, long, int) override;
  size_t Tell(Handle_t) override;

  size_t Length(Handle_t) override;

private:
  static Handle_t MakeUniqueHandle(void) noexcept;

private:
  ::std::map<Handle_t, FileInfo> m_Infos;
};

} // namespace gui

} // namespace covellite
