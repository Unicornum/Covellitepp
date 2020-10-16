
#pragma once
#include <gmock/gmock.hpp>
#include <alicorn/std/vector.hpp>
#include <alicorn/std/string/encoding.hpp>
#include <boost/algorithm/string/replace.hpp>

#ifndef OpenGLShader_test

class OpenGLShader_test :
  public ::testing::Test
{

};

#endif

namespace
{

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Present_Index_CreateBuffer_Fail)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pComponent)
  {
    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenBuffers(_))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, _))
      .Times(AtLeast(1));

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(1908232144));

    EXPECT_THROW(itCreator->second(_pComponent), ::std::exception);
  };

  const ::std::vector<int> Source;

  const auto pBuffer = Component_t::Make(
    {
      { uT("content"), Source },
    });

  TestCallRender(pBuffer);

  const auto pData = Component_t::Make(
    {
      { uT("kind"), uT("Buffer") },
      { uT("content"), Source },
    });

  TestCallRender(Component_t::Make({ { uT("service"), Object_t{ pData } } }));
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Present_Index)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  const ::mock::GLuint BufferId = 1908241121;

  ::std::vector<int> Indices =
  {
    1808261927,
    1808261928,
    1808261929,
    1808261930,
    1808261931
  };

  IntroduceBufferSize(Indices);

  auto itPresentCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itPresentCreator);

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pPresent)
  {
    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenBuffers(1))
      .Times(1)
      .WillOnce(Return(BufferId));

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferId))
      .Times(1);

    const auto Size =
      static_cast<::mock::GLsizeiptr>(Indices.size() * sizeof(int));

    EXPECT_CALL(GLProxy, BufferData(GL_ELEMENT_ARRAY_BUFFER, Size, 
      GetExpected(Indices), GL_STATIC_DRAW))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    const auto Render = itPresentCreator->second(_pPresent);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, DrawElements(GL_TRIANGLES, (int)Indices.size(),
      GL_UNSIGNED_INT, nullptr))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(0); // Нельзя!!!

    Render();

    EXPECT_CALL(GLProxy, DeleteBuffers(1, BufferId))
      .Times(1);
  };

  {
    const auto pPresent = Component_t::Make(
      {
        { uT("content"), Indices },
      });

    TestCallRender(pPresent);
  }

  {
    // Индексный буфер через компонент Data

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("content"), Indices },
      });

    TestCallRender(Component_t::Make({ { uT("service"), Object_t{ pData } } }));
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Present_Instance_SizeAndCountInvalidValue)
{
  ::mock::GLProxy GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto TestCallMapper = [&](
    const Component_t::ComponentPtr_t & _pInstance,
    const auto & _Mapper)
  {
    const ::std::vector<int> Source;

    (*_pInstance)[uT("mapper")] = _Mapper;

    const auto pPresent = Component_t::Make(
      {
        { uT("content"), Source },
        { uT("service"), Object_t{ _pInstance } }
      });

    using namespace ::testing;

    EXPECT_CALL(GLProxy, GenBuffers(_)) // Индексный буфер
      .Times(1);

    EXPECT_THROW(itCreator->second(pPresent), ::std::exception);
  };

  auto TestCall = [&](const Component_t::ComponentPtr_t & _pInstance)
  {
    using BufferMapperMaxCount_t = ::std::function<bool(void *)>;
    using BufferMapperCangeCount_t = ::std::function<bool(void *, ::std::size_t &)>;

    TestCallMapper(_pInstance, BufferMapperMaxCount_t{});
    TestCallMapper(_pInstance, BufferMapperCangeCount_t{});
  };

  TestCall(Component_t::Make(
    {
      { uT("kind"), uT("Buffer") },
    }));

  TestCall(Component_t::Make(
    {
      { uT("kind"), uT("Buffer") },
      { uT("size"), ::std::size_t(17) },
    }));

  TestCall(Component_t::Make(
    {
      { uT("kind"), uT("Buffer") },
      { uT("size"), ::std::size_t(32) },
    }));

  TestCall(Component_t::Make(
    {
      { uT("kind"), uT("Buffer") },
      { uT("size"), ::std::size_t(32) },
      { uT("count"), ::std::size_t(5) },
    }));
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Present_Instance_CreateBuffer_Fail)
{
  using BufferMapperMaxCount_t = ::std::function<bool(void *)>;
  using BufferMapperCangeCount_t = ::std::function<bool(void *, ::std::size_t &)>;

  ::mock::GLProxy GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto TestCallRender = [&](const auto & _Mapper)
  {
    const ::std::vector<int> Source;

    const auto pInstanceData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("mapper"), _Mapper },
        { uT("size"), ::std::size_t(16) },
        { uT("count"), ::std::size_t(1) },
      });

    const auto pPresent = Component_t::Make(
      {
        { uT("content"), Source },
        { uT("service"), Object_t{ pInstanceData } }
      });

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenBuffers(_))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, _))
      .Times(AtLeast(1));

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    EXPECT_CALL(GLProxy, GenBuffers(_))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, _))
      .Times(AtLeast(1));

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(1908232144));

    EXPECT_THROW(itCreator->second(pPresent), ::std::exception);
  };

  TestCallRender(BufferMapperMaxCount_t{});
  TestCallRender(BufferMapperCangeCount_t{});
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Present_Instance_MaxCount)
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<void>;

  ::mock::GLProxy GLProxy;

  class MapperProxy_t
  {
  public:
    MOCK_METHOD1(Map, bool(void *));
  };

  MapperProxy_t MapperProxy;

  const ::mock::GLuint IndexBufferId = 1909231813;
  const ::mock::GLuint InstanceBufferId = 1909231820;
  const ::std::size_t InstanceCount = 120;
  const ::std::size_t InstanceStride = 48;
  const ::std::size_t InstanceBufferSize = InstanceCount * InstanceStride;
  const ::mock::GLint ProgramId = 1909231917;

  ::std::vector<int> Indices =
  {
    1808261927,
    1808261928,
    1808261929,
    1808261930,
    1808261931,
    1909231821,
    1909231822,
    1909231823,
  };

  IntroduceBufferSize(Indices);

  void * pLocalData = (void *)1909232013;

  const BufferMapper_t Mapper = [&](void * _pData)
  {
    pLocalData = _pData;
    return MapperProxy.Map(_pData);
  };

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pPresent)
  {
    using namespace ::testing;

    InSequence Dummy;

    // *************************** Индексный буфер ************************** //

    EXPECT_CALL(GLProxy, GenBuffers(1))
      .Times(1)
      .WillOnce(Return(IndexBufferId));

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId))
      .Times(1);

    const auto Size =
      static_cast<::mock::GLsizeiptr>(Indices.size() * sizeof(int));

    EXPECT_CALL(GLProxy, BufferData(GL_ELEMENT_ARRAY_BUFFER, Size, 
      GetExpected(Indices), GL_STATIC_DRAW))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    // ****************************** Инстанс-буфер ************************* //

    EXPECT_CALL(GLProxy, GenBuffers(1))
      .Times(1)
      .WillOnce(Return(InstanceBufferId));

    EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, InstanceBufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, BufferData(GL_ARRAY_BUFFER, InstanceBufferSize, 
      ::std::vector<uint8_t>{}, GL_DYNAMIC_DRAW))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    // ********************************************************************** //

    const auto Render = itCreator->second(_pPresent);
    ASSERT_NE(nullptr, Render);

    // ****************** Отрисовка без обновления ************************** //

    const auto TestCallDraw = [&](void)
    {
      EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId))
        .Times(1);

      EXPECT_CALL(GLProxy, DrawElementsInstanced(GL_TRIANGLES,
        (int)Indices.size(), GL_UNSIGNED_INT, nullptr, InstanceCount))
        .Times(1);

      EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))
        .Times(1);

      EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
        .Times(0); // Нельзя!!!
    };

    EXPECT_CALL(MapperProxy, Map(nullptr))
      .Times(1)
      .WillOnce(Return(false));

    EXPECT_CALL(MapperProxy, Map(_))
      .Times(0);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, InstanceBufferId))
      .Times(0);

    TestCallDraw();
    Render();

    // ******************* Отрисовка с обновлением ************************** //

    const auto TestCallSetInstanceData = [&](
      const ::mock::GLenum _iType,
      const ::mock::GLenum _Type,
      const ::mock::GLint _LocationIndex0,
      auto pData)
    {
      EXPECT_CALL(MapperProxy, Map(nullptr))
        .Times(1)
        .WillOnce(Return(true));

      EXPECT_CALL(MapperProxy, Map(pData))
        .Times(1);

      EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, InstanceBufferId))
        .Times(1);

      EXPECT_CALL(GLProxy, 
        BufferSubDataRaw(GL_ARRAY_BUFFER, 0, InstanceBufferSize, pData))
        .Times(1);

      EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
        .Times(1)
        .WillOnce(Return(&ProgramId));

      constexpr auto BlockSize = sizeof(float) * 4;

      EXPECT_CALL(GLProxy, GetProgramiv(ProgramId, GL_ACTIVE_ATTRIBUTES))
        .Times(1)
        .WillOnce(Return(BlockSize));

      for (int i = 0; i < BlockSize; i++)
      {
        EXPECT_CALL(GLProxy, GetActiveAttribType(ProgramId, i)) 
          .Times(1)
          .WillOnce(Return(_iType));

        EXPECT_CALL(GLProxy, GetActiveAttribName(ProgramId, i, 255))
          .Times(1)
          .WillOnce(Return("iValue" + ::std::to_string(i + 1)));
      }

      for (int i = 0; i < InstanceStride / BlockSize; i++)
      {
        EXPECT_CALL(GLProxy,
          GetAttribLocation(ProgramId, "iValue" + ::std::to_string(i + 1)))
          .Times(1)
          .WillOnce(Return(_LocationIndex0 + i));

        if (_LocationIndex0 + i >= 0)
        {
          EXPECT_CALL(GLProxy, EnableVertexAttribArray(_LocationIndex0 + i))
            .Times(1);

          EXPECT_CALL(GLProxy, VertexAttribPointer(_LocationIndex0 + i, 4, 
            _Type, GL_FALSE, InstanceStride, (void*)(BlockSize * i)))
            .Times(1);

          EXPECT_CALL(GLProxy, VertexAttribDivisor(_LocationIndex0 + i, 1))
            .Times(1);
        }
        else
        {
          EXPECT_CALL(GLProxy, EnableVertexAttribArray(_))
            .Times(0);

          EXPECT_CALL(GLProxy, VertexAttribPointer(_, _, _, _, _, _))
            .Times(0);

          EXPECT_CALL(GLProxy, VertexAttribDivisor(_, _))
            .Times(0);
        }
      }

      EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, 0))
        .Times(1);
    };

    // Первый вызов без значения, чтобы сохранился указатель на локальные
    // данные в pLocalData
    TestCallSetInstanceData(GL_FLOAT_VEC4, GL_FLOAT, 1909232004, _);
    TestCallDraw();
    Render();

    TestCallSetInstanceData(GL_FLOAT_VEC4, GL_FLOAT, 1909232004, pLocalData);
    TestCallDraw();
    Render();

    TestCallSetInstanceData(GL_INT_VEC4, GL_INT, 1909232005, pLocalData);
    TestCallDraw();
    Render();

    TestCallSetInstanceData(2005221201, 2005221201, 2005221202, pLocalData);
    TestCallDraw();
    Render();

    TestCallSetInstanceData(GL_FLOAT_VEC4, GL_FLOAT, -1, pLocalData);
    TestCallDraw();
    Render();

    TestCallSetInstanceData(GL_INT_VEC4, GL_INT, -1, pLocalData);
    TestCallDraw();
    Render();

    // ********************************************************************** //

    EXPECT_CALL(GLProxy, DeleteBuffers(1, IndexBufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, DeleteBuffers(1, InstanceBufferId))
      .Times(1);
  };

  const auto pInstanceData = Component_t::Make(
    {
      { uT("kind"), uT("Buffer") },
      { uT("size"), InstanceBufferSize },
      { uT("mapper"), Mapper },
      { uT("count"), InstanceCount },
    });

  const auto pPresent = Component_t::Make(
    {
      { uT("content"), Indices },
      { uT("service"), Object_t{ pInstanceData } }
    });

  TestCallRender(pPresent);
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Present_Instance_ChangeCount)
{
  using BufferMapper_t = ::std::function<bool(void *, ::std::size_t &)>;

  ::mock::GLProxy GLProxy;

  class MapperProxy_t
  {
  public:
    MOCK_METHOD1(GetCount, ::std::size_t(::std::size_t));
    MOCK_METHOD1(Map, bool(void *));
  };

  MapperProxy_t MapperProxy;

  const ::mock::GLuint IndexBufferId = 1909231813;
  const ::mock::GLuint InstanceBufferId = 1909231820;
  const ::std::size_t InstanceCount = 120;
  const ::std::size_t InstanceStride = 48;
  const ::std::size_t InstanceBufferSize = InstanceCount * InstanceStride;
  const ::mock::GLint ProgramId = 1909231917;

  ::std::vector<int> Indices =
  {
    1808261927,
    1808261928,
    1808261929,
    1808261930,
    1808261931,
    1909231821,
    1909231822,
    1909231823,
  };

  IntroduceBufferSize(Indices);

  void * pLocalData = (void *)1909232013;

  const BufferMapper_t Mapper = [&](void * _pData, ::std::size_t & _Count)
  {
    pLocalData = _pData;
    _Count = MapperProxy.GetCount(_Count);
    return MapperProxy.Map(_pData);
  };

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pPresent)
  {
    using namespace ::testing;

    InSequence Dummy;

    // *************************** Индексный буфер ************************** //

    EXPECT_CALL(GLProxy, GenBuffers(1))
      .Times(1)
      .WillOnce(Return(IndexBufferId));

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId))
      .Times(1);

    const auto Size =
      static_cast<::mock::GLsizeiptr>(Indices.size() * sizeof(int));

    EXPECT_CALL(GLProxy, BufferData(GL_ELEMENT_ARRAY_BUFFER, Size,
      GetExpected(Indices), GL_STATIC_DRAW))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    // ****************************** Инстанс-буфер ************************* //

    EXPECT_CALL(GLProxy, GenBuffers(1))
      .Times(1)
      .WillOnce(Return(InstanceBufferId));

    EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, InstanceBufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, BufferData(GL_ARRAY_BUFFER, InstanceBufferSize,
      ::std::vector<uint8_t>{}, GL_DYNAMIC_DRAW))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    // ********************************************************************** //

    const auto Render = itCreator->second(_pPresent);
    ASSERT_NE(nullptr, Render);

    // ****************** Отрисовка без обновления ************************** //

    const auto TestCallDraw = [&](const ::std::size_t _Count)
    {
      EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId))
        .Times(1);

      EXPECT_CALL(GLProxy, DrawElementsInstanced(GL_TRIANGLES,
        (int)Indices.size(), GL_UNSIGNED_INT, nullptr, _Count))
        .Times(1);

      EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))
        .Times(1);

      EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
        .Times(0); // Нельзя!!!
    };

    EXPECT_CALL(MapperProxy, GetCount(InstanceCount))
      .Times(1)
      .WillOnce(Return(InstanceCount / 2));

    EXPECT_CALL(MapperProxy, Map(nullptr))
      .Times(1)
      .WillOnce(Return(false));

    EXPECT_CALL(MapperProxy, Map(_))
      .Times(0);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, InstanceBufferId))
      .Times(0);

    TestCallDraw(InstanceCount / 2);
    Render();

    // ********** Отрисовка без обновления (слишком большое число) ********** //

    EXPECT_CALL(MapperProxy, GetCount(InstanceCount))
      .Times(1)
      .WillOnce(Return(InstanceCount * 2));

    EXPECT_CALL(MapperProxy, Map(nullptr))
      .Times(1)
      .WillOnce(Return(false));

    EXPECT_CALL(MapperProxy, Map(_))
      .Times(0);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, InstanceBufferId))
      .Times(0);

    TestCallDraw(InstanceCount);
    Render();

    // ******************* Отрисовка с обновлением ************************** //

    const auto TestCallSetInstanceData = [&](
      const ::mock::GLenum _iType,
      const ::mock::GLenum _Type,
      const ::mock::GLint _LocationIndex0,
      auto pData,
      const ::std::size_t _ReturnCount)
    {
      EXPECT_CALL(MapperProxy, GetCount(InstanceCount))
        .Times(1)
        .WillOnce(Return(0));

      EXPECT_CALL(MapperProxy, Map(nullptr))
        .Times(1)
        .WillOnce(Return(true));

      EXPECT_CALL(MapperProxy, GetCount(0))
        .Times(1)
        .WillOnce(Return(_ReturnCount));

      EXPECT_CALL(MapperProxy, Map(pData))
        .Times(1);

      EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, InstanceBufferId))
        .Times(1);

      EXPECT_CALL(GLProxy,
        BufferSubDataRaw(GL_ARRAY_BUFFER, 0, InstanceBufferSize, pData))
        .Times(1);

      EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
        .Times(1)
        .WillOnce(Return(&ProgramId));

      constexpr auto BlockSize = sizeof(float) * 4;

      EXPECT_CALL(GLProxy, GetProgramiv(ProgramId, GL_ACTIVE_ATTRIBUTES))
        .Times(1)
        .WillOnce(Return(BlockSize));

      for (int i = 0; i < BlockSize; i++)
      {
        EXPECT_CALL(GLProxy, GetActiveAttribType(ProgramId, i))
          .Times(1)
          .WillOnce(Return(_iType));

        EXPECT_CALL(GLProxy, GetActiveAttribName(ProgramId, i, 255))
          .Times(1)
          .WillOnce(Return("iValue" + ::std::to_string(i + 1)));
      }

      for (int i = 0; i < InstanceStride / BlockSize; i++)
      {
        EXPECT_CALL(GLProxy,
          GetAttribLocation(ProgramId, "iValue" + ::std::to_string(i + 1)))
          .Times(1)
          .WillOnce(Return(_LocationIndex0 + i));

        if (_LocationIndex0 + i >= 0)
        {
          EXPECT_CALL(GLProxy, EnableVertexAttribArray(_LocationIndex0 + i))
            .Times(1);

          EXPECT_CALL(GLProxy, VertexAttribPointer(_LocationIndex0 + i, 4,
            _Type, GL_FALSE, InstanceStride, (void *)(BlockSize * i)))
            .Times(1);

          EXPECT_CALL(GLProxy, VertexAttribDivisor(_LocationIndex0 + i, 1))
            .Times(1);
        }
        else
        {
          EXPECT_CALL(GLProxy, EnableVertexAttribArray(_))
            .Times(0);

          EXPECT_CALL(GLProxy, VertexAttribPointer(_, _, _, _, _, _))
            .Times(0);

          EXPECT_CALL(GLProxy, VertexAttribDivisor(_, _))
            .Times(0);
        }
      }

      EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, 0))
        .Times(1);
    };

    // Первый вызов без значения, просто чтобы сохранился указатель
    // на локальные данные в pLocalData
    TestCallSetInstanceData(GL_FLOAT_VEC4, GL_FLOAT, 1909232004, _, 0);
    TestCallDraw(0);
    Render();

    TestCallSetInstanceData(GL_FLOAT_VEC4, GL_FLOAT, 1909232004, pLocalData, InstanceCount / 3);
    TestCallDraw(InstanceCount / 3);
    Render();

    TestCallSetInstanceData(GL_INT_VEC4, GL_INT, 1909232005, pLocalData, InstanceCount / 4);
    TestCallDraw(InstanceCount / 4);
    Render();

    TestCallSetInstanceData(2005221201, 2005221201, 2005221202, pLocalData, InstanceCount * 2);
    TestCallDraw(InstanceCount);
    Render();

    TestCallSetInstanceData(GL_FLOAT_VEC4, GL_FLOAT, -1, pLocalData, InstanceCount * 3);
    TestCallDraw(InstanceCount);
    Render();

    TestCallSetInstanceData(GL_INT_VEC4, GL_INT, -1, pLocalData, InstanceCount + 1);
    TestCallDraw(InstanceCount);
    Render();

    // ********************************************************************** //

    EXPECT_CALL(GLProxy, DeleteBuffers(1, IndexBufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, DeleteBuffers(1, InstanceBufferId))
      .Times(1);
  };

  const auto pInstanceData = Component_t::Make(
    {
      { uT("kind"), uT("Buffer") },
      { uT("size"), InstanceBufferSize },
      { uT("mapper"), Mapper },
      { uT("count"), InstanceCount },
    });

  const auto pPresent = Component_t::Make(
    {
      { uT("content"), Indices },
      { uT("service"), Object_t{ pInstanceData } }
    });

  TestCallRender(pPresent);
}

} // unnamed namespace
