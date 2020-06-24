
#include "stdafx.h"
#include "Empty.hpp"
#include <chrono>

using namespace basement;

Empty::Empty(WindowExpanse_t & _Window) :
  Common(_Window)
{
  srand(::std::chrono::system_clock::now().time_since_epoch().count());

  const Updater_t Updater = [=](const float)
  {
    for (const auto & ObjectId : m_Objects)
    {
      AddToRenderQueue(ObjectId, rand() % 10000);
    }
  };

  CreateObject(
    {
      Component_t::Make(
        {
          { uT("id"), uT("Updater.Id") },
          { uT("type"), uT("Updater") },
          { uT("function"), Updater }
        })
    });

  for (size_t i = 0; i < 25000; i++)
  {
    GameObject_t GameObject;

    for (size_t r = 0; r < rand() % 6; r++)
    {
      GameObject.push_back(
        Component_t::Make(
          {
            { uT("id"), uT("Test.Id") },
            { uT("type"), uT("Test") },
          }));
    }

    m_Objects.push_back(CreateObject(GameObject));
  }
}
