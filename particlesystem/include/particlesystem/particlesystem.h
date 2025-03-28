#include <particlesystem/particle.h>
#include <particlesystem/emitter.h>
#include <particlesystem/effect.h>

class Particlesystem {
public:
    void update(float dt);

    std::vector<Emitter*> emitters;

    std::vector<Effect*> effects;

    std::vector<Particle> particles;
};