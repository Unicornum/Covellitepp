
#pragma once

#if BOOST_COMP_MSVC 
# pragma warning(push)
# pragma warning(disable: 4458)
#endif

#define STATIC_LIB
#include <Rocket/ProgressBar/Header.h>
#include <Rocket/Core/Geometry.h>
#include <Rocket/Core/Texture.h>
#include <Rocket/Controls/ElementFormControl.h>

#if BOOST_COMP_MSVC 
# pragma warning(pop)
#endif

namespace Rocket 
{

namespace ProgressBar 
{

enum ProgressBarOrientation
{
	ProgressBarOrientationLeft = 0,
	ProgressBarOrientationRight,
	ProgressBarOrientationTop,
	ProgressBarOrientationBottom
};

/**
  A progress bar element using a value between 0 and 1.

  @author Gauthier Billot
 */
class ROCKETPROGRESSBAR_API ElementProgressBar : 
  public Controls::ElementFormControl
{
public:
  Core::String GetValue() const override;
	void SetValue(const Core::String &) override;

protected:
	void OnRender(void) override;
	void OnAttributeChange(const Core::AttributeNameList &) override;
	void OnPropertyChange(const Core::PropertyNameList &) override;

private:
	void GenerateGeometry();
	void LoadTexture(int);
	void LoadTexture(Core::URL &, int, const char *, Core::Geometry &);

private:
	Core::Texture texture[3];
  Core::Vector2f texsize[3][2];
	Core::Vector2f texcoords[3][2];
	Core::Vector2f initial_part_size[3];

	// The geometries used to render this element.
	Core::Geometry start_geometry;
	Core::Geometry center_geometry;
	Core::Geometry end_geometry;
	bool geometry_dirty;
  bool is_mask;
	float value; // The current value of the progress bar.
  ProgressBarOrientation progressbar_orientation;

public:
  ElementProgressBar(const Rocket::Core::String &);
};

} // namespace ProgressBar 

} // namespace Rocket 
