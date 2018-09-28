
#include"Vertex2d.h"
#include<vector>
#include<utility>

class Mass{
    public:
        Mass(){}
        Mass(float weight, Vector2D pos);
        Mass(float weight,float x, float y);
        ~Mass(){}


    Vector2D   Velocity,
               Position, 
               Acceleration, 
               Force;

    float  Kg;

    void Apply_Gravity  ();
    void Apply_Physics  ();
    void Set_Force      (float x, float y);
    void Set_Position   (float x,float y);

    Vector2D Get_Position();

    inline float Get_X(){return Position.X;}
    inline float Get_Y(){return Position.Y;}

    struct convert{
        inline float Pounds_to_Kg  (float lb) { return lb * .453; }
        inline float Newtons_to_Kg (float N)  { return N / 9.8;   }
        inline float Kg_to_Newtons (float Kg) { return Kg * 9.8;  }
    }Convert;

    static void Set_Timestep(float time){Current_Timestep = time;}
private:
       
    static float Current_Timestep;
};


class Spring{
    public:
          Spring(){}
         ~Spring(){}

         Spring(Mass &mass1, Mass &mass2, float stiffness, float damper);

    Mass *Mass1,
         *Mass2;

private:
    float  Stiffness,
           Damper;
};



class Structure{
public:
    Structure();
    ~Structure(){}

    std::vector<Mass>    MassList;
    std::vector<Spring>  SpringSprings;

    int     Number_of_Springs,
            Number_of_Masses;

    int     ID;

public:
    int     Number_of_Bodies;
    static  std::vector<Structure *> Structurelist;
    void    Add_Sping(Mass &Mass1, Mass &Mass2);
};



class Ball{
public:
    Ball(float mass,  float x, float y);
    ~Ball(){}
    Mass Body;
    void Render();

    float Radius,
          Angle,
          Speed;
    int ID;

    float Friction;

    int  Get_Overlap(Ball other);
    bool Collision ();
    void Update();
    void Wrap();


public:
    static void Dynamic_Resolution();

    static int Number_of_Balls;
    static std::vector<Ball *> BallList;
    static std::vector<std::pair<Ball*,Ball*>> CollisionList;
};
