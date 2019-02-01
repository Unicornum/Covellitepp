
#include "stdafx.h"
#include "Example2DGame.hpp"
#include <alicorn/std/string.hpp>
#include "Description.hpp"
#include "../Basements/Simple2DGame.hpp"

using namespace layers;

static const auto DescriptionLayer =
  uT("Окно демонстрации возможностей фреймворка по созданию простой ") +
  uT("кроссплатформенной 2D игры.<br/>") +
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
  m_Events[::events::Simple2DGame.Start](5 * m_FontSize);

  m_Events[::events::Simple2DGame.Finish].Connect([&](const float & _GameTime)
  {
    using namespace ::alicorn::extension::std;

    float Second = 0.0f;
    const float MilliSecond = modff(_GameTime, &Second);

    GetElement("id_game_time").SetText(string_cast<::std::string, Locale::UTF8>(
      uT("Игровое время: {GAME_TIME_SEC}.{GAME_TIME_MILLI} сек.")
      .Replace(uT("{GAME_TIME_SEC}"), (int)Second)
      .Replace(uT("{GAME_TIME_MILLI}"), (int)(MilliSecond * 100.0f))));
  });

  using namespace ::covellite::events;

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
}

Simple2DGame::~Simple2DGame(void)
{
  m_Events[::events::Basement.Stop]();
}
