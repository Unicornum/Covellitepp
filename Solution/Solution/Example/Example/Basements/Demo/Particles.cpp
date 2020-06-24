
#include "stdafx.h"
#include "Particles.hpp"
#include <random>
#include <alicorn/std/vector.hpp>
#include <Covellite/Covellite.hpp>
#include "Constants.hpp"

namespace basement
{

namespace model
{

namespace math = ::alicorn::extension::cpp::math;

Particles::Particles(void) :
  GameObject(Another::Particles),
  m_IsParticleInstance{ Constant::GetSettings<bool>(uT("IsParticleInstance")) },
  m_Count{ Constant::GetSettings<::std::size_t>(uT("ParticlesCount")) }
{
  AddTexture("demo.particles.png");
  if (m_IsParticleInstance) BuildInstansedParticles();
  else BuildVertexesParticles();
}

auto Particles::GetObject(const Any_t & /*_Value*/) const /*override*/ -> Objects_t
{
  return m_IsParticleInstance ? GetInstancedObject() : GetVertexesObject();
}

void Particles::BuildVertexesParticles(void)
{
  // fps не зависит от того, рендеритс€ буфер целиком или част€ми.
  static const ::std::size_t CellSize = m_Count;

  m_pParticles = ::std::make_shared<::std::vector<Particle>>(m_Count, Particle
    {
      ::glm::vec3{ 0.0f }, ::glm::vec3{ 0.0f }, 0.0f, 0.0f
    });

  m_Vertexes.resize(3 * m_pParticles->size());
  m_Indices.resize(m_Count / CellSize);

  for (::std::size_t i = 0; i < m_pParticles->size(); i++)
  {
    const auto iOffset = 3 * i;

    m_Vertexes[iOffset + 0].px = 0.0f;
    m_Vertexes[iOffset + 0].pz = 0.0f;
    m_Vertexes[iOffset + 0].pw = 1.0f;
    m_Vertexes[iOffset + 0].ew = 0.0f;
    m_Vertexes[iOffset + 0].tu = 0.5f;
    m_Vertexes[iOffset + 0].tv = 0.0f;

    m_Vertexes[iOffset + 1].pz = 0.0f;
    m_Vertexes[iOffset + 1].pw = 1.0f;
    m_Vertexes[iOffset + 1].ew = 0.0f;
    m_Vertexes[iOffset + 1].tu = 1.0f;
    m_Vertexes[iOffset + 1].tv = 0.75f;

    m_Vertexes[iOffset + 2].pz = 0.0f;
    m_Vertexes[iOffset + 2].pw = 1.0f;
    m_Vertexes[iOffset + 2].ew = 0.0f;
    m_Vertexes[iOffset + 2].tu = 0.0f;
    m_Vertexes[iOffset + 2].tv = 0.75f;

    auto & Indicies = m_Indices[i / CellSize];

    using namespace ::alicorn::extension::std;

    Indicies += ::std::vector<int>
    {
      static_cast<int>(iOffset + 0), 
      static_cast<int>(iOffset + 2), 
      static_cast<int>(iOffset + 1),
    };
  }
}

auto Particles::GetVertexesObject(void) const -> Objects_t
{
  using namespace ::alicorn::extension::std;
  using cbBufferMapper = ::covellite::api::cbBufferMap_t<Vertex_t>;
  using Updater_t = ::covellite::api::Updater_t;

  static const auto ShaderData = ::covellite::app::Vfs_t::GetInstance()
    .GetData("Data\\Shaders\\vParticles.fx");
  static const ::std::size_t Step = 1;

  const cbBufferMapper Mapper = [pParticles = m_pParticles,
    Size = m_pParticles->size()](Vertex_t * _pData)
  {
    if (_pData == nullptr) return true;

    static ::std::size_t FirstIndex = 0;
    FirstIndex++;
    FirstIndex = FirstIndex % Step;

    for (::std::size_t i = FirstIndex; i < Size; i += Step)
    {
      const auto & Source = (*pParticles)[i];
      auto * Vertexes = _pData + 3 * i;

      //Vertexes[0].px = Source.Position.x;
      Vertexes[0].py = 2.0f * Source.HalfSize;
      Vertexes[0].ex = Source.Position.x;
      Vertexes[0].ey = Source.Position.y;
      Vertexes[0].ez = Source.Position.z;

      Vertexes[1].px = 2.0f * Source.HalfSize;
      Vertexes[1].py = -Source.HalfSize;
      Vertexes[1].ex = Source.Position.x;
      Vertexes[1].ey = Source.Position.y;
      Vertexes[1].ez = Source.Position.z;

      Vertexes[2].px = -2.0f * Source.HalfSize;
      Vertexes[2].py = -Source.HalfSize;
      Vertexes[2].ex = Source.Position.x;
      Vertexes[2].ey = Source.Position.y;
      Vertexes[2].ez = Source.Position.z;
    }

    return false;
  };

  const Updater_t Updater = [
    pParticles = m_pParticles, 
    Count = static_cast<int>(m_pParticles->size()),
    ParticleHalfSize = m_ParticleHalfSize](const float _Time)
  {
    static const auto Random = [](const float _From, const float _To)
    {
      static ::std::mt19937 Generator{ ::std::random_device{}() };
      return ::std::uniform_real_distribution<float>{ _From, _To }(Generator);
    };

    namespace math = ::alicorn::extension::cpp::math;

    static auto LastTime = _Time;
    const auto dTime = _Time - LastTime;

    static int FirstIndex = 0;
    FirstIndex++;
    FirstIndex = FirstIndex % Step;

    for (int i = FirstIndex; i < Count; i += Step)
    {
      auto & Particle = (*pParticles)[i];

      if (Particle.Time <= 0.0f)
      {
        // ¬рем€ жизни частицы закончилось, создаем новую.

        const auto Radius = Random(0.0f, 2.5f);
        const auto Angle = Random(0.0f, 360.0f);
        const auto X = Radius * math::degree::Cos(Angle);
        const auto Y = Radius * math::degree::Sin(Angle);

        Particle =
        {
          ::glm::vec3{ X, Y, 0.0f },
          ::glm::vec3{ 0.0f, 0.0f, Random(0.1f, 0.5f) },
          ParticleHalfSize,
          Random(5.0f, 10.0f)
        };
      }
      else
      {
        // —мещаем частицу с учетом ее скорости.
        Particle.Position += Particle.Speed * dTime;

        // ”меньшаем частицу до нул€ на последней секунде жизни.
        if (Particle.Time < 1.0f)
        {
          Particle.HalfSize = Particle.Time * ParticleHalfSize;
        }
      }

      Particle.Time -= dTime;
    }

    LastTime = _Time;
  };

  auto Result =
    GetTexture(0).GetObject() +
    Object_t
  {
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shader.Vertex.Particles") },
      { uT("type"), uT("Shader") },
      { uT("entry"), uT("vsParticles") },
      { uT("content"), ShaderData },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shader.Pixel.Particles") },
      { uT("type"), uT("Shader") },
      { uT("entry"), uT("psParticles") },
      { uT("content"), ShaderData },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Updater.Particles") },
      { uT("type"), uT("Updater") },
      { uT("function"), Updater },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Vertex.Particles") },
      { uT("type"), uT("Buffer") },
      { uT("content"), m_Vertexes },
      { uT("mapper"), Mapper },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.State.Depth.Particles") },
      { uT("type"), uT("State") },
      { uT("kind"), uT("Depth") },
      { uT("enabled"), true },
      { uT("overwrite"), false },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Transform.Particles") },
      { uT("type"), uT("Transform") },
      { uT("kind"), uT("Billboard") },
    }),
  };

  for (::std::size_t i = 0; i < m_Indices.size(); i++)
  {
    using namespace ::alicorn::extension::std;

    const auto Id = String_t{ uT("%ID%") }
      .Replace(uT("%ID%"), i);

    Result += Object_t
    {
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Present.Particles.") + Id },
        { uT("type"), uT("Present") },
        { uT("content"), m_Indices[i] },
      })
    };
  }

  return { Result };
}

void Particles::BuildInstansedParticles(void)
{
  m_pParticles = ::std::make_shared<::std::vector<Particle>>(m_Count, Particle
    {
      ::glm::vec3{ 0.0f }, ::glm::vec3{ 0.0f }, 0.0f, 0.0f
    });

  m_Vertexes.push_back(Vertex_t
    {
      0.0f, 2.0f, 0.0f, 1.0f,
      0.5f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f,
    });

  m_Vertexes.push_back(Vertex_t
    {
      2.0f, -1.0f, 0.0f, 1.0f,
      1.0f, 0.75f,
      0.0f, 0.0f, 0.0f, 0.0f,
    });

  m_Vertexes.push_back(Vertex_t
    {
      -2.0f, -1.0f, 0.0f, 1.0f,
      0.0f, 0.75f,
      0.0f, 0.0f, 0.0f, 0.0f,
    });

  m_Indices.push_back({ 0, 2, 1 });
}

auto Particles::GetInstancedObject(void) const -> Objects_t
{
  using namespace ::alicorn::extension::std;
  using cbBufferMapper = ::covellite::api::cbBufferMap_t<void>;
  using Updater_t = ::covellite::api::Updater_t;

  static const auto ShaderData = ::covellite::app::Vfs_t::GetInstance()
    .GetData("Data\\Shaders\\iParticles.fx");
  static const int Step = 1;

  const cbBufferMapper Mapper = [
    pParticles = m_pParticles,
    Count = static_cast<int>(m_pParticles->size())](void * _pData)
  {
    if (_pData == nullptr) return true;

    auto * Data = reinterpret_cast<Instance *>(_pData);

    static int FirstIndex = 0;
    FirstIndex++;
    FirstIndex = FirstIndex % Step;

    for (int i = FirstIndex; i < Count; i += Step)
    {
      const auto & Source = (*pParticles)[i];

      Data[i].x = Source.Position.x;
      Data[i].y = Source.Position.y;
      Data[i].z = Source.Position.z;
      Data[i].HalfSize = Source.HalfSize;
    }

    return false;
  };

  const Updater_t Updater = [
    pParticles = m_pParticles, 
    Count = static_cast<int>(m_pParticles->size()),
    ParticleHalfSize = m_ParticleHalfSize](const float _Time)
  {
    static const auto Random = [](const float _From, const float _To)
    {
      static ::std::mt19937 Generator{ ::std::random_device{}() };
      return ::std::uniform_real_distribution<float>{ _From, _To }(Generator);
    };

    namespace math = ::alicorn::extension::cpp::math;

    static auto LastTime = _Time;
    const auto dTime = _Time - LastTime;

    static int FirstIndex = 0;
    FirstIndex++;
    FirstIndex = FirstIndex % Step;

    for (int i = FirstIndex; i < Count; i += Step)
    {
      auto & Particle = (*pParticles)[i];

      if (Particle.Time <= 0.0f)
      {
        // ¬рем€ жизни частицы закончилось, создаем новую.

        const auto Radius = Random(0.025f, 2.5f);
        const auto Angle = Random(0.0f, 360.0f);
        const auto X = Radius * math::degree::Cos(Angle);
        const auto Y = Radius * math::degree::Sin(Angle);

        Particle =
        {
          ::glm::vec3{ X, Y, 0.0f },
          ::glm::vec3{ 0.0f, 0.0f, Random(0.1f, 0.5f) },
          ParticleHalfSize,
          Random(5.0f, 10.0f)
        };
      }
      else
      {
        // —мещаем частицу с учетом ее скорости.
        Particle.Position += Particle.Speed * dTime;

        // ”меньшаем частицу до нул€ на последней секунде жизни.
        if (Particle.Time < 1.0f)
        {
          Particle.HalfSize = Particle.Time * ParticleHalfSize;
        }
      }

      Particle.Time -= dTime;
    }

    LastTime = _Time;
  };

  const Object_t InstanceData =
  {
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Buffer") },
      { uT("mapper"), Mapper },
      { uT("count"), m_Count },
      { uT("size"), m_Count * sizeof(Instance) },
    })
  };

  return
  {
    GetTexture(0).GetObject() +
    Object_t
    {
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Shader.Vertex.Particles") },
        { uT("type"), uT("Shader") },
        { uT("entry"), uT("vsParticles") },
        { uT("content"), ShaderData },
        { uT("instance"), Instance::GetDescriptor() },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Shader.Pixel.Particles") },
        { uT("type"), uT("Shader") },
        { uT("entry"), uT("psParticles") },
        { uT("content"), ShaderData },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.State.Depth.Particles") },
        { uT("type"), uT("State") },
        { uT("kind"), uT("Depth") },
        { uT("enabled"), true },
        { uT("overwrite"), false },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Updater.Particles") },
        { uT("type"), uT("Updater") },
        { uT("function"), Updater },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Vertex.Particles") },
        { uT("type"), uT("Buffer") },
        { uT("content"), m_Vertexes },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Transform.Particles") },
        { uT("type"), uT("Transform") },
        { uT("kind"), uT("Billboard") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Present.Particles") },
        { uT("type"), uT("Present") },
        { uT("content"), m_Indices[0] },
        { uT("service"), InstanceData },
      })
    }
  };
}

} // namespace model

} // namespace basement
