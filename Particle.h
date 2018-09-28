#pragma once

#include"window.h"
#include"Physics.h"



#define GRID   20

#define TRAIL_LENGTH        50
#define MAX_PARTICLES     1000

struct Point
{
    float x,y;
};


extern Point FlowField[GRID][GRID];

struct Particle
{
    Particle()
    {
    }
    Particle(int x, int y, int life);
    ~Particle(){}
    
    unsigned int ID; 
    Vec2 Position;
    Mass Body;

    int LifeSpan;
    int  Age;
    bool Alive;

    void Update();
    void Render();
    int  Kill();
};

class ParticleEngine
{
public:
    ParticleEngine(int particlecount);

    int MaxParticles;
    int TrailLength;


    bool Init();
    void Update();
    void Render();
    void KillParticle(int ID);
    int  Spawn(int x, int y);

    bool Stopped;

    void Start();
    void Run();
    bool Ready;
    int GetFreeID();
    SDL_Point *Pixels;
    SDL_Point *PastPixels;
    std::vector<Particle> ParticleList;
    int NumberOfParticles;
    std::vector<int> FreeID;
};