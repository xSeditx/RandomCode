#include"Physics.h"
#include "Window.h"

// ---------------_____ STATIC DEFINITIONS _____-----------------

std::vector<Structure*> Structure::Structurelist;

float Mass::Current_Timestep = 1;

int Ball::Number_of_Balls = 0;

std::vector<Ball *> Ball::BallList;
std::vector<std::pair<Ball*,Ball*>> Ball::CollisionList;


Structure::Structure()
    : ID(Number_of_Bodies++)
{
    Structurelist.push_back(this);
}


Spring::Spring(Mass &mass1, Mass &mass2, float stiffness, float damper)
    : Mass1(&mass1), Mass2(&mass2), Stiffness(stiffness), Damper(damper)
{}



Mass::Mass(float weight, Vector2D pos)
    : Kg(weight), Position(pos) , Velocity(0,0), Acceleration(0,0)
{}
Mass::Mass(float weight,float x, float y) : Kg(weight), Position(x,y){}


void Mass::Set_Force(float x,float y)
{
    Force.X = x;
    Force.Y = y;
}
void Mass::Apply_Physics()
{

    Velocity.X *= .800f;
    Velocity.Y *= .800f;

    Acceleration.X = Force.X / Kg ;//* Current_Timestep;
    Acceleration.Y = Force.Y / Kg;// * Current_Timestep;

    Velocity.X += Acceleration.X ;//* Current_Timestep;
    Velocity.Y += Acceleration.Y ;//* Current_Timestep;

    Position.X += Velocity.X;// * Current_Timestep;
    Position.Y += Velocity.Y;// * Current_Timestep;
    Force.X = 0;
    Force.Y = 0;
}
void Mass::Apply_Gravity(){
    Force.Y = Kg * 9.82;

}
void Mass::Set_Position(float x,float y){
   Position.X = x; 
   Position.Y = y;
}


Vector2D Mass::Get_Position(){
    return Position;
}

Ball::Ball(float mass, float x, float y)
    : Body(Mass(mass ,x,y)), Radius(mass), Angle(0), Speed(0), ID(Number_of_Balls++)
{
    BallList.push_back(this);
}

void Ball::Render()
{
   SCREEN->CIRCLE(Body.Position.X, Body.Position.Y, Radius);
   SCREEN->LINE  (Body.Position.X, Body.Position.Y, 
        Body.Position.X + _COS(Angle) * Radius,
        Body.Position.Y + _SIN(Angle) * Radius);
}
void Ball::Update(){
    Body.Apply_Physics();
  if( fabs(Body.Velocity.X +  Body.Velocity.Y) < .01 ) Angle = 0;
}
bool Ball::Collision(){

    for_loop(count, Number_of_Balls){
        //for(Ball &B: &BallList){
        if(Ball::BallList[count]->ID != ID){
            
            if( fabs(Squared( Body.Position.Y - Ball::BallList[count]->Body.Position.Y )   +
                     Squared( Body.Position.X - Ball::BallList[count]->Body.Position.X )) <=
                              (Squared(Radius + Ball::BallList[count]->Radius)))
            {
 
                Ball *B2 = Ball::BallList[count];

                float Distance = sqrtf(Squared( Body.Position.Y - B2->Body.Position.Y ) +
                                       Squared( Body.Position.X - B2->Body.Position.X ));
//---------------------------- Static Resolution ------------------------------------------------------------------------------------------------------
                float Displacement = 0.5f * (Distance - Radius - B2->Radius);
                

                float Xdis = Body.Position.X - B2->Body.Position.X;//(Body.Position.X + Body.Velocity.X) - (Body.)    * .1;
                float Ydis = Body.Position.Y - B2->Body.Position.Y; //(Body.Position.Y + Body.Velocity.Y)    * .1;

                float Xdis2 = B2->Body.Position.X - Body.Position.X; //(Body.Position.Y + Body.Velocity.Y)    * .1;
                float Ydis2 = B2->Body.Position.Y  - Body.Position.Y;//''

   // Body.Position.X -=Displacement * (Body.Position.X - B2->Body.Position.X) / Distance;
                Body.Position.Y -=Displacement * (Body.Position.Y - B2->Body.Position.Y) / Distance;
            
                B2->Body.Position.X +=Displacement * (Body.Position.X - B2->Body.Position.X) / Distance;
                B2->Body.Position.Y +=Displacement * (Body.Position.Y - B2->Body.Position.Y) / Distance;
                float K = .1;
                float Damper = 2;

                Body.Force.X -= -K + (Xdis);// - (Body.Velocity.X / Damper);
                Body.Force.Y -= -K + (Ydis);// - (Body.Velocity.Y / Damper);

                B2->Body.Force.X += -K + (Xdis);
                B2->Body.Force.Y += -K + (Ydis);

//-----------------------------------------------------------------------------------------------------------------------------------------------------
                SCREEN->SET_DRAW_COLOR(RGB(255,0,0)); // DRAW THE LINE SHOWING COLLISION
                SCREEN->LINE( Body.Position.X , Body.Position.Y,  B2->Body.Position.X ,  B2->Body.Position.Y);
                SCREEN->SET_DRAW_COLOR(RGB(255,255,255));
                Ball::CollisionList.emplace_back(this, B2);
//-----------------------------------------------------------------------------------------------------------------------------------------------------


                    return true;
            }
        }
    }
    return false;
}
void Ball::Wrap(){
    if( Body.Position.X > SCREENWIDTH)
    { 
        Body.Position.X = (int)Body.Position.X % SCREENWIDTH;
        return;
    }
    if( Body.Position.Y > SCREENHEIGHT)
    { 
        Body.Position.Y = (int)Body.Position.Y % SCREENHEIGHT;
        return;
    }
    if( Body.Position.Y < 0)
    { 
        Body.Position.Y +=  SCREENHEIGHT;
        return;
    }
    if( Body.Position.X < 0)
    { 
        Body.Position.X +=  SCREENWIDTH;
        return;
    }
}



void Ball::Dynamic_Resolution(){
           //float Momentum = 
              for(auto CBalls : CollisionList){
                    Ball *B1 = CBalls.first;
                    Ball *B2 = CBalls.second;


                    float Distance = sqrtf(Squared( B1->Body.Position.Y - B2->Body.Position.Y ) +
                                          Squared( B1->Body.Position.X - B2->Body.Position.X ));

                    float NormalX = (B2->Body.Position.X - B1->Body.Position.X) / Distance;
                    float NormalY = (B2->Body.Position.Y - B1->Body.Position.Y) / Distance;

                    float TangentX = -NormalY;
                    float TangentY =  NormalX;

                    float dpTan1 = (B1->Body.Velocity.X * TangentX) + (B1->Body.Velocity.Y * TangentY);
                    float dpTan2 = (B2->Body.Velocity.X * TangentX) + (B2->Body.Velocity.Y * TangentY);

                    float dbNormal1 = (B1->Body.Velocity.X * NormalX) + (B1->Body.Velocity.Y * NormalY);
                    float dbNormal2 = (B2->Body.Velocity.X * NormalX) + (B2->Body.Velocity.Y * NormalY);
                          
                    float M1 = (dbNormal1 * (B1->Body.Kg - B2->Body.Kg) + 2.0f * B2->Body.Kg * dbNormal2) / (B1->Body.Kg + B2->Body.Kg);
                    float M2 = (dbNormal2 * (B2->Body.Kg - B1->Body.Kg) + 2.0f * B1->Body.Kg * dbNormal1) / (B1->Body.Kg + B2->Body.Kg) ;

                    B1->Body.Velocity.X = TangentX  * dpTan1  + NormalX  * M1;
                    B1->Body.Velocity.Y = TangentY  * dpTan1  + NormalY  * M1;

                    B2->Body.Velocity.X = TangentX  * dpTan2  + NormalX  * M2;
                    B2->Body.Velocity.Y = TangentY  * dpTan2  + NormalY  * M2;
              }
}