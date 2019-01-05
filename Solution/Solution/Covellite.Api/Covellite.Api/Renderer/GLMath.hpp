
#pragma once

#if BOOST_COMP_MSVC
#pragma warning(push)
#pragma warning(disable: 4127)
#pragma warning(disable: 4201)
#endif

#define GLM_FORCE_DEPTH_ZERO_TO_ONE // z == [0...1]
#define GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#if BOOST_COMP_MSVC
#pragma warning(pop)
#endif
