
#pragma once

#if BOOST_COMP_MSVC
#pragma warning(push)
#pragma warning(disable: 4127)
#pragma warning(disable: 4201)
#pragma warning(disable: 4324)

//#define GLM_FORCE_AVX2

// Явное включение поддержки SSE в настройках проекта не требуется, для x64 его
// использование включено по умолчанию (для SSE42 fps в 2 раза выше, чем для SS2).
#define GLM_FORCE_SSE42
//#define GLM_FORCE_SSE2
#endif

#if BOOST_COMP_GCC
// Требуется включение поддержки NEON в свойствах проекта 
// (C/C++ -> Создание кода)
#define GLM_FORCE_NEON
#endif

// Если не указать явно, будут использоваться настройки проекта.
//#define GLM_FORCE_NEON
//#define GLM_FORCE_AVX2
//#define GLM_FORCE_AVX
//#define GLM_FORCE_SSE42
//#define GLM_FORCE_SSE41
//#define GLM_FORCE_SSSE3
//#define GLM_FORCE_SSE3
//#define GLM_FORCE_SSE2
//#define GLM_FORCE_SSE

#define GLM_FORCE_ALIGNED
#define GLM_FORCE_INLINE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // z == [0...1]
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#if BOOST_COMP_MSVC
#pragma warning(pop)
#endif
