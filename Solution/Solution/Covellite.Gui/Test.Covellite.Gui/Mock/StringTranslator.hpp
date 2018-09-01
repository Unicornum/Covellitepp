
#pragma once

namespace mock
{

namespace covellite
{

namespace gui
{

class StringTranslator :
  public IStringTranslator
{
  using Bank_t = ::std::map<Utf8String_t, Utf8String_t>;

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD0(Constructor, Id_t(void));
    MOCK_METHOD2(Set, void(Id_t, Bank_t));
  };

public:
  void Set(const Bank_t & _Bank)
  {
    Proxy::GetInstance()->Set(m_Id, _Bank);
  }

public:
  StringTranslator(void) :
    IStringTranslator(Proxy::GetInstance()->Constructor())
  {

  }
};

} // namespace gui

} // namespace covellite

} // namespace mock
