
#pragma once
#include <alicorn\std\string.hpp>

/*
An example of use:

::mock::covellite::Configuration Example;

using namespace ::testing;

InSequence Dummy;

EXPECT_CALL(Example, Function(Value1))
  .Times(1)
  .WillOnce(Return(Value2));

const auto Result = Example.Function(Value1);
EXPECT_EQ(Value2, Result);
*/

namespace mock
{

namespace covellite
{

namespace os
{

class Configuration
{
  using String_t = ::alicorn::extension::std::String;

public:
  MOCK_CONST_METHOD0(GetSystemLanguage, String_t(void));
};

} // namespace os

} // namespace covellite

} // namespace mock
