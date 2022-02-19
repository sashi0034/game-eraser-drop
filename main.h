#pragma once

#define _USE_MATH_DEFINES
#include "DxLib.h"
#include "lua.hpp"

#include <random>
#include <any>
#include <map>
#include <math.h>

#include "sprite.h"
#include "game_utils.h"

#define DEBUG

#ifdef DEBUG

#endif // DEBUG



using namespace gameUtils;







namespace useful
{
    class Random
    {
        std::random_device device;
        std::default_random_engine engine;
        std::uniform_int_distribution<int> distribution;

    public:
        Random(void);
        Random(int seed);
        int Get(int max);
        int Get(int min, int max);
    };
}





namespace game
{
    namespace resorce 
    {
        class Image
        {
        public:
            Graph Test = Graph::LoadGraph(R"(.\asset\image\cloud_128x64.png)");
            Graph Chicken = Graph::LoadGraph(R"(.\asset\image\chicken_32x32.png)");
            Graph Tile32 = Graph::LoadGraph(R"(.\asset\image\magma_tile_black_32x32.png)");

#if 0
            int Templa = LoadGraph(R"(.png)");
#endif // 0

            Image()
            {
            }
        };

    }


    namespace lua
    {
        void DefineSpriteFunc();
        
        class LuaDebugManager
        {
            bool mHasLastWriteTime = false;
            SYSTEMTIME mLastWriteTime;

        public:
            LuaDebugManager();

            Sprite* Spr;
            int Count = 0;

            void Update();

            static void CallUpdate(Sprite* hSpr);
            static void Calldestructor(Sprite* hSpr);
        };

    }

}


namespace game{
    
    const int ROUGH_WIDTH = 416, ROUGH_HEIGHT = 240;
    const int ROUGH_SCALE = 3;
    const int FULL_WIDTH = ROUGH_WIDTH * ROUGH_SCALE;
    const int FULL_HEIGHT = ROUGH_HEIGHT * ROUGH_SCALE;

    useful::Random* Rand = new useful::Random();

    int GameState;
    bool CanRestartProgram = false;

    resorce::Image* Img;
    lua_State* Lua;


    int Process();
    int SceneTransition();

    void LoopBasicUpdate();



    namespace main 
    {
        class Scene
        {
        public:
            Scene();
            void Loop();
        };





        class Test
        {
        public:
            static Test* GetIn;

            Sprite* Spr;
            Sprite* OtherSp;

            Test();
            void Update();

            static void CallUpdate(Sprite* hSpr);
            static void Calldestructor(Sprite* hSpr);
        };



 



        class BackGround
        {
        public:
            static BackGround* GetIn;

            Sprite* Spr;
            int Image;

            BackGround();
            void Update();

            static void CallUpdate(Sprite* hSpr);
            static void Calldestructor(Sprite* hSpr);
        };




#if 0

        // テンプレ
        class Templa
        {
        public:
            Sprite* Spr;
            int Image;
            double X = 0, Y = 0;
            int Count = 0;

            Templa();
            void Update();

            static void CallUpdate(Sprite* hSpr);
            static void Calldestructor(Sprite* hSpr);
        };

        // テンプレ(手動描画)
        class Templa
        {
        public:
            Sprite* Spr;
            int Count = 0;

            Templa();
            void Update();
            void Drawing(int hX, int hY);

            static void CallUpdate(Sprite* hSpr);
            static void CallDrawing(int hSp, int hX, int hY);
            static void Calldestructor(Sprite* hSpr);
        };

        // コライダー付きテンプレ
        class Templa : public hit::Collider
        {
        public:
            Sprite* Spr;
            int Image;
            // X, Yはいらない
            int Count = 0;

            Templa();
            void Update();

            static void CallUpdate(Sprite* hSpr);
            static void Calldestructor(Sprite* hSpr);
        };


#endif // 0


    }
}


