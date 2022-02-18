#pragma once

#define _USE_MATH_DEFINES
#include "DxLib.h"
#include "lua.hpp"

#include <random>
#include <any>
#include <map>
#include <math.h>

#include "sprite.h"

#define DEBUG

#ifdef DEBUG

#endif // DEBUG











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
            int Test = LoadGraph(R"(.\asset\image\cloud_128x64.png)");
            int Chicken = LoadGraph(R"(.\asset\image\chicken_32x32.png)");
            int Tile32 = LoadGraph(R"(.\asset\image\magma_tile_black_32x32.png)");

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

            int Sp;
            int Count = 0;

            void Update();

            static void CallUpdate(int hSp);
            static void CallDestructer(int hSp);
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

            int Sp, OtherSp;

            Test();
            void Update();

            static void CallUpdate(int hSp);
            static void CallDestructer(int hSp);
        };



 



        class BackGround
        {
        public:
            static BackGround* GetIn;

            int Sp;
            int Image;

            BackGround();
            void Update();

            static void CallUpdate(int hSp);
            static void CallDestructer(int hSp);
        };




#if 0

        // テンプレ
        class Templa
        {
        public:
            int Sp;
            int Image;
            double X = 0, Y = 0;
            int Count = 0;

            Templa();
            void Update();

            static void CallUpdate(int hSp);
            static void CallDestructer(int hSp);
        };

        // テンプレ(手動描画)
        class Templa
        {
        public:
            int Sp;
            int Count = 0;

            Templa();
            void Update();
            void Drawing(int hX, int hY);

            static void CallUpdate(int hSp);
            static void CallDrawing(int hSp, int hX, int hY);
            static void CallDestructer(int hSp);
        };

        // コライダー付きテンプレ
        class Templa : public hit::Collider
        {
        public:
            int Sp;
            int Image;
            // X, Yはいらない
            int Count = 0;

            Templa();
            void Update();

            static void CallUpdate(int hSp);
            static void CallDestructer(int hSp);
        };


#endif // 0


    }
}


