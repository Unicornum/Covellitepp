
#pragma once

namespace mock
{

namespace covellite
{

namespace expanse
{

class C3DScene
{
  using ObjectId_t = ::covellite::expanse::ObjectId_t;

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD0(Constructor, Id_t(void));
    MOCK_METHOD4(Add, void(Id_t, size_t, ObjectId_t, size_t));
    MOCK_METHOD2(Add, void(Id_t, ObjectId_t));
    MOCK_METHOD1(Render, void(Id_t));
  };

public:
  void Add(const size_t _Pass, const ObjectId_t _Id, const size_t _Hash)
  {
    Proxy::GetInstance()->Add(m_Id, _Pass, _Id, _Hash);
  }

  void Add(const ObjectId_t _Id)
  {
    Proxy::GetInstance()->Add(m_Id, _Id);
  }

  void Render(void)
  {
    Proxy::GetInstance()->Render(m_Id);
  }

public:
  const Id_t m_Id;

public:
  C3DScene(void) :
    m_Id(Proxy::GetInstance()->Constructor())
  {

  }
};

} // namespace expanse

} // namespace covellite

} // namespace mock
