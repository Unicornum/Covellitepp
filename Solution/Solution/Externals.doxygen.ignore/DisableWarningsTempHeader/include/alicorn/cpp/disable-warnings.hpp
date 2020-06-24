
#pragma once
#include <alicorn/cpp/warnings.hpp>

#if BOOST_COMP_MSVC

# define ALICORN_DISABLE_ALL_WARNINGS \
  __pragma(warning(push, 0)) \
  __pragma(warning(disable: ALICORN_ALL_WARNINGS))

# define ALICORN_DISABLE_GTEST_WARNINGS \
  __pragma(warning(push, 0)) \
  __pragma(warning(disable: 6326 26440 26455 26409 26433))

# define ALICORN_RESTORE_WARNINGS \
  __pragma(warning(pop))

#else

# define ALICORN_DISABLE_ALL_WARNINGS
# define ALICORN_DISABLE_GTEST_WARNINGS
# define ALICORN_RESTORE_WARNINGS

#endif
