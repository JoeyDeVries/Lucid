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
#ifndef GAME_STATE_H
#define GAME_STATE_H

/*
    represents the different states of the game
*/
enum GameState {
    GAME_NONE,
    GAME_MAIN_MENU,
    GAME_MAIN,
	GAME_INTRO,
    GAME_GAME_MENU,
};

#endif