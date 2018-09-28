#include"assets.h"


#if USING_ASSETS

#include<memory>


AssetManager<SoundEffect> SoundEffect::Manager;
AssetManager<Sprite>           Sprite::Manager;
AssetManager<Image>             Image::Manager;
AssetManager<Music>             Music::Manager;

const Image *ErrorImage;


// VERY RARE AND OCCASIONAL ERROR UPON LOADING 
//  00EF836F  int         3  
//  __unwindfunclet$??0SpriteSheet@@QAE@PAD@Z$0:
//  00EF8370  mov         eax,dword ptr [ebp-0E0h]  
//  00EF8376  push        eax  
//  00EF8377  call        operator delete (0EBE6D1h)  
//  00EF837C  pop         ecx  
//  00EF837D  ret  
//  __ehhandler$??0SpriteSheet@@QAE@PAD@Z:
//  00EF837E  mov         edx,dword ptr [esp+8]  <---- Here is the Final Line of code
//  00EF8382  lea         eax,[edx+0Ch]  
//  00EF8385  mov         ecx,dword ptr [edx-0F8h]  
//  The error is Occuring in d3d9.dll calling igdumd32.dll but I believe the later is just an error handler.
//  	d3d9.dll!00c98c27()	Unknown <- Last Function on the callstack.
// I believe it has something to do with some sort of conflict as the program is loading and the
// threads are fighting over resources but I am not really sure and do not have time to figure it out


//==================================================================================================================================
//_______________________________________  Sprite Class ______________________________________________________________________
//                                                                                                                                  
//                                                                                                                                  
//==================================================================================================================================
Sprite::~Sprite()
{
    if(ID > 0)
    {
        Manager.Remove(ID);
    }
}
Sprite::Sprite(char *name, Image *source, int numstates)
      : TotalStates(numstates),
        SpriteSheet (source)
{
        Name         = name;
        TotalStates = numstates;
        States.reserve(numstates);

        for_loop(count, TotalStates)
        {
            State temp;
            States.emplace_back(temp);
        }
        
        Angle          = 0;
        AnimationSpeed = 150;
        Animated       = true;
        CurrentState   = 0;

        ID             = Manager.Add(this);
}
void Sprite::Update()
{
// Update information about this sprite, perhaps the specific frame or anythign of that nature
}
void Sprite::Render()
{
    Uint32 ticks = SDL_GetTicks();

    if( Animated == true)
    {
     
        States[CurrentState].CURRENT_FRAME = (ticks / AnimationSpeed) % States[CurrentState].TOTAL_FRAMES;
    }

    SDL_Rect srcrect = States[CurrentState].FRAMES[States[CurrentState].CURRENT_FRAME]; //{ frames * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT };
    SDL_Rect dstrect = {Position.x - (Size.x *.5), Position.y - (Size.y * .5), Size.x, Size.y};
    SDL_RenderCopy(SCREEN->Renderer, SpriteSheet->ImageTexture, &srcrect, &dstrect);  
}
void Sprite::Render(float angle)
{
    Uint32 ticks = SDL_GetTicks();

    if( Animated == true)
    {
     
        States[CurrentState].CURRENT_FRAME = (ticks / AnimationSpeed) % States[CurrentState].TOTAL_FRAMES;
    }

    SDL_Rect srcrect = States[CurrentState].FRAMES[States[CurrentState].CURRENT_FRAME]; //{ frames * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT };
    SDL_Rect dstrect = {Position.x - (Size.x *.5), Position.y - (Size.y * .5), Size.x, Size.y};
//    SDL_RenderCopy(SCREEN->Renderer, SpriteSheet->Sheet->ImageTexture, &srcrect, &dstrect);  
    SDL_Point Center = {Size.x / 2,Size.y/2};
    SDL_RenderCopyEx(SCREEN->Renderer, SpriteSheet->ImageTexture ,  &srcrect, &dstrect, Angle, &Center, SDL_RendererFlip(0));
}
void Sprite::Render(Vec2 position)
{
    Uint32 ticks = SDL_GetTicks();

    if( Animated == true)
    {
        States[CurrentState].CURRENT_FRAME = (ticks / AnimationSpeed) % States[CurrentState].TOTAL_FRAMES;
    }

    SDL_Rect srcrect = States[CurrentState].FRAMES[States[CurrentState].CURRENT_FRAME]; //{ frames * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT };
    SDL_Rect dstrect = {position.x - (Size.x *.5), position.y - (Size.y * .5), Size.x, Size.y};

    SDL_Point Center = {Size.x / 2,Size.y/2};
    SDL_RenderCopyEx(SCREEN->Renderer, SpriteSheet->ImageTexture ,  &srcrect, &dstrect, Angle, &Center,SDL_RendererFlip(0));
}
void Sprite::Render(Vec2 pos, float angle)
{
    Uint32 ticks = SDL_GetTicks();

    if(Animated == true)
    {
        States[CurrentState].CURRENT_FRAME = (ticks / AnimationSpeed) % States[CurrentState].TOTAL_FRAMES;
    }

    SDL_Rect srcrect = States[CurrentState].FRAMES[States[CurrentState].CURRENT_FRAME]; //{ frames * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT };
    SDL_Rect dstrect = {pos.x - (Size.x *.5), pos.y - (Size.y * .5), Size.x, Size.y};

    SDL_Point Center = {Size.x / 2,Size.y/2};
    SDL_RenderCopyEx(SCREEN->Renderer, SpriteSheet->ImageTexture ,  &srcrect, &dstrect, Angle, &Center,SDL_RendererFlip(0));

}
AABB* Sprite::MakeCollisionBox()
{
    //IDK WTF TO DO WITH THIS AT THE MOMENT
    //SDL_Rect dstrect = {Position.x - (Size.x *.5), Position.y - (Size.y * .5), Size.x, Size.y}
    return new AABB(Position, ((Size * .8f) *.5f)) ;
}


//==================================================================================================================================
//_______________________________________  State Class ______________________________________________________________________

//                                                                                                                                  
//   Each State is a state that an Entity can be in, Dead alive etc and is a group of frames that represents a state of the StaticSprite
//==================================================================================================================================


State::State(SDL_Rect srcrect,uint numframes)
     :TOTAL_FRAMES(numframes)
{
        CURRENT_FRAME = 0;
    	FRAMES.reserve(numframes);
    	uint x = srcrect.x;
    	for_loop(Count, numframes)
        {
    		    SDL_Rect frame;
    		    frame.x = x;
    		    frame.y = srcrect.y;
    		    frame.w = srcrect.w;
    		    frame.h = srcrect.h;
			   *this += frame; //FIXED Remember to get back to fixing the Overloaded Operator so that you can += new frames and states into your frame list.
    		    x += srcrect.w;
    	}
}
std::vector<SDL_Rect> &State::ADD_FRAME(SDL_Rect frame)
{
		FRAMES.emplace_back(frame);
	return FRAMES;
} // Adds a frame to the State

//==================================================================================================================================
//_______________________________________  Image Class ______________________________________________________________________

//                                                                                                                                  
//                                                                                                                                  
//==================================================================================================================================

Image::~Image()
{
    if(ID > 0)
    {
        Manager.Remove(ID);
    }
}
Image::Image(char *name,  const char *file)
{
     Name = name;
     ImageSurface = IMG_Load(file);
     if(!ImageSurface)
     {
             Print("ImageClass: Image Can not be loaded");Print(file);
        // *this = ErrorImage; // Set it to the Default Error image so that game does not crash during Beta Testing if a file does not exist        
        system("Pause");
             exit(_ERROR_);
             return;
     }
     
     ImageTexture = SDL_CreateTextureFromSurface(SCREEN->Renderer, ImageSurface);

     if(!ImageTexture)
     {
             Print("ImageClass: Texture Can not be made.");Print(file);
        exit(_ERROR_);
        system("Pause");
        //    *this = ErrorImage; // Set it to the Default Error image if Texture could not be made
         return;
     }

     Size.x = ImageSurface->w;
     Size.y = ImageSurface->h;
     ID = Manager.Add(this);
}
Image::Image(SDL_Surface *surf, SDL_Rect area)
{
 //   ID = Manager.Count++;
    ImageSurface = SDL_CreateRGBSurface(            NULL, 
                                          area.w, area.h,
                              surf->format->BitsPerPixel,
                                     surf->format->Rmask, 
                                     surf->format->Gmask,
                                     surf->format->Bmask, 
                                     surf->format->Amask);

    SDL_Rect dest = {0, 0, area.w, area.h};
    SDL_BlitSurface(surf, &area, ImageSurface, &dest);

    ImageTexture = SDL_CreateTextureFromSurface(SCREEN->Renderer, ImageSurface);
    if(!ImageTexture)
    {
            Print("ImageClass: Texture Can not be made from existing Surface."); 
        *this = *ErrorImage; // Set it to the Default Error image if Texture could not be made
        return;
    }
   // Images.push_back(std::shared_ptr<Image>(this));
 //   List.push_back((this));
    Manager.Add(this);
}
void Image::Render(Vec2 position)
{

    SDL_Rect srcrect = {0, 0, Size.x, Size.y}; 
    SDL_Rect dstrect = {position.x, position.y , position.x + Size.x, position.y + Size.y};
    SDL_RenderCopy(SCREEN->Renderer, ImageTexture, &srcrect, &dstrect);
   // SDL_RenderCopyEx(SCREEN->Renderer, SOURCE->SOURCE, &srcrect, &dstrect, Angle, NULL, SDL_FLIP_NONE);
}
void Image::Render(Vec2 position, Vec2 size)
{
    SDL_Rect srcrect = {0, 0, size.x, size.y}; 
    SDL_Rect dstrect = {position.x, position.y ,  size.x,size.y};
    SDL_RenderCopy(SCREEN->Renderer, ImageTexture, &srcrect, &dstrect);
}
 

//==================================================================================================================================
//____________________________________  SoundEffects Class ______________________________________________________________________

//                                                                                                                                  
//                                                                                                                                  
//==================================================================================================================================

SoundEffect::~SoundEffect()
{
   // Object = NULL;
   // Manager.Remove(ID);
   // Mix_FreeChunk(Object);
}
SoundEffect::SoundEffect(char *name, const char *file, int volume)
{
    Name = name;
    Volume = volume;
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT,2, 4096 ) == -1 )
    {
        Print("Failed to initialize Sound Effect" << Mix_GetError() );  
      return;
    }
    else
    {
        Object = Mix_LoadWAV(file);
    }

    ID = Manager.Add(this);
}
void SoundEffect::Play()
{
    Mix_PlayChannel(-1, Object, 0);
}
void SoundEffect::SetVolume(int vol)
{
    Volume = vol;
}


//==================================================================================================================================
//____________________________________  Music Class ______________________________________________________________________

//                                                                                                                                  
//                                                                                                                                  
//==================================================================================================================================

Music::~Music()
{
    Object = NULL;
    if(ID > 0)
    {
        Manager.Remove(ID);
    }
}
Music::Music(char *name, const char *file, int volume) 
{
    Name = name;
    Volume = volume;
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT,2, 4096 ) == -1 )
    {
        Print("Failed to initialize Music" << Mix_GetError() );  
      return;
    }
    else
    {
        Object = Mix_LoadMUS(file);
        Mix_VolumeMusic(2000);
    }
    Stereo = true;

ID = Manager.Add(this);
}
void Music::Play()
{
        Mix_PlayMusic(Object, -1 );
}
void Music::SetVolume(int vol)
{
        Volume = vol;
        Mix_VolumeMusic(Volume);
}
void Music::Delete()
{
    if(Object)
    {
//         Mix_FreeMusic(Object);
//          delete(Object);
    }
}

//==================================================================================================================================
//____________________________________  Tile Class ______________________________________________________________________


Tile::~Tile(){}
Tile::Tile(Image &sheet, Vec2 size)
{
  SpriteSheet = sheet;
}



 //   Vec2 Size;
//    std::vector<Sprite> Tile::TileList;

//==================================================================================================================================
//                                                                                                                                  
//                                                                                                                                  
//==================================================================================================================================



void SpriteInitialize()
{

//======================---~~~  Players Ship  ~~~---=================================
 
     Sprite *ShipSprite = new Sprite("Ship", Image::Manager.GetAsset("Ship"),4) ;
     ShipSprite->Size = Vec2(42);
     ShipSprite->States[0] = State(MAKE_Rect(0, 0,42, 42), 4);
     ShipSprite->States[1] = State(MAKE_Rect(0, 42,42, 42), 4);
     ShipSprite->AnimationSpeed = 100;
//   
//======================---~~~  Standard Bullet  ~~~---===============================
             
     Sprite *Bullet = new Sprite("Bullet",  Image::Manager.GetAsset("Bullet"), 1); 
        Bullet->States[0] = State(MAKE_Rect(0, 0,8, 8), 3);
        Bullet->Size = Vec2(8);

//=======================---~~~  Plasma Burst  ~~~---=================================
     Sprite *Plasma = new Sprite("PlasmaBurst", Image::Manager.GetAsset("PlasmaBurst"), 1);
        Plasma->States[0] = State(MAKE_Rect(0, 0,35,35), 42);
        Plasma->AnimationSpeed = 10;
        Plasma->Size = Vec2(35);

//======================---~~~  Ship Explosion  ~~~---================================
//   
  
 Sprite *Explosion = new Sprite("Explosion", Image::Manager.GetAsset("Explosion1"), 1);
          Explosion->Size = Vec2(64);
          Explosion->AnimationSpeed = 100;

          Explosion->States[0]  = State(MAKE_Rect(0,   0, 128, 128), 4);
          Explosion->States[0] += State(MAKE_Rect(0, 128, 128, 128), 4);
          Explosion->States[0] += State(MAKE_Rect(0, 256, 128, 128), 4);
          Explosion->States[0] += State(MAKE_Rect(0, 384, 128, 128), 4);
  
//   //=======================---~~~  EnergySphere  ~~~---=================================
         Sprite *EnergySphere = new Sprite("EnergySphere", Image::Manager.GetAsset("EnergySpheres"),1);
          EnergySphere->Size = Vec2(32);
          EnergySphere->AnimationSpeed = 50;
          EnergySphere->States[0]  = State(MAKE_Rect(0,       0,  84, 84), 7);
          EnergySphere->States[0] += State(MAKE_Rect(0,      84,  84, 84), 7);
          EnergySphere->States[0] += State(MAKE_Rect(0, (2 * 84), 84, 84), 7);
          EnergySphere->States[0] += State(MAKE_Rect(0, (2 * 84), 84, 84), 7 );
          EnergySphere->States[0] += State(MAKE_Rect(0, (2 * 84), 84, 84), 7);


//=========================---~~~  Cthulu Eye  ~~~---=================================
//   
Sprite*  CthuluEye = new Sprite("CthuluEye", Image::Manager.GetAsset("Cthulu"),1 );
       CthuluEye->Size = Vec2(32);
       CthuluEye->AnimationSpeed = 150;
       CthuluEye->States[0] = State(MAKE_Rect(0,0,32,32),2);
//           
//           
  Sprite*DragonSprite = new Sprite("DragonSprite", Image::Manager.GetAsset("Dragon"),4);
          DragonSprite->Size = Vec2(64);
          DragonSprite->AnimationSpeed = 150;

          DragonSprite->States[0] = State(MAKE_Rect(0,0,56,56),4);
          DragonSprite->States[1] = State(MAKE_Rect(0,56,56,56),4);
          DragonSprite->States[2] = State(MAKE_Rect(0,112,56,56),4);
          DragonSprite->States[3] = State(MAKE_Rect(0,168,56,56),4);
//           
Sprite* FireBall = new Sprite("FireBall", Image::Manager.GetAsset("FireBall"),1);
           FireBall->Size = Vec2(16);
           FireBall->AnimationSpeed = 50;
           FireBall->States[0] = State(MAKE_Rect(0,0,16,16),3);
//           
Sprite*GreenEye = new Sprite("GreenEye", new Image("Tenticles","Assets\\Tenticles.bmp"),3);
           GreenEye->Size = Vec2(100);
           GreenEye->AnimationSpeed = 100;
           GreenEye->States[0] = State(MAKE_Rect(0,113,88,112),6);
           GreenEye->States[1] = State(MAKE_Rect(0,0,88,112),6); 
           GreenEye->States[2] = State(MAKE_Rect(264,0,88,112),3);
           GreenEye->CurrentState = 0;
           
Sprite* FireOrb = new Sprite("FireOrb", new Image("FireOrb","Assets\\FireOrb.bmp"), 2);
           FireOrb->Size = Vec2(42);
           FireOrb->AnimationSpeed = 30;
           FireOrb->States[0] = State(MAKE_Rect(0,0,47,47),5);
           FireOrb->States[1] = State(MAKE_Rect(0,47,47,47),5); 
//   
Sprite  *Lightning01 = new Sprite("Lightning01", new Image("Lightning","Assets\\Lightning1.bmp"), 1);
           Lightning01->Size = Vec2(90);
           Lightning01->AnimationSpeed = 50;
           Lightning01->States[0] = State(MAKE_Rect(0,0,90,90),4);
//   //64x70
Sprite* CthuluDeath = new Sprite("CthuluDeath",  new Image("RedExplosion","Assets\\RedExplosion.bmp"),1);
           CthuluDeath->Size = Vec2(64);
           CthuluDeath->AnimationSpeed = 100;
           CthuluDeath->States[0]  = State(MAKE_Rect(0,0,64,70),4);
           CthuluDeath->States[0] += State(MAKE_Rect(0,70,64,70),4);
           CthuluDeath->States[0] += State(MAKE_Rect(0,140,64,70),4);
           CthuluDeath->States[0] += State(MAKE_Rect(0,210,64,70),4);
   
Sprite*BigGun = new Sprite("BigGun", new Image("BigGun","Assets\\BigGun.bmp"),1);
           BigGun->Size = Vec2(64);//76x84
           BigGun->Animated = false;
           BigGun->AnimationSpeed = 0;
           BigGun->States[0] = State(MAKE_Rect(0,0,76,84),1);
   
Sprite*BossDragon = new Sprite("BossDragon", new Image("BlueDragon","Assets\\BlueDragon.BMP"),4);
           BossDragon->States[0] = State(MAKE_Rect(0,0,96,96), 4);
           BossDragon->Size = Vec2(128);
           BossDragon->States[1] = State(MAKE_Rect(0,96,96,96), 4);
           BossDragon->States[2] = State(MAKE_Rect(0,96*2,96,96), 4);
           BossDragon->States[3] = State(MAKE_Rect(0,96*3,96,96), 4);
   
           float A = 20;
Sprite*  HealthPowerup = new Sprite("HealthPowerup", new Image("HealthPowerup","Assets\\HealthPowerup.bmp"),1);
           HealthPowerup->Size = Vec2(34);
           HealthPowerup->AnimationSpeed = 50;
           HealthPowerup->States[0] =  State(MAKE_Rect(0,0,A,A), 4);
           HealthPowerup->States[0] += State(MAKE_Rect(0,A+1,A,A), 4);
           HealthPowerup->States[0] += State(MAKE_Rect(0,(A+1)*2,A,A), 4);
           HealthPowerup->States[0] += State(MAKE_Rect(0,(A+1)*3,A,A), 4);
   
//   
//   
Sprite*   ExtraLife =new Sprite("ExtraLife", new Image("ExtraLife","Assets\\ExtraLife.bmp"),1);
          ExtraLife->Size = Vec2(34);
          ExtraLife->Animated = false;
          ExtraLife->AnimationSpeed = 0;
          ExtraLife->States[0] =  State(MAKE_Rect(0,0,160,160), 1);
  
  
Sprite*  GreenFireBall = new Sprite("GreenFireBall", new Image("GreenFireball","Assets\\GreenFireBall.bmp"),1);
          GreenFireBall->Size = Vec2(64);
          GreenFireBall->AnimationSpeed = 50;
          GreenFireBall->States[0]  = State(MAKE_Rect(0,   0, 64, 64), 3);
          GreenFireBall->States[0] += State(MAKE_Rect(0,  64, 64, 64), 3);
          GreenFireBall->States[0] += State(MAKE_Rect(0, 128, 64, 64), 3);
  
//   }
//   void Sprite::Unload()
//   {
//       delete(ShipSprite);
//       delete(Explosion);
//       delete(DragonSprite);
//       delete(CthuluEye);
//       delete(EnergySphere);
//       delete(Plasma);
//       delete(FireBall);
//       delete(Bullet);
//       delete(GreenEye);
//       delete(FireOrb);
//   }
}
void ImageInitialize()
{
     ErrorImage = new Image("ErrorDefault", "Assets\\ErrorDefault.bmp");
#if USING_PACKED_SPRITES

    Image Sheet = Image("Assets\\PackedSheet.bmp");

        Ship          = new Image("Assets\\Ship.bmp");
        EnergySpheres = new Image("Assets\\EnergySpheres.bmp");
        Bullet        = new Image("Assets\\Bullet.bmp");
        PlasmaBurst   = new Image("Assets\\PlasmaBurst.bmp");
        Explosion1    = new Image("Assets\\Explosion.bmp");// 128x128'
        Dragon        = new Image(Sheet.ImageSurface, MAKE_Rect(0,0,224,224));//new Image("Assets\\Dragon1.bmp"); 224x224
        Cthulu        = new Image(Sheet.ImageSurface, MAKE_Rect(0,226,64,32));
        FireBall      = new Image("Assets\\Fireball.bmp");
#else   
   //     Image *Eye           = new Image("Eye",           ("Assets\\Eye.bmp"));
        Image *Ship          = new Image("Ship",          ("Assets\\Ship.bmp"));
        Image *EnergySpheres = new Image("EnergySpheres", ("Assets\\EnergySpheres.bmp"));
        Image *Bullet        = new Image("Bullet",        ("Assets\\Bullet.bmp"));
        Image *PlasmaBurst   = new Image("PlasmaBurst",   ("Assets\\PlasmaBurst.bmp"));
        Image *Explosion1    = new Image("Explosion1",    ("Assets\\Explosion.bmp"));// 128x128'
        Image *Dragon        = new Image("Dragon",        ("Assets\\Dragon1.bmp"));
        Image *Cthulu        = new Image("Cthulu",        ("Assets\\Cthulu.bmp"));
        Image *FireBall      = new Image("FireBall",      ("Assets\\Fireball.bmp"));
#endif
}
void ImageUnload()
{
  //  delete(Eye           );
  //  delete(Ship          );
  //  delete(EnergySpheres );
  //  delete(Bullet        );
  //  delete(PlasmaBurst   );
  //  delete(Explosion1    );
  //  delete(Dragon        );
  //  delete(Cthulu        );
  //  delete(FireBall      );
}
void InitializeSound()
{
     SoundEffect  *LaserSound =    new  SoundEffect ("LaserSound","Assets\\Laser.wav", 100)        ;
     SoundEffect  *PlayerHit  =    new  SoundEffect ("PlayerHit", "assets\\PlayerHit.wav", 100)   ;
     SoundEffect  *Boom1 =         new  SoundEffect ("Boom1", "assets\\Boom1.wav", 100)        ;
     SoundEffect  *Boom2 =         new  SoundEffect ("Boom2",  "assets\\Boom2.wav", 100)        ;
     SoundEffect  *Boom3 =         new  SoundEffect ("Boom3", "assets\\Boom3.wav", 100)        ;
     SoundEffect  *Boom5 =         new  SoundEffect ("Boom5",  "assets\\Boom5.wav", 100)       ;
     SoundEffect  *Boom8 =         new  SoundEffect ("Boom8",  "assets\\Boom8.wav", 100)       ;
     SoundEffect  *LaserHead0006 = new  SoundEffect ("LaserHead0006", "Assets\\Head0006.wav", 100)     ;
     SoundEffect  *LaserTail0004 = new  SoundEffect ("LaserTail0004",  "Assets\\Tail0004.wav", 100)     ;
     SoundEffect  *LaserTail0005 = new  SoundEffect ("LaserTail0005",  "Assets\\Tail0005.wav", 100)     ;
     SoundEffect  *Beep01 =        new  SoundEffect ("Beep01",    "Assets\\Click.wav", 100)        ;
     SoundEffect  *GunFire =       new  SoundEffect ("GunFire",  "Assets\\antimatterhit.wav",100) ;
     SoundEffect  *Extralife =     new  SoundEffect ("Extralife", "assets\\ExtraLife.wav",100)     ;
}
void InitializeMusic()
{
   Music *BackgroundMusic  = new Music("BackGround","Assets\\Music.wav", 1000);
   Music *BackgroundMusic2 = new Music("Intro","Assets\\Mayhem.mp3", 1000);
}
SDL_Rect  MAKE_Rect(int x,int y, int w, int h)
{
	SDL_Rect ret;
		ret.x=x;
		ret.y=y;
		ret.h=h;
		ret.w=w;
return ret;
}
#endif