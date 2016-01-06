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
#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include <glm/glm.hpp>

#include <vector>
#include <memory>

class Shader;
class Texture2D;
class Renderer;
class Scene;

/*
    a single particle and its state.
*/
struct Particle {
    glm::vec2 Position, Velocity, Scale; // per-particle world position state
    glm::vec4 Color;                     // per-particle color
    float     Life;                      // life of a particle (how long it's been active)

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

/*
    ParticleEmitter acts as a container for rendering a large number of 
    particles by repeatedly spawning and updating particles and killing 
    them after a given amount of time.
*/
class ParticleEmitter
{
private:
    std::vector<Particle>      m_Particles;          // a list of all particles in the emitter (dead and alive)
    std::shared_ptr<Shader>    m_Shader;             // the shader used to render the particles
    std::shared_ptr<Texture2D> m_ParticleTexture;    // the texture used to render the particles
    unsigned int               m_MaxParticles;       // the max number of particles the emitter can generate
    unsigned int               m_VAO;                // the render VAO relevant to rendering the particle quads
    unsigned int               m_InstancedArrayVBO;  // the instanced array vertex buffer object for holding all particle instance data
    unsigned int               m_LastActiveParticle; // stores the last active particle found in the list of particles for subsequent quick lookups

    // respawns a (dead) particle with new (semi-randomly) generated begin values
    void         respawnParticle(Particle &particle, glm::vec2 range);
    // finds the first inactive/dead particle that can be respawned; if none are found, use the first particle of the list
    unsigned int getInactiveParticle();
public:
    ParticleEmitter();

    // getters
    std::shared_ptr<Shader> GetShader();

    // initializes the particle system and its instanced render data
    bool Init(int maxParticles, glm::mat4 projection);
    // updates all particles' state
    void Update(Scene *scene, float deltaTime, int newParticles);

    // renders all active (alive) particles using instanced rendering
    void Render(Renderer *renderer, glm::mat4 view);
};
#endif