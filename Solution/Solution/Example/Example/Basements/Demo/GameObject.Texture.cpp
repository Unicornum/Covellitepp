
#include "stdafx.h"
#include "GameObject.hpp"
#include <alicorn/boost/filesystem.hpp>
#include <alicorn/image.hpp>
#include <alicorn/logger.hpp>
#include <Covellite/Api/Component.inl>
#include <Covellite/App/Settings.hpp>
#include <Covellite/App/Vfs.hpp>

namespace basement
{

namespace model
{

namespace image = ::alicorn::source::image;
static size_t Index = 0;

class GameObject::Texture::Image final
{
public:
  static ImagePtr_t Load(const Path_t &);

public:
  const image::Universal_t<image::pixel::RGBA> m_Image;

public:
  explicit Image(const Path_t & _PathToTextureFile) :
    m_Image(::covellite::app::Vfs_t::GetInstance().GetData(_PathToTextureFile))
  {

  }
};

/*static*/ auto GameObject::Texture::Image::Load(const Path_t & _FileName) -> ImagePtr_t
{
  using ::covellite::app::Settings_t;

  const auto PathToTextureDirectory =
    Settings_t::GetInstance().Get<Path_t>(uT("PathToTextureDirectory"));
  const auto PathToTextureFile = PathToTextureDirectory / "demo" / _FileName;

  LOGGER(Trace) << "Load texture file: " << PathToTextureFile.c_str();

  return ::std::make_shared<Image>(PathToTextureFile);
}

GameObject::Texture::Texture(const Path_t & _FileName) :
  m_pImage(Image::Load(_FileName)),
  m_Id(uT("Demo.Texture.%TYPE%").Replace(uT("%TYPE%"), Index++))
{
}

Object_t GameObject::Texture::GetObject(void) const
{
  const auto & ImageData = m_pImage->m_Image.GetData();

  // Компонент Data.Texture не используется, т.к. это приводит к глюкам
  // на экране загрузки при использовании общего Present'a для разных уровней
  // LOD.

  return
  {
    Component_t::Make(
    {
      { uT("id"), m_Id },
      { uT("type"), uT("Texture") },
      { uT("mipmapping"), true },
      { uT("data"), ImageData.Buffer.data() },
      { uT("width"), static_cast<int>(ImageData.Width) },
      { uT("height"), static_cast<int>(ImageData.Height) },
    }),
  };
}

float GameObject::Texture::GetRatioXY(void) const
{
  const auto & ImageData = m_pImage->m_Image.GetData();

  return ImageData.Width / static_cast<float>(ImageData.Height);
}

} // namespace model

} // namespace basement
