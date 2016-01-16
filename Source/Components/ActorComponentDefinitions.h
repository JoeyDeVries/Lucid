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
#ifndef ACTOR_COMPONENT_DEFINITIONS_H
#define ACTOR_COMPONENT_DEFINITIONS_H

/*
    The default types of actors available.
*/
enum DEFAULT_ACTOR_TYPES {
    ACTOR_EMPTY,
    ACTOR_STATIC,
    ACTOR_STATE_BLOCK,
    ACTOR_ENEMY,
    ACTOR_PLAYER,
    ACTOR_LANTERN,
    ACTOR_COMPLETE_CHECK,
    ACTOR_SIGNPOST,
    ACTOR_DEATHTOUCH,
    ACTOR_MOVE_LOOP,
};
#endif