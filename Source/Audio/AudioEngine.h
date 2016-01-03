/*******************************************************************
** Copyright (C) 2014-2015 {Joey de Vries} {joey.d.vries@gmail.com}
**
** This code is part of Lucid.
** https://github.com/JoeyDeVries/Lucid
**
** Lucid is free software: you can redistribute it and/or modify it
** under the terms of the CC BY-NC 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
*******************************************************************/
#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H
#include <string>
#include <irrklang/irrKlang.h>
#include <glm/glm.hpp>

class AudioEngine
{
private:
	irrklang::ISoundEngine *m_AudioEngine;
public:
	AudioEngine();
	~AudioEngine();


	void PlaySound(std::string name, bool loop = false, float volume = 1.0f);
	void PlaySoundLocation(std::string name, glm::vec2 location, bool loop = false, float volume = 1.0f);
    void StopSound(std::string name);
    void StopAll();

    void SetPlayerPosition(glm::vec2);

	// pre-loads (large) sound-files
	void PreLoad();
};

#endif