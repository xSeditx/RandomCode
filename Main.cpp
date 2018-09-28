#include "Physics.h"
#include "Window.h"

#include<iostream>

std::vector<Ball*> Balls;
Ball *Selected = nullptr;




void MLDown(int mX, int mY)
{
    Print("Left Down");
    
    if(Selected == nullptr)
    {
        for_loop(count, Ball::Number_of_Balls)
        {
            if(SCREEN->Mouse.Button.Left == true)
                if( Squared( Ball::BallList[count]->Body.Position.X - SCREEN->Mouse.Position.X) +  
                    Squared( Ball::BallList[count]->Body.Position.Y - SCREEN->Mouse.Position.Y) < 
                    Squared( Ball::BallList[count]->Radius))
                {
                    Selected = Ball::BallList[count];
                    break;
                }
        }
    }
    else
    {
        if(SCREEN->Mouse.Button.Right != true){
            Selected->Body.Position.X = SCREEN->Mouse.Position.X;
            Selected->Body.Position.Y = SCREEN->Mouse.Position.Y;
        }
    }
}

void MLUp(int mX, int mY)
{
    Selected = nullptr;
}

void MRDown(int mX, int mY)
{
  
    if(Selected != nullptr)
    {
  Print("Right");
        SCREEN->SET_DRAW_COLOR(RGB(0,0,255));
        SCREEN->LINE(Selected->Body.Position.X, Selected->Body.Position.Y, SCREEN->Mouse.Position.X, SCREEN->Mouse.Position.Y);
        SCREEN->SET_DRAW_COLOR(RGB(255,255,255));

   }
}

void MRUp(int mX, int mY)
{
 Print("RELEASED RIGHT MOUSE");
    if(Selected != nullptr)
    {
        float MAngle = atan2f( Selected->Body.Position.Y - SCREEN->Mouse.Position.Y , 
                               Selected->Body.Position.X - SCREEN->Mouse.Position.X );
        Selected->Angle = MAngle * 180 / 3.15159;
        Selected->Body.Force.X = 5.0f * (Selected->Body.Position.X -(float)SCREEN->Mouse.Position.X);
        Selected->Body.Force.Y = 5.0f * (Selected->Body.Position.Y -(float)SCREEN->Mouse.Position.Y);
           SCREEN->SET_DRAW_COLOR(RGB(0,0,255));
        SCREEN->LINE(Selected->Body.Position.X, Selected->Body.Position.Y, SCREEN->Mouse.Position.X, SCREEN->Mouse.Position.Y);
        SCREEN->SET_DRAW_COLOR(RGB(255,255,255)); 
    }
    SDL_Event sdlevent;
    sdlevent.type = SDL_KEYDOWN;
    sdlevent.key.keysym.sym = SDLK_1;
    SDL_PushEvent(&sdlevent);
}




void main(){
    Window main_window(0,0,SCREENWIDTH,SCREENHEIGHT,"SDL Physics Test");
    main_window. Set_Sync_rate(60);
    Mass::Set_Timestep(1);



    for_loop(count, 350){
        Ball *Bal = new Ball(RANDOM(1)*20, rand()%SCREENWIDTH,rand()%SCREENHEIGHT/ 2);
        Balls.push_back(Bal);
    }

    for(Ball *B: Balls){
            B->Body.Set_Position( B->Body.Get_X(),B->Body.Get_Y());
    }

    main_window.CallBacks.SetOnLButtonDown(MLDown);
    main_window.CallBacks.SetOnLButtonUp(MLUp);
    main_window.CallBacks.SetOnRButtonDown(MRDown);
    main_window.CallBacks.SetOnRButtonUp(MRUp);


    while(main_window.LOOP_GAME()){
          main_window.CLS();

          SCREEN->SET_DRAW_COLOR(RGB(255,255,255));

          for(Ball *C1 : Ball::BallList){
           //     C1->Collision();
           }         

          //  Ball::Dynamic_Resolution();

            for(Ball *B : Ball::BallList){
                //B->Body.Apply_Gravity();
                B->Update();
                B->Collision();
                B->Render();
                B->Wrap();
            }

             SCREEN->LINE2(100,100, SCREEN->Mouse.Angle, 100);

//========================================================================================================================================
        std::cout << " X: " << SCREEN->Mouse.Position.X << "  Y:"  << SCREEN->Mouse.Position.Y << std::endl;
        main_window.SYNC();
    }
}






  //        if(SCREEN->Mouse.Button.Left == false)Selected = nullptr;