
#include "stdafx.h"
#include "Common.hpp"
#include <alicorn/image.hpp>
#include <Covellite/App/Settings.hpp>
#include <Covellite/Api/Component.inl>

using namespace basement;

/******************************************************************************/

Common::Id::Id(void) :
  m_Id(GetUniqueId())
{

}

auto Common::Id::GetStringId(void) const -> String_t 
{ 
  return uT("ID").Replace(uT("ID"), m_Id); 
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

Common::Common(const RendersPtr_t & _pRenders) :
  m_pRenders(_pRenders)
{
}

void Common::Render(void) /*override*/
{
  for (const auto Id : m_Scene)
  {
    for (auto & Render : m_Objects[Id])
    {
      Render();
    }
  }
}

void Common::LoadTexture(const Path_t & _RelativePathToSourceFile,
  const String_t & _TextureId)
{
  using ::covellite::app::Settings_t;
  namespace image = ::alicorn::source::image;
    
  const auto PathToTextureDirectory =
    Settings_t::GetInstance().Get<Path_t>(uT("PathToTextureDirectory"));
    
  /// [Load texture]
  const image::Universal_t<image::pixel::RGBA> Image
  {
    ::boost::filesystem::load_binary_file(
      PathToTextureDirectory / _RelativePathToSourceFile)
  };
    
  const auto Renders = m_pRenders->Obtain(
    {
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Texture") },
        { uT("data"), Image.GetData().Buffer.data() },
        { uT("width"), static_cast<int>(Image.GetData().Width) },
        { uT("height"), static_cast<int>(Image.GetData().Height) },
      }),
      Component_t::Make(
      {
        { uT("id"), _TextureId },
        { uT("type"), uT("Texture") },
      }),
    });
    
  ::boost::ignore_unused(Renders);
  /// [Load texture]
}