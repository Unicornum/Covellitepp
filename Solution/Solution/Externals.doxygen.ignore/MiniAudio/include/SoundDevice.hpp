
#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <memory>
#include <algorithm>
#include <cstdio>

#ifdef SOUND_DEVICE_IMPLEMENTATION
# define DR_MP3_IMPLEMENTATION
# define MINIAUDIO_IMPLEMENTATION
#endif

#include <extras/dr_mp3.h>   /* Enables MP3 decoding. */
#include <miniaudio.h>

class SoundDevice final
{
public:
  class Sound final
  {
  public:
    void Dummy(void) {};

  public:
    ::std::vector<uint8_t> m_Data;
    SoundDevice * m_pDevice;
    ma_decoder    m_Decoder;
    ma_uint64     m_Lenght = 0;
    ma_uint64     m_CurrentPosition = 0;

  public:
    Sound(SoundDevice * _pDevice, const ::std::string & _PathToFile) :
      m_pDevice(_pDevice)
    {
      // Автоматический поиск декодера для типа файла
      //const auto result = ma_decoder_init_file(_FileName, NULL, &decoder);

      const auto Result = ma_decoder_init_file_mp3(_PathToFile.c_str(), 
        nullptr, &m_Decoder);
      if (Result != MA_SUCCESS)
      {
        throw ::std::runtime_error{ " ma_decoder_init_...() fail." };
      }

      // Длина композиции в кадрах
      m_Lenght = ma_decoder_get_length_in_pcm_frames(&m_Decoder);

      ::std::lock_guard<::std::mutex> lock(m_pDevice->m_Mutex);
      m_pDevice->m_Decoders.push_back(this);
    }

    Sound(SoundDevice * _pDevice, const ::std::vector<uint8_t> & _Data) :
      m_pDevice(_pDevice),
      m_Data(_Data)
    {
      const auto Result = ma_decoder_init_memory_mp3(m_Data.data(), 
        m_Data.size(), nullptr, &m_Decoder);
      if (Result != MA_SUCCESS)
      {
        throw ::std::runtime_error{ " ma_decoder_init_...() fail." };
      }

      // Длина композиции в кадрах
      m_Lenght = ma_decoder_get_length_in_pcm_frames(&m_Decoder);

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
    memset(pOutput, 0x00, sizeof(float) * _FrameCount *
      2); // Количество каналов звука

    ::std::vector<float> Buffer(_FrameCount * 2, 0.0f);

    ::std::lock_guard<::std::mutex> lock(pSoundDevice->m_Mutex);

    for (auto * pSound : pSoundDevice->m_Decoders)
    {
      const auto FrameCount =
        ma_decoder_read_pcm_frames(&pSound->m_Decoder, Buffer.data(), _FrameCount);
      pSound->m_CurrentPosition += FrameCount;

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
  ma_device_config m_DeviceConfig;
  ma_device m_Device;
  ::std::vector<Sound *> m_Decoders;
  ::std::mutex m_Mutex;

public:
  SoundDevice(void) :
    m_DeviceConfig(ma_device_config_init(ma_device_type_playback))
  {
    m_DeviceConfig.playback.format = ma_format_f32;
    m_DeviceConfig.playback.channels = 2;
    m_DeviceConfig.sampleRate = 44100;
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
