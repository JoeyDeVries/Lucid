#include "ParticleEmitter.h"
#include "../Resources/ResourceManager.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/shader.h"
#include "../Renderer/texture2D.h"
#include "../Scene/Scene.h"

#include <random>

ParticleEmitter::ParticleEmitter() : m_MaxParticles(0), m_VAO(0)
{

}

std::shared_ptr<Shader> ParticleEmitter::GetShader()
{
    return m_Shader;
}

bool ParticleEmitter::Init(int maxParticles, glm::mat4 projection)
{
    // initialize emitter state
    m_Shader          = ResourceManager::GetInstance()->LoadShader("particle", "shaders/particle.vs", "shaders/particle.frag");
    m_ParticleTexture = ResourceManager::GetInstance()->LoadTexture("particle", "textures/particle.png", true);
    m_MaxParticles    = maxParticles;

    // initialize render state
    glGenVertexArrays(1, &m_VAO);
    unsigned int VBO; glGenBuffers(1, &VBO);
    glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        float particle_quad[] = {
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindVertexArray(0);

    // initialize instanced array
    glGenBuffers(1, &m_InstancedArrayVBO);
    glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_InstancedArrayVBO);
        glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec4) * m_MaxParticles, NULL, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(glm::vec4) * m_MaxParticles));
        glVertexAttribDivisor(1, 1);
        glVertexAttribDivisor(2, 1);
    glBindVertexArray(0);

    // initialize shader
    m_Shader->Use();
    m_Shader->SetMatrix4("projection", projection);
    m_Shader->SetInteger("particle", 0);

    // initialize maxParticles number of particles to their default state
    for(int i = 0; i < m_MaxParticles; ++i)
        m_Particles.push_back(Particle());    

    return true;
}

int frameCounter = 0;
void ParticleEmitter::Update(Scene *scene, float deltaTime, int newParticles)
{
    std::vector<glm::vec4> particleInfo;
    std::vector<glm::vec4> colors;
    // add new particles 
    if (frameCounter % newParticles == 0) // new particle every 'newParticles' frame
    {
        int unusedParticle = getInactiveParticle();
        respawnParticle(m_Particles[unusedParticle], glm::vec2(scene->GetSceneWidth(), scene->GetSceneHeight()));
    }
    // update all particles
    for (unsigned int i = 0; i < m_MaxParticles; ++i)
    {
        Particle &p = m_Particles[i];
        p.Life -= deltaTime; // reduce life
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
            p.Position += p.Velocity * deltaTime;
            p.Color.a -= p.Life > 5.0f ? deltaTime * -0.2f : deltaTime * 0.2f; // reduce transparency over time
        }
        particleInfo.push_back(glm::vec4(p.Position, p.Scale));
        colors.push_back(p.Color);
    }
    // update buffer for instanced rendering 
    glBindBuffer(GL_ARRAY_BUFFER, m_InstancedArrayVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * m_MaxParticles, &particleInfo[0]);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * m_MaxParticles, sizeof(glm::vec4) * m_MaxParticles, &colors[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ++frameCounter;
}

void ParticleEmitter::Render(Renderer *renderer, glm::mat4 view)
{
    // additive blending to give it a 'glow' effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    m_Shader->Use();
    m_Shader->SetMatrix4("view", view);

    m_ParticleTexture->Bind(0);
    glBindVertexArray(m_VAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, m_MaxParticles); 
    glBindVertexArray(0);

    // reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


std::random_device rd;
std::mt19937 e2(rd());
std::uniform_real_distribution<float> dist(0, 1);
void ParticleEmitter::respawnParticle(Particle &particle, glm::vec2 range)
{
    particle.Position = glm::vec2(dist(e2) * range.x, dist(e2) * range.y);
    particle.Color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    particle.Life = 10.0f;
    particle.Velocity = glm::vec2((dist(e2) * 2.0f - 1.0f) * 15.0f, (dist(e2) * 2.0f - 1.0f) * 7.0f); // TODO(Joey): random!
    particle.Scale = glm::vec2(10.0f);
}

unsigned int ParticleEmitter::getInactiveParticle()
{
    // search from last active particle, this will usually return almost instantly
    for (unsigned int i = m_LastActiveParticle; i < m_MaxParticles; ++i) {
        if (m_Particles[i].Life <= 0.0f) {
            m_LastActiveParticle = i;
            return i;
        }
    }
    // otherwise, do a linear search
    for (GLuint i = 0; i < m_LastActiveParticle; ++i) {
        if (m_Particles[i].Life <= 0.0f) {
            m_LastActiveParticle = i;
            return i;
        }
    }
    // all particles are active, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    m_LastActiveParticle = 0;
    return 0;
}
