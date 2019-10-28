
#include "stdafx.h"
#include "Draw3DObject.hpp"
#include <alicorn/std/string.hpp>
#include "Description.hpp"

// 17 Март 2019 11:45 (unicornum.verum@gmail.com)
TODO("Недопустимая ссылка на заголовочный файл!");
#include "../Basements/Simple3DObject.hpp"

using namespace layers;

static const auto DescriptionLayer = 
  uT("Окно демонстрации возможностей фреймворка по отображению 3D объектов. ") +
  uT("Представляет собой 3D сцену, содержащую один или несколько ") +
  uT("вращающихся кубиков и три точечных источника света, поверх которых ") +
  uT("отображаются GUI панелей управления (в верхней части отображается FPS ") +
  uT("для оценки возможностей устройства по одновременному отображению ") +
  uT("количества полигонов).<br/>") +
  uT("<br/>") +
  uT("- Включение рендеринга при помощи инстансинга производится в настройках ") +
  uT("программы.<br/>") +
  uT("- Checkbox'ы слева позволяют включать/отключать источники света и ") +
  uT("использование режима PBR-текстур.<br/>") +
  uT("- Кнопками внизу окна можно менять:<br/>") + 
  uT(" + Количество полигонов, из которых состоит кубик.<br/>") +
  uT(" + Количество кубиков в сцене.<br/>") +
  uT("");

Draw3DObject::Draw3DObject(IWindowGui_t & _Window) :
  Layer(_Window, "draw3dobject.rml", uT("3D объект"), DescriptionLayer, true)
{
  using namespace ::alicorn::extension::std;

  GetElement("id_polygons_minus").SetMeaning(uT("\uF056"));
  GetElement("id_polygons").SetMeaning(uT("{POLYGONS}")
    .Replace(uT("{POLYGONS}"), m_Polygons));
  GetElement("id_polygons_plus").SetMeaning(uT("\uF055"));
  GetElement("id_cubes_minus").SetMeaning(uT("\uF056"));
  GetElement("id_cubes").SetMeaning(uT("{CUBES}")
    .Replace(uT("{CUBES}"), m_CubesCount));
  GetElement("id_cubes_plus").SetMeaning(uT("\uF055"));

  const auto ChangeLights = [&](::basement::Lights::Type _Type)
  {
    const auto Offset = (1 << _Type);

    if ((m_LightFlags & Offset) != 0)
    {
      m_LightFlags &= ~Offset;
    }
    else
    {
      m_LightFlags |= Offset;
    }

    m_Events[::events::Simple3DObject.LightsChanged](m_LightFlags);
  };

  using namespace ::covellite::events;

  m_Events[Change.DocumentId(GetId()).ElementId("id_ambient")]
    .Connect([=](void)
  {
    ChangeLights(::basement::Lights::Ambient);
  });

  m_Events[Change.DocumentId(GetId()).ElementId("id_directional")]
    .Connect([=](void)
  {
    ChangeLights(::basement::Lights::Directional);
  });

  m_Events[Change.DocumentId(GetId()).ElementId("id_red")]
    .Connect([=](void)
  {
    ChangeLights(::basement::Lights::Red);
  });

  m_Events[Change.DocumentId(GetId()).ElementId("id_green")]
    .Connect([=](void)
  {
    ChangeLights(::basement::Lights::Green);
  });

  m_Events[Change.DocumentId(GetId()).ElementId("id_blue")]
    .Connect([=](void)
  {
    ChangeLights(::basement::Lights::Blue);
  });

  m_Events[Change.DocumentId(GetId()).ElementId("id_pbr")]
    .Connect([=](void)
  {
    ChangeLights(::basement::Lights::PBR);
  });

  const auto ChangeScene = [&](const ::std::string & _ElementId, int _Value)
  {
    if (!_ElementId.empty())
    {
      GetElement(_ElementId).SetMeaning(uT("{VALUE}")
        .Replace(uT("{VALUE}"), _Value));
    }

    m_Events[::events::Simple3DObject.Start](
      ::std::pair<int, int>{ m_Polygons, m_CubesCount });
    m_Events[::events::Simple3DObject.LightsChanged](m_LightFlags);
  };

  m_Events[Click.DocumentId(GetId()).ElementId("id_polygons_plus")]
    .Connect([&, ChangeScene](void)
  {
    if (m_LastFps < 15) return;

    ChangeScene("id_polygons", m_Polygons *= 2);
  });

  m_Events[Click.DocumentId(GetId()).ElementId("id_polygons_minus")]
    .Connect([&, ChangeScene](void)
  {
    if (m_Polygons <= 12) return;

    ChangeScene("id_polygons", m_Polygons /= 2);
  });

  m_Events[Click.DocumentId(GetId()).ElementId("id_cubes_plus")]
    .Connect([&, ChangeScene](void)
  {
    if (m_LastFps < 15) return;

    ChangeScene("id_cubes", m_CubesCount *= 4);
  });

  m_Events[Click.DocumentId(GetId()).ElementId("id_cubes_minus")]
    .Connect([&, ChangeScene](void)
  {
    if (m_CubesCount <= 1) return;

    ChangeScene("id_cubes", m_CubesCount /= 4);
  });

  m_Events[Drawing.Do].Connect([&](void)
  {
    using namespace ::std::chrono;

    static auto Begin = system_clock::now();

    m_Fps++;

    if (duration_cast<seconds>(system_clock::now() - Begin) >= seconds{ 1 })
    {
      GetElement("id_fps").SetMeaning(uT("FPS: {FPS}")
        .Replace(uT("{FPS}"), m_Fps));

      m_LastFps = m_Fps;
      m_Fps = 0;
      Begin = system_clock::now();
    }
  });

  ChangeScene("", 0);
}
