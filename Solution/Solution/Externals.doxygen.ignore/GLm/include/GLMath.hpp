
#pragma once

#ifndef __USING_GTEST

// ќтдельные определени€ в проектах тестировни€ понадобились из-за того, что
// при определении некоторых макросов ::glm::mat4::operator==() возвращает true
// дл€ разных матриц.

//#define GLM_FORCE_NEON
//#define GLM_FORCE_AVX2
//#define GLM_FORCE_AVX
//#define GLM_FORCE_SSE42
//#define GLM_FORCE_SSE41
//#define GLM_FORCE_SSSE3
//#define GLM_FORCE_SSE3
//#define GLM_FORCE_SSE2
//#define GLM_FORCE_SSE

#if BOOST_COMP_MSVC

// явное включение поддержки SSE в настройках проекта не требуетс€, дл€ x64 его
// использование включено по умолчанию (дл€ SSE42 fps в 2 раза выше, чем дл€ SS2).
#define GLM_FORCE_SSE42
//#define GLM_FORCE_SSE2

#elif BOOST_COMP_GCC

// “ребуетс€ включение поддержки NEON в свойствах проекта 
// (C/C++ -> —оздание кода)
#define GLM_FORCE_NEON

#endif

//#define GLM_FORCE_INLINE
//#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES

#endif

#if BOOST_COMP_MSVC
#pragma warning(push)
#pragma warning(disable: 4127)
#pragma warning(disable: 4201)
#pragma warning(disable: 4324)
#endif

#define GLM_FORCE_ALIGNED
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // z == [0...1]
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#if BOOST_COMP_MSVC
#pragma warning(pop)
#endif
