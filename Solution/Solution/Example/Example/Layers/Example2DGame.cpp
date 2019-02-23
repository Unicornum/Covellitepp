﻿
#include "stdafx.h"
#include "Example2DGame.hpp"
#include <alicorn/std/string.hpp>
#include "Description.hpp"
#include "../Basements/Simple2DGame.hpp"

using namespace layers;

static const auto DescriptionLayer =
  uT("Окно демонстрации возможностей фреймворка по созданию простой ") +
  uT("кроссплатформенной 2D игры.<br/>") +
  uT("Часы со стрелками демонстрируют:<br/>") +
  uT("- Использование единой текстуры для отображения нескольких объектов.<br/>") +
  uT("- Использование смешения цвета вершин и текстуры.<br/>") +
  uT("- Вращение объектов.<br/>") +
  uT("- Совместное смещение дочерних объектов при смещении родительского.<br/>") +
  uT("<br/>") +
  uT("Правила игры:<br/>") +
  uT("Двигайте квадрат в центре, избегая касания стен и движущихся ") +
  uT("прямоугольников. Задача - продержаться как можно дольше.<br/>") +
  uT("<br/>") +
  uT("Для перезапуска игры выйдите на стартовый экран и снова зайдите ") +
  uT("на экран игры.<br/>") +
  uT("");

Simple2DGame::Simple2DGame(IWindowGui_t & _Window) :
  Layer(_Window, "simple2dgame.rml", uT("Простая 2D игра"), DescriptionLayer)
{
  using ::events::Simple2DGame;
  using ::covellite::events::Drawing;

  const auto Left = 0.0f;
  const auto Top = 5.0f * m_FontSize;
  const auto Right = static_cast<float>(GetWidth());
  const auto Bottom = static_cast<float>(GetHeight());

  m_Events[Simple2DGame.Start](::std::vector<float>{ Left, Top, Right, Bottom });

  m_Events[Simple2DGame.Finish].Connect([&](const float & _GameTime)
  {
    using namespace ::alicorn::extension::std;

    float Second = 0.0f;
    const float MilliSecond = modff(_GameTime, &Second);

    GetElement("id_game_time").SetMeaning(
      uT("Игровое время: {GAME_TIME_SEC}.{GAME_TIME_MILLI} сек.")
      .Replace(uT("{GAME_TIME_SEC}"), (int)Second)
      .Replace(uT("{GAME_TIME_MILLI}"), (int)(MilliSecond * 100.0f)));
  });

  m_Events[Drawing.Do].Connect([&](void)
  {
    using namespace ::std::chrono;

    static auto Begin = system_clock::now();

    m_Fps++;

    if (duration_cast<seconds>(system_clock::now() - Begin) >= seconds{ 1 })
    {
      using namespace ::alicorn::extension::std;

      GetElement("id_fps").SetMeaning(uT("FPS: {FPS}")
        .Replace(uT("{FPS}"), m_Fps));

      m_LastFps = m_Fps;
      m_Fps = 0;
      Begin = system_clock::now();
    }
  });
}

Simple2DGame::~Simple2DGame(void)
{
  m_Events[::events::Basement.Stop]();
}