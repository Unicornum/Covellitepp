
#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <memory>
#include <algorithm>
#include <cstdio>

#include <miniaudio/miniaudio.hpp>

class SoundDevice final
{
public:
  class Sound final
  {
  public:
    // cppcheck-suppress functionStatic
    void Dummy(void) const {};

  public:
    SoundDevice *           m_pDevice;
    ::std::vector<uint8_t>  m_Data;
    ma_decoder              m_Decoder;
    ma_uint64               m_Lenght = 0;
    ma_uint64               m_CurrentPosition = 0;

  public:
    Sound(SoundDevice * _pDevice, const ::std::string & _PathToFile) :
      m_pDevice(_pDevice)
    {
      const auto Result = 
        ma_decoder_init_file(_PathToFile.c_str(), &_pDevice->m_DecoderConfig, &m_Decoder);
      if (Result != MA_SUCCESS)
      {
        throw ::std::runtime_error{ " ma_decoder_init_...() fail." };
      }

      // Длина композиции в кадрах
      ma_decoder_get_length_in_pcm_frames(&m_Decoder, &m_Lenght);

      ::std::lock_guard<::std::mutex> lock(m_pDevice->m_Mutex);
      m_pDevice->m_Decoders.push_back(this);
    }

    Sound(SoundDevice * _pDevice, const ::std::vector<uint8_t> & _Data) :
      m_pDevice(_pDevice),
      m_Data(_Data)
    {
      const auto Result = ma_decoder_init_memory(m_Data.data(), 
        m_Data.size(), &_pDevice->m_DecoderConfig, &m_Decoder);
      if (Result != MA_SUCCESS)
      {
        throw ::std::runtime_error{ " ma_decoder_init_...() fail." };
      }

      // Длина композиции в кадрах
      ma_decoder_get_length_in_pcm_frames(&m_Decoder, &m_Lenght);

      ::std::lock_guard<::std::mutex> lock(m_pDevice->m_Mutex);
      m_pDevice->m_Decoders.push_back(this);
    }

    ~Sound(void)
    {
      ::std::lock_guard<::std::mutex> lock(m_pDevice->m_Mutex);
      const auto itThis = ::std::find(m_pDevice->m_Decoders.cbegin(), 
        m_pDevice->m_Decoders.cend(), this);
      m_pDevice->m_Decoders.erase(itThis);
      ma_decoder_uninit(&m_Decoder);
    }
  };

public:
  template<class T>
  ::std::shared_ptr<Sound> Make(const T & _PathToFile)
  {
    return ::std::make_shared<Sound>(this, _PathToFile);
  }

private:
  static void cbData(
    ma_device * _pDevice,
    void * _pOutput,
    const void *,
    ma_uint32 _FrameCount)
  {
    auto * pSoundDevice = reinterpret_cast<SoundDevice *>(_pDevice->pUserData);
    if (pSoundDevice == nullptr) return;

    auto * pOutput = reinterpret_cast<float *>(_pOutput);
    memset(pOutput, 0x00, sizeof(float) * _FrameCount * CHANNEL_COUNT);

    ::std::vector<float> Buffer(_FrameCount * CHANNEL_COUNT, 0.0f);

    ::std::lock_guard<::std::mutex> lock(pSoundDevice->m_Mutex);

    for (auto * pSound : pSoundDevice->m_Decoders)
    {
      ma_uint64 FrameCount = 0;

      ma_decoder_read_pcm_frames(&pSound->m_Decoder, Buffer.data(), _FrameCount, &FrameCount);

      if (FrameCount == 0)
      {
        // Повтор воспроизведения
        pSound->m_CurrentPosition = 0;

        // Зацикливает воспроизведение, начиная с конкретного кадра
        const auto Result = ma_decoder_seek_to_pcm_frame(&pSound->m_Decoder, 0);
        if (Result != MA_SUCCESS) continue;
      }
      else
      {
        pSound->m_CurrentPosition += FrameCount;

        // Смешивание сэмплов нескольких сигналов
        for (::std::size_t i = 0; i < Buffer.size(); i++)
        {
          pOutput[i] += Buffer[i];
            //* 0.25f; // Громкость звука
        }
      }
    }
  }

private:
  static const ma_uint32 CHANNEL_COUNT = 2;
  const ma_decoder_config m_DecoderConfig;
  ma_device_config m_DeviceConfig;
  ma_device m_Device;
  ::std::vector<Sound *> m_Decoders;
  ::std::mutex m_Mutex;

public:
  SoundDevice(void) :
    m_DecoderConfig{ ma_decoder_config_init(ma_format_f32, CHANNEL_COUNT, 44100) },
    m_DeviceConfig(ma_device_config_init(ma_device_type_playback))
  {
    m_DeviceConfig.playback.format = m_DecoderConfig.format;
    m_DeviceConfig.playback.channels = m_DecoderConfig.channels;
    m_DeviceConfig.sampleRate = m_DecoderConfig.sampleRate;
    m_DeviceConfig.dataCallback = cbData;
    m_DeviceConfig.pUserData = this;

    auto Result = ma_device_init(NULL, &m_DeviceConfig, &m_Device);
    if (Result != MA_SUCCESS)
    {
      throw ::std::runtime_error{ "ma_device_init() fail." };
    }

    Result = ma_device_start(&m_Device);
    if (Result != MA_SUCCESS)
    {
      throw ::std::runtime_error{ "ma_device_start() fail." };
    }
  }
  ~SoundDevice(void)
  {
    ma_device_uninit(&m_Device);
  }
};
