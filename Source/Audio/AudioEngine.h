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

#include <irrklang/irrKlang.h>
#include <glm/glm.hpp>

#include <string>

/*
    Manages all audio-related functionality and abstracts from OS-specific commands.
*/
class AudioEngine
{
private:
    // the 3rd party's audio engine API reference point
	irrklang::ISoundEngine *m_AudioEngine;
public:
	AudioEngine();
	~AudioEngine();

    // plays a sound with or without loop and a given volume
	void PlaySound(std::string name, bool loop = false, float volume = 1.0f);
    // plays a sound with or without loop and a give volume at a given location; volume will be reduced at range
	void PlaySoundLocation(std::string name, glm::vec2 location, bool loop = false, float volume = 1.0f);
    // stops playing a sound
    void StopSound(std::string name);
    // stops playing all sounds
    void StopAll();

    // defines the position of the player for 2D/3D positional audio 
    void SetPlayerPosition(glm::vec2);

	// pre-loads (large) sound-files
	void PreLoad();
};
#endif