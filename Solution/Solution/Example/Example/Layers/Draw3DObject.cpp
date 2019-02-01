
#include "stdafx.h"
#include "Draw3DObject.hpp"
#include <alicorn/std/string.hpp>
#include "Description.hpp"
#include "../Basements/Simple3DObject.hpp"

using namespace layers;

static const auto DescriptionLayer = 
  uT("Окно демонстрации возможностей фреймворка по отображению 3D объектов. ") +
  uT("Представляет собой 3D сцену, содержащую один или несколько ") +
  uT("вращающихся кубиков и три движущихся источника света, поверх которых ") +
  uT("отображаются GUI панелей управления (в верхней части отображается FPS ") +
  uT("для оценки возможностей устройства по одновременному отображению ") +
  uT("количества полигонов).<br/>") +
  uT("<br/>") + 
  uT("Кнопками внизу окна можно менять:<br/>") + 
  uT("- Количество полигонов, из которых состоит кубик.<br/>") +
  uT("- Количество отображаемых кубиков.<br/>") +
  uT("");

Draw3DObject::Draw3DObject(IWindowGui_t & _Window) :
  Layer(_Window, "draw3dobject.rml", uT("3D объект"), DescriptionLayer)
{
  using namespace ::alicorn::extension::std;

  GetElement("id_polygons_minus").SetText(u8"\uF056");
  GetElement("id_polygons").SetText(string_cast<::std::string, Locale::UTF8>(
    uT("{POLYGONS}").Replace(uT("{POLYGONS}"), m_Polygons)));
  GetElement("id_polygons_plus").SetText(u8"\uF055");
  GetElement("id_cubes_minus").SetText(u8"\uF056");
  GetElement("id_cubes").SetText(string_cast<::std::string, Locale::UTF8>(
    uT("{CUBES}").Replace(uT("{CUBES}"), m_CubesCount)));
  GetElement("id_cubes_plus").SetText(u8"\uF055");

  using namespace ::covellite::events;

  m_Events[Click.DocumentId(GetId()).ElementId("id_polygons_plus")]
    .Connect([&](void) 
  {
    if (m_LastFps < 15) return;

    m_Polygons *= 2;
    m_Events[::events::Simple3DObject.Start](
      ::std::pair<int, int>{ m_Polygons, m_CubesCount });

    GetElement("id_polygons").SetText(string_cast<::std::string, Locale::UTF8>(
      uT("{POLYGONS}").Replace(uT("{POLYGONS}"), m_Polygons)));
  });

  m_Events[Click.DocumentId(GetId()).ElementId("id_polygons_minus")]
    .Connect([&](void)
  {
    if (m_Polygons <= 12) return;

    m_Polygons /= 2;
    m_Events[::events::Simple3DObject.Start](
      ::std::pair<int, int>{ m_Polygons, m_CubesCount });

    GetElement("id_polygons").SetText(string_cast<::std::string, Locale::UTF8>(
      uT("{POLYGONS}").Replace(uT("{POLYGONS}"), m_Polygons)));
  });

  m_Events[Click.DocumentId(GetId()).ElementId("id_cubes_plus")]
    .Connect([&](void)
  {
    if (m_LastFps < 15) return;

    m_CubesCount *= 4;
    m_Events[::events::Simple3DObject.Start](
      ::std::pair<int, int>{ m_Polygons, m_CubesCount });

    GetElement("id_cubes").SetText(string_cast<::std::string, Locale::UTF8>(
      uT("{CUBES}").Replace(uT("{CUBES}"), m_CubesCount)));
  });

  m_Events[Click.DocumentId(GetId()).ElementId("id_cubes_minus")]
    .Connect([&](void)
  {
    if (m_CubesCount <= 1) return;
    
    m_CubesCount /= 4;
    m_Events[::events::Simple3DObject.Start](
      ::std::pair<int, int>{ m_Polygons, m_CubesCount });

    GetElement("id_cubes").SetText(string_cast<::std::string, Locale::UTF8>(
      uT("{CUBES}").Replace(uT("{CUBES}"), m_CubesCount)));
  });

  m_Events[Drawing.Do].Connect([&](void)
  {
    using namespace ::std::chrono;

    static auto Begin = system_clock::now();

    m_Fps++;

    if (duration_cast<seconds>(system_clock::now() - Begin) >= seconds{ 1 })
    {
      using namespace ::alicorn::extension::std;

      GetElement("id_fps").SetText(string_cast<::std::string, Locale::UTF8>(
        uT("FPS: {FPS}").Replace(uT("{FPS}"), m_Fps)));

      m_LastFps = m_Fps;
      m_Fps = 0;
      Begin = system_clock::now();
    }
  });

  m_Events[::events::Simple3DObject.Start](
    ::std::pair<int, int>{ m_Polygons, m_CubesCount });
}

Draw3DObject::~Draw3DObject(void)
{
  m_Events[::events::Basement.Stop]();
}
