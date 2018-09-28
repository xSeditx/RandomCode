

#include"Particle.h"
#include<thread>

float Mass::Current_Timestep = 1;
float Mass::Gravity = 10; 

Point FlowField[GRID][GRID];

void UpdateFlow()
{
    for_loop(yy,GRID)
    {
        for_loop(xx,GRID)
        {
           FlowField[xx][yy].x +=   (RANDOM(.2) - .1);//cos(RADIANS(FlowField[xx][yy].x));
           FlowField[xx][yy].y +=   (RANDOM(.2) - .1);//sin(RADIANS(FlowField[xx][yy].x));
        }
    }
}


int ParticleEngine::GetFreeID()
{
    int ret = FreeID.back();
    FreeID.pop_back();
    return ret;
}

ParticleEngine::ParticleEngine(int particlecount)
{
    MaxParticles = particlecount;
    TrailLength = 50;
    Init();
}
bool ParticleEngine::Init()
{
    Mass::Set_Gravity(0);
    Mass::Set_Timestep(.10);

    ParticleList.reserve(MaxParticles);
    FreeID.reserve(MaxParticles);
    FreeID.clear();

    for_loop(Count, MaxParticles)
    {
        FreeID.push_back(MaxParticles - (Count + 1));
    }

    ParticleList.resize(MaxParticles);
    for_loop(Index, MaxParticles)
    {
        Spawn(rand() % 500, rand() % 500);
    }

    for_loop(yy,GRID)
    {
        for_loop(xx,GRID)
        {
           FlowField[xx][yy].x = RANDOM(20)-10;// - 10;
           FlowField[xx][yy].y = RANDOM(20);// - 10;
        }
    }

    Pixels = new SDL_Point[MaxParticles];

    PastPixels = new SDL_Point[TrailLength * MaxParticles];
    memset(PastPixels, 0, sizeof(SDL_Point) * (TrailLength * MaxParticles));
  return true;
}
void ParticleEngine::Update()
{
    for(auto &P:ParticleList)
    {
        if(P.Alive == true)
        {
            P.Update();
            if(P.Age > P.LifeSpan)
            {
                KillParticle(P.ID);
            }
            if(P.Position.x < 0 || P.Position.y < 0 ||
               P.Position.x > SCREENWIDTH || P.Position.y > SCREENHEIGHT)
            {
                KillParticle(P.ID);
            }
            SDL_Point pos = {P.Position.x, P.Position.y};
            Pixels[P.ID] = pos;

        }
    }

 
    memmove( PastPixels, 
             PastPixels+(MaxParticles), 
            (sizeof(SDL_Point) * MaxParticles) * (TrailLength - 1));
    
    memcpy( PastPixels+(MaxParticles * (TrailLength - 1)),
            Pixels,
            (sizeof(SDL_Point) *MaxParticles));
 // UpdateFlow();  
}
void ParticleEngine::Render()
{
    for_loop(Count, TrailLength)
    {
        float Col = (255 / TrailLength) * (Count);
        SDL_SetRenderDrawColor(SCREEN->Renderer, 255,  Col, Col / (Count +1), 255);
        SDL_RenderDrawPoints(SCREEN->Renderer, &PastPixels[Count * MaxParticles], MaxParticles);
//for_loop(Index, MaxParticles )
//{
//    float x =  PastPixels[(Count * MaxParticles) + Index].x,
//          y =  PastPixels[(Count * MaxParticles) + Index].y;
//
//    SCREEN->SET_PIXELII(x,y,RGB(255,  Col, Col / (Count +1)));
//}
    }

    SDL_SetRenderDrawColor(SCREEN->Renderer,   0,   0,   0, 255);
}
void ParticleEngine::KillParticle(int ID)
{
    ParticleList[ID].Alive = false;
    ParticleList[ID].Age = 0;
    ParticleList[ID].Position = Vec2(0,0);
    Pixels[ID] = Make_Point(0,0);
    FreeID.push_back(ID);
}
int  ParticleEngine::Spawn(int x, int y)
{
    if(FreeID.empty() != true)
    {
       int newID = GetFreeID();
       ParticleList[newID] = Particle(x,y, RANDOM(500));
       ParticleList[newID].ID = newID;
       return newID;
    }
  return 0;
}
void ParticleEngine::Run()
{
    Stopped = false;
    while(Stopped == false)
    {
            Update();
            Render();
UpdateFlow();
    }
}
void ParticleEngine::Start()
{
 std::thread *t = new std::thread(&ParticleEngine::Run, this);

}
Particle::Particle(int x, int y, int life)
{
    Age = 0;
    Alive = true;
    Body = Mass(1, Vec2(x,y));
    LifeSpan = life;
    Body.Velocity.y = -RANDOM(20);
    Body.Velocity.x = RANDOM(20) - 10;
    Position = Body.Position;
}
void Particle::Update()
{
    Age++;

    int X = (((float)(GRID) / SCREENWIDTH)) * (Body.Position.x);
    int Y = (((float)(GRID) / SCREENHEIGHT))* (Body.Position.y);
    if(Y < 0) Y = 0; if(Y > GRID)Y = GRID;
    if(X < 0) X = 0; if(X > GRID)X = GRID;
    Body.Force.y =   RANDOM(20)-10;  // FlowField[X][Y].y; //
    Body.Force.x =   RANDOM(20)-10; // FlowField[X][Y].x; //
    Body.Update();
    Position = Body.Position;

}
int  Particle::Kill()
{
    Alive = false;
    Age  = 0;
    Position = Vec2(0,0);
    return ID;
}