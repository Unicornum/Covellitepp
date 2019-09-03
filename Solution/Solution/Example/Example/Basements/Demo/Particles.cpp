
#include "stdafx.h"
#include "Particles.hpp"
#include <random>
#include <alicorn/std/vector.hpp>
#include <alicorn/boost/format.inl>
#include <Covellite/Covellite.hpp>

namespace basement
{

namespace model
{

namespace math = ::alicorn::extension::cpp::math;

Particles::Particles(void) :
  GameObject(Another::Particles)
{
  AddTexture("demo.particles.png");
  BuildParticles();
}

auto Particles::GetObject(const Any_t & /*_Value*/) const /*override*/ -> Objects_t
{
  using namespace ::alicorn::extension::std;
  using cbBufferMapper = ::covellite::api::cbBufferMap_t<Vertex_t>;
  using Updater_t = ::covellite::api::Updater_t;

  static const auto ShaderData =
    ::covellite::app::Vfs_t::GetInstance().GetData("Data\\Shaders\\Example.fx");
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
      Vertexes[1].py = - Source.HalfSize;
      Vertexes[1].ex = Source.Position.x;
      Vertexes[1].ey = Source.Position.y;
      Vertexes[1].ez = Source.Position.z;
                 
      Vertexes[2].px = - 2.0f * Source.HalfSize;
      Vertexes[2].py = - Source.HalfSize;
      Vertexes[2].ex = Source.Position.x;
      Vertexes[2].ey = Source.Position.y;
      Vertexes[2].ez = Source.Position.z;
    }

    return false;
  };

  const Updater_t Updater = 
    [pParticles = m_pParticles, Size = static_cast<int>(m_pParticles->size())](const float _Time)
  {
    static const auto Random = [](const float _From, const float _To)
    {
      static ::std::mt19937 Generator{ ::std::random_device{}() };
      return ::std::uniform_real_distribution<float>{ _From, _To }(Generator);
    };

    static const auto ParticleHalfSize = 0.03f;

    namespace math = ::alicorn::extension::cpp::math;

    static auto LastTime = _Time;
    const auto dTime = _Time - LastTime;

    static int FirstIndex = 0;
    FirstIndex++;
    FirstIndex = FirstIndex % Step;

    for (int i = FirstIndex; i < Size; i += Step)
    {
      auto & Particle = (*pParticles)[i];

      if (Particle.Time <= 0.0f)
      {
        // Время жизни частицы закончилось, создаем новую.

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
        // Смещаем частицу с учетом ее скорости.
        Particle.Position += Particle.Speed * dTime;

        // Уменьшаем частицу до нуля на последней секунде жизни.
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
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Shader") },
        { uT("entry"), uT("vsParticles") },
        { uT("data"), ShaderData.data() },
        { uT("count"), ShaderData.size() },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Shader.Vertex.Particles") },
        { uT("type"), uT("Shader") },
      }),
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Shader") },
        { uT("entry"), uT("psParticles") },
        { uT("data"), ShaderData.data() },
        { uT("count"), ShaderData.size() },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Shader.Pixel.Particles") },
        { uT("type"), uT("Shader") },
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
        { uT("data"), m_Vertexes.data() },
        { uT("count"), m_Vertexes.size() },
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
    };

  for (::std::size_t i = 0; i < m_Indices.size(); i++)
  {
    using namespace ::alicorn::extension::std;
    using ::alicorn::extension::boost::Format;

    // Использование здесь String::Replace() увеличивает время формирования
    // идентификатора в 10(!) раз, что существенно сказывается
    // на производительности.
    const auto Id = (Format{ uT("%1%") } % i).ToString();

    Result += Object_t
    {
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Index.Particles.") + Id },
        { uT("type"), uT("Buffer") },
        { uT("data"), m_Indices[i].data() },
        { uT("count"), m_Indices[i].size() },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Present.Particles.") + Id },
        { uT("type"), uT("Present") },
        { uT("kind"), uT("Geometry") },
        { uT("variety"), uT("Billboard") },
      })
    };
  }

  return { Result };
}

void Particles::BuildParticles(void)
{
  static const ::std::size_t Count = 10000;

  // fps не зависит от того, рендерится буфер целиком или частями.
  static const ::std::size_t CellSize = Count;

  m_pParticles = ::std::make_shared<::std::vector<Particle>>(Count, Particle
    {
      ::glm::vec3{ 0.0f }, ::glm::vec3{ 0.0f }, 0.0f, 0.0f
    });

  m_Vertexes.resize(3 * m_pParticles->size());
  m_Indices.resize(Count / CellSize);

  for (::std::size_t i = 0; i < m_pParticles->size(); i++)
  {
    const auto iOffset = static_cast<int>(3 * i);

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
      iOffset + 0, iOffset + 2, iOffset + 1, 
    };
  }
}

} // namespace model

} // namespace basement
