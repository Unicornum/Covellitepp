
#pragma once

// 27 Май 2020 12:00 (unicornum.verum@gmail.com)
TODO("Удалить файл и заменить ссылку, когда будет исправлен файл RmlUi/RmlUi.mock.hpp");

#define FontDatabase FontDatabase_dummy
#define LoadFontFace LoadFontFace_dummy
#include <RmlUi/RmlUi.mock.hpp>
#undef FontDatabase
#undef LoadFontFace

namespace mock
{

namespace Rml
{

namespace Core
{

class FontDatabase
{
public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(LoadFontFace, bool(String));
    MOCK_METHOD2(LoadFontFace, bool(::std::vector<byte>, String));
  };
};

namespace Style
{

enum class FontStyle : uint8_t { Normal, Italic };
enum class FontWeight : uint8_t { Normal, Bold };

} // namespace Style

namespace
{

bool LoadFontFace(const String & _Path)
{
  return FontDatabase::Proxy::GetInstance()->LoadFontFace(_Path);
}

bool LoadFontFace(const byte * data, int data_size, const String & font_family,
  Style::FontStyle style, Style::FontWeight weight)
{
  return FontDatabase::Proxy::GetInstance()->LoadFontFace(
    ::std::vector<byte>{ data, data + data_size }, font_family);
}

} // unnamed namespace

} // namespace Core

} // namespace RmlUi

} // namespace mock
