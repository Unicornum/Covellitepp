
#pragma once
#include <functional>
#include <alicorn/std/string.forward.hpp>

namespace covellite
{

namespace api
{

class Component;

namespace renderer
{

/**
* \ingroup CovelliteApiRendererGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Класс реализации логик работы компонента \b Updater.
*  
* \version
*  1.0.0.0        \n
* \date
*  24 Июль 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Updater final
{
  using String_t = ::alicorn::extension::std::String;
  using Render_t = ::std::function<void(void)>;
  using ComponentPtr_t = ::std::shared_ptr<Component>;
  using Creator_t = ::std::function<Render_t(const ComponentPtr_t &)>;
  using CreatorPair_t = ::std::pair<String_t, Creator_t>;

public:
  CreatorPair_t GetCreator(void) const;
  void UpdateTime(void);

private:
  Render_t CreateUpdater(const ComponentPtr_t &) const;

private:
  using TimePoint_t = ::std::chrono::system_clock::time_point;
  const TimePoint_t m_StartProgram;
  float             m_CurrentFrameTime;

public:
  Updater(void);
};

} // namespace renderer

} // namespace api

} // namespace covellite
