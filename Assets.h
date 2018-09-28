#pragma once
#define USING_ASSETS   1


#if USING_ASSETS
#include"window.h"
#include <SDL_IMAGE.h>
#include "Collision.h"
#ifdef _SDL_
        typedef SDL_Surface  Surface;
        typedef SDL_Texture  Texture;
        typedef unsigned int uint;
#endif

template<typename T>


struct AssetManager
{



      T *GetAsset(char *name)
        {
            for_loop(Index, Count)
            {
                if(List[Index]->Name == name)
                {
                    return List[Index];
                }
            }
           Print("Asset " << name << " Not Registered: Please check the description of ( " << name << " ) ");
           system("Pause");
           exit(_ERROR_);
        }
      T *GetAsset(int id)
        {
            return List[id];
        }


        int  Add(T *object)
        {
            ID = Count++; 
            List.push_back(object);
            return ID;
        }
        void Delete(char *name)
        {
            Delete(GetAsset(name)->ID);
        }
        void Delete(int id)
        {
            delete(List[id]);
            Remove(id);
            //List.erase(List.begin() + id);
            //for(int Itr = id; Itr < List.size(); Itr++)
            //{
            //    List[Itr]->ID--;
            //}
        }
        void Remove(int id)
        {
            List.erase(List.begin() + (id));
            for(int Itr = id; Itr < List.size(); Itr++)
            {
                List[Itr]->ID--;
            }
        }
        void FreeAll()
        {
           for_loop(Index, List.size())
           {
               delete(List[Index]);
           }
        }
        bool IsExistant(char *name)
        {
            for(auto &L: List)
            {
                if(L.Name == name) return true;
            }
          return false;
        }

private:
        std::vector<T *> List;
        int ID;
        int Count;
};



class Image
{
public:
       ~Image();


        Image()
        {
            Size = Vec2(0);
        }

        Image(const Image& other)
        {
            *this = other;
            this->ID = 0;
        }

        Image(char *name, const char *file);
        Image(SDL_Surface *surf, SDL_Rect area);

        char *Name;
        int ID;
        Vec2 Size;
        
        Texture *ImageTexture;
        Surface *ImageSurface;
        
        void Render(Vec2 position);
        void Render(Vec2 position, Vec2 size);
        
    static AssetManager<Image> Manager;
};




class State
{
public:
	    State(){}
	   ~State(){}

	    State(SDL_Rect sourcerect,uint numframes);
        State(SDL_Rect sourcerect,Vec2 size, uint numframes);
	    State(uint x, uint y,uint w,uint h,uint numframes)
        {
            SDL_Rect src = {x,y,w,h};
            *this = State(src,numframes);
        }

	    std::vector<SDL_Rect>FRAMES;
	    uint TOTAL_FRAMES;
	    uint CURRENT_FRAME;

        State operator +=(SDL_Rect newframe)
        { 
            this->AddFrame(newframe);  
            return *this;
        }
        State operator +=(State newstate)
        {
            for_loop(count, newstate.TOTAL_FRAMES)
            {
                this->ADD_FRAME(newstate.FRAMES[count]); 
                this->TOTAL_FRAMES++;
            }
            return *this;
        }
 
    static State* MakeStates(SDL_Rect sourcerect, uint numframes, uint numstates)
    { // WARNING: This function is currently incomplete and not working
        Print("You have not properly figured out how to return the Array of States through a pointer");
        Print("Please remove reference to State* MakeStates() Method in the State Class");
        State *ret = new State[numstates];
        uint Ypos  = sourcerect.y;
        uint Ystep = sourcerect.h;
        for_loop(Index, numstates)
        {
            ret[Index] = State(sourcerect.x, Ypos, sourcerect.w,sourcerect.h, numframes);
            Ypos += Ystep;
        }
        return ret;
    }

private:
		std::vector<SDL_Rect>& AddFrame(SDL_Rect frame);
};

class Sprite
{
public:
	    Sprite(){}
       ~Sprite();

	    Sprite(char *name, Image *source, int numstates);
        Sprite(const Sprite& other)
        {
            *this = other;
            this->ID = 0;
        }

	    Sprite(Image *source){*this = Sprite(source, 1);} //	 std::Bind would be useful here
        char *Name;
        int ID;

        Vec2 Position;
        Vec2 Size;
	    
	    bool Animated;
	    
        float Angle;

	    Image *SpriteSheet;
	    
        uint TotalStates;
        std::vector<State> States;

	    uint CurrentState;
        int  AnimationSpeed;
  
        bool Collidable, 
             Static;
public:

        inline void SetPosition(Vec2 pos){ Position = pos;}

        void  Update();
        void  Render();
        void  Render(float angle);
        void  Render(Vec2 pos);
        void  Render(Vec2 pos, float angle);

        AABB* MakeCollisionBox();

//------------------------------------------------------------------------------------------------------------------

    static AssetManager<Sprite> Manager;
//------------------------------------------------------------------------------------------------------------------
};

class Tile
{
    ~Tile();
    Tile(){}
    Tile(Image &spritesheet, Vec2 size);


    Image SpriteSheet;

    Vec2 Size;
    std::vector<Sprite> TileList;
};

#include"SDL_Mixer.h"

class SoundEffect 
{
public:
        SoundEffect(){}
        SoundEffect(const SoundEffect& other)
        {
            *this = other;
            this->ID = 0;
        }       
       ~SoundEffect();
        SoundEffect(char *name, const char *file, int volume);
        
        char *Name;
        
        int ID;
        int Volume;
        
        Mix_Chunk *Object;
        
        void Play();
        void SetVolume(int vol);
//---------------------------------------------------------------------------------------------------------------------
    static AssetManager<SoundEffect> Manager;
//------------------------------------------------------------------------------------------------------------------
};

class Music 
{
public:
        Music(){}
        Music(const Music& other)
        {
            *this = other;
            this->ID = 0;
        }
       ~Music();
        Music(char *name, const char *file, int volume) ;
 
        void Play();
        void SetVolume(int vol);

        void Delete();

        char *Name;
        int ID;
        Mix_Music *Object;
        bool Stereo;
        int Volume;


static AssetManager<Music> Manager;
};





void SpriteInitialize();
void InitializeSound();
void InitializeMusic();
void ImageInitialize();


void ImageUnload();


SDL_Rect  MAKE_Rect(int x,int y, int w, int h);


extern const Image *ErrorImage;
#endif














