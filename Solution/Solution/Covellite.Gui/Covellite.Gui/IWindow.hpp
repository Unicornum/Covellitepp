
#pragma once
#include <memory>
#include <alicorn/boost/filesystem.forward.hpp>
#include <Covellite/Events/IEvents.hpp>
#include <Covellite/Gui/External.hpp>

namespace covellite
{

namespace gui
{

/**
* \ingroup CovelliteGuiGroup
* \brief
*  Класс входит в проект \ref CovelliteGuiPage \n
*  Интерфесный класс окна, отображающего элементы GUI.
*  
* \version
*  1.0.0.0        \n
* \date
*  23 Июль 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class IWindow :
  public ::covellite::events::IEvents
{
protected:
  using Document_t = CovelliteGui::ElementDocument;
  using DocumentPtr_t = ::std::unique_ptr<Document_t, void(*)(Document_t *)>;
  using PathToFile_t = ::boost::filesystem::path;

public:
  virtual DocumentPtr_t LoadDocument(const PathToFile_t &) = 0;
};

} // namespace gui

} // namespace covellite
