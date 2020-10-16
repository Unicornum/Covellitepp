
#include "stdafx.h"
#include "Common.hpp"
#include <alicorn/image.hpp>
#include <Covellite/App/Settings.hpp>
#include <Covellite/App/Vfs.hpp>
#include <Covellite/Api/Component.inl>

using namespace basement;

/******************************************************************************/

Common::Id::Id(void) :
  m_Id(GetUniqueId())
{

}

auto Common::Id::GetStringId(void) const -> String_t 
{
  return String_t{ uT("%ID%") }.Replace(uT("%ID%"), m_Id);
}

bool Common::Id::operator< (const Id & _Id) const
{
  return m_Id < _Id.m_Id;
}

/*static*/ size_t Common::Id::GetUniqueId(void)
{
  static size_t Id = 0;
  return Id++;
}

/******************************************************************************/

Common::Common(WindowExpanse_t & _Window) :
  m_pWindowExpanse(&_Window)
{

}

Common::~Common(void)
{
  for (const auto & Id : m_AllObjects)
  {
    m_pWindowExpanse->RemoveObject(Id);
  }
}

/*static*/ auto Common::LoadTexture(
  const Path_t & _RelativePathToSourceFile,
  const String_t & _Id,
  const String_t & _Destination) -> GameObject_t
{
  return LoadTexture(_RelativePathToSourceFile, _Id,
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Texture") },
      { uT("destination"), _Destination },
    }));
}

/*static*/ auto Common::LoadTexture(
  const Path_t & _RelativePathToSourceFile,
  const String_t & _Id,
  const String_t & _Name,
  const int _Index) -> GameObject_t
{
  return LoadTexture(_RelativePathToSourceFile, _Id,
    Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Texture") },
        { uT("name"), _Name },
        { uT("index"), _Index },
      }));
}

/*static*/ auto Common::LoadTexture(
  const Path_t & _RelativePathToSourceFile, 
  const String_t & _Id,
  const Component_t::ComponentPtr_t & _pData) -> GameObject_t
{
  using ::covellite::app::Settings_t;
  namespace image = ::alicorn::source::image;

  const auto PathToTextureDirectory =
    Settings_t::GetInstance().Get<Path_t>(uT("PathToTextureDirectory"));

  /// [Load texture]
  const image::Universal_t<image::pixel::RGBA> Image
  {
    ::covellite::app::Vfs_t::GetInstance().GetData(
      PathToTextureDirectory / _RelativePathToSourceFile)
  };

  (*_pData)[uT("content")] = Image.GetData().Buffer;
  (*_pData)[uT("width")]  = static_cast<int>(Image.GetData().Width);
  (*_pData)[uT("height")] = static_cast<int>(Image.GetData().Height);

  return
  {
    Component_t::Make(
    {
      { uT("id"), _Id },
      { uT("type"), uT("Texture") },
      { uT("service"), GameObject_t{ _pData } },
    }),
  };

  /// [Load texture]
}

auto Common::CreateObject(const GameObject_t & _Object) -> ObjectId_t
{
  const auto Id = m_pWindowExpanse->CreateObject(_Object);

  m_AllObjects.push_back(Id);
  return Id;
}

void Common::AddToRenderQueue(const ObjectId_t _Id)
{
  m_pWindowExpanse->Add(_Id);
}
