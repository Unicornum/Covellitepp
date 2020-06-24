
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

GameObject::Texture::Texture(const Path_t & _FileName)
{
  LOGGER(Trace) << "Load texture file: " << _FileName.c_str();

  using ::covellite::app::Settings_t;

  const auto PathToTextureDirectory =
    Settings_t::GetInstance().Get<Path_t>(uT("PathToTextureDirectory"));
  const auto PathToTextureFile = PathToTextureDirectory / "demo" / _FileName;

  namespace image = ::alicorn::source::image;

  const image::Universal_t<image::pixel::RGBA> Image =
    ::covellite::app::Vfs_t::GetInstance().GetData(PathToTextureFile);

  const auto & ImageData = Image.GetData();

  // Компонент Data.Texture не используется, т.к. это приводит к глюкам
  // на экране загрузки при использовании общего Present'a для разных уровней
  // LOD.

  static size_t Index = 0;

  m_pTexture = Component_t::Make(
    {
      { uT("id"), uT("Demo.Texture.%TYPE%").Replace(uT("%TYPE%"), Index++) },
      { uT("type"), uT("Texture") },
      { uT("mipmapping"), true },
      { uT("content"), ImageData.Buffer },
      { uT("width"), static_cast<int>(ImageData.Width) },
      { uT("height"), static_cast<int>(ImageData.Height) },
    });

  m_RatioXY = ImageData.Width / static_cast<float>(ImageData.Height);
}

} // namespace model

} // namespace basement
