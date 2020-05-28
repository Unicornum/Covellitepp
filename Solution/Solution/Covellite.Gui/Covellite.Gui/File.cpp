
#include "stdafx.h"
#include <Covellite\Gui\File.hpp>
#include <Covellite\App\Vfs.hpp>

namespace covellite
{

namespace gui
{

/**
* \brief
*  Функция открытия указанного файла.
*/
File::Handle_t File::Open(const Path_t & _PathToFile) /*override*/
{
  try
  {
    const auto Handle = File::MakeUniqueHandle();

    m_Infos[Handle].Data = app::Vfs_t::GetInstance().GetData(
      CovelliteGuiStringToUtf8(_PathToFile));
    m_Infos[Handle].Position = 0;
    return Handle;
  }
  catch (const ::std::exception &)
  {
    // Сюда попадаем, если файла нет или он недоступен для чтения.
  }

  return 0;
}

/**
* \brief
*  Функция закрытия ранее открытого файла.
*/
void File::Close(Handle_t _hFile) /*override*/
{
  m_Infos.erase(_hFile);
}

/**
* \brief
*  Функция чтения данных из ранее открытого файла.
*/
size_t File::Read(void * _pBuffer, size_t _Size, Handle_t _hFile) /*override*/
{
  auto itFile = m_Infos.find(_hFile);
  if (itFile == m_Infos.end()) return 0;

  auto & FileInfo = itFile->second;

  const auto DataSize = FileInfo.Data.size() - FileInfo.Position;
  const auto ReadSize = ::std::min(_Size, DataSize);

  ::std::memcpy(_pBuffer, 
    FileInfo.Data.data() + FileInfo.Position, ReadSize);
  FileInfo.Position += ReadSize;

  return ReadSize;
}

/**
* \brief
*  Функция установки позиции чтения из ранее открытого файла.
*/
bool File::Seek(Handle_t _hFile, long _BeginPosition, int _Origin) /*override*/
{
  auto itFile = m_Infos.find(_hFile);
  if (itFile == m_Infos.end()) return false;

  auto & FileInfo = itFile->second;

  size_t NewPosition = 0;

  if (_Origin == SEEK_SET)
  {
    NewPosition = _BeginPosition;
  }
  else if (_Origin == SEEK_CUR)
  {
    NewPosition = FileInfo.Position + static_cast<size_t>(_BeginPosition);
  }
  else if (_Origin == SEEK_END)
  {
    NewPosition = FileInfo.Data.size() + static_cast<size_t>(_BeginPosition);
  }

  if (NewPosition > FileInfo.Data.size()) return false;

  FileInfo.Position = NewPosition;
  
  return true;
}

/**
* \brief
*  Функция получения текущей позиции чтения указанного файла.
*/
size_t File::Tell(Handle_t _hFile) /*override*/
{
  auto itFile = m_Infos.find(_hFile);
  if (itFile == m_Infos.end()) return 0;

  return itFile->second.Position;
}

/**
* \brief
*  Функция получения размера в байтах указанного файла.
*/
size_t File::Length(Handle_t _hFile) /*override*/
{
  auto itFile = m_Infos.find(_hFile);
  if (itFile == m_Infos.end()) return 0;

  return itFile->second.Data.size();
}

/**
* \brief
*  Функция создания уникального идентификатора для открываемого файла.
*/
/*static*/ File::Handle_t File::MakeUniqueHandle(void)
{
  // Первый индекс должен быть > 0, т.к. 0 используется для индикации ошибки.
  static size_t Index = 1;
  return Index++;
}

} // namespace gui

} // namespace covellite
