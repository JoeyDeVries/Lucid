#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class Shader;
class Texture2D;
class Renderer;
class Scene;

// a single particle and its state
struct Particle {
    glm::vec2 Position, Velocity, Scale;
    glm::vec4 Color;
    float Life;

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};


// ParticleEmitter acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class ParticleEmitter
{
private:
    std::vector<Particle>      m_Particles;
    std::shared_ptr<Shader>    m_Shader;
    std::shared_ptr<Texture2D> m_ParticleTexture;
    unsigned int               m_MaxParticles;
    unsigned int               m_VAO;
    unsigned int               m_InstancedArrayVBO;
    unsigned int               m_LastActiveParticle;

    void         respawnParticle(Particle &particle, glm::vec2 range);
    unsigned int getInactiveParticle();
public:
    ParticleEmitter();

    std::shared_ptr<Shader> GetShader();

    bool Init(int maxParticles, glm::mat4 projection);
    void Update(Scene *scene, float deltaTime, int newParticles);

    void Render(Renderer *renderer, glm::mat4 view);
};

#endif