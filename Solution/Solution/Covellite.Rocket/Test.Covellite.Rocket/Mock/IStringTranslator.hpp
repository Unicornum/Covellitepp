
#pragma once

namespace mock
{

namespace covellite
{

namespace rocket
{

class IStringTranslator
{
protected:
  using Utf8String_t = ::std::string;

public:
  const Id_t m_Id;

public:
  virtual Utf8String_t Translate(const Utf8String_t &) { throw ::std::exception(); }

public:
  explicit IStringTranslator(Id_t _Id) : m_Id(_Id) {}
  virtual ~IStringTranslator(void) {}
};

} // namespace rocket

} // namespace covellite

} // namespace mock
