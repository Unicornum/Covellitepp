
#pragma once
#include <alicorn\boost\filesystem\load-binary-file.hpp>

namespace covellite
{

namespace rocket
{

/**
* \ingroup gCovelliteRocketClasses
* \brief
*  ����� ������ � ������ \ref CovelliteRocketPage \n
*  ���������� ���������� libRocket ��� ������ ������ �� assets.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
* \date
*  07 ������� 2016    \n
*  18 ������� 2016    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2016
*/
class File final :
  public Rocket::Core::FileInterface
{
  using Path_t = Rocket::Core::String;
  using Handle_t = Rocket::Core::FileHandle;
  using Data_t = ::boost::filesystem::binary::Data_t;

  struct FileInfo
  {
    Data_t Data;
    size_t Position;
  };

public:
  // ��������� FileInterface:
  Handle_t Open(const Path_t &) override;
  void Close(Handle_t) override;

  size_t Read(void *, size_t, Handle_t) override;
  bool Seek(Handle_t, long, int) override;
  size_t Tell(Handle_t) override;

  size_t Length(Handle_t) override;

private:
  static Handle_t MakeUniqueHandle(void);

private:
  ::std::map<Handle_t, FileInfo> m_Infos;
};

} // namespace rocket

} // namespace covellite
