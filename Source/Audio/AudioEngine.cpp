#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
	m_AudioEngine = irrklang::createIrrKlangDevice();
}

AudioEngine::~AudioEngine()
{
	delete m_AudioEngine;
}

void AudioEngine::PlaySound(std::string name, bool loop, float volume)
{
	// IDEA(Joey): create ring buffer of audio play requests that are only played during 
	// engine's update call; at the end of each frame. This allows us to only query
	// the driver at one point of time and allows us to dismiss/combine overlapping
	// audio requests.
	irrklang::ISound *sound = m_AudioEngine->play2D(name.c_str(), loop, true);
	if (sound)
	{
		sound->setVolume(volume);
		sound->setIsPaused(false);
	}
}

void AudioEngine::PlaySoundLocation(std::string name, glm::vec2 location, bool loop, float volume)
{
	irrklang::ISound *sound = m_AudioEngine->play3D(name.c_str(), irrklang::vec3df(location.x, location.y, 0.0), loop, true);
	if (sound)
	{
		sound->setVolume(volume);
		sound->setIsPaused(false);
	}
}

void AudioEngine::StopSound(std::string name)
{
    m_AudioEngine->removeSoundSource(name.c_str());
}
void AudioEngine::StopAll()
{
    m_AudioEngine->stopAllSounds();
}

void AudioEngine::PreLoad()
{
	m_AudioEngine->addSoundSourceFromFile("audio/ambient2.wav", irrklang::E_STREAM_MODE::ESM_AUTO_DETECT, true);
	m_AudioEngine->addSoundSourceFromFile("audio/end_hit.wav", irrklang::E_STREAM_MODE::ESM_AUTO_DETECT, true);


}