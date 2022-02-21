#pragma once
#include "sol.hpp"
#include <any>
#include "DxLib.h"
#include "sprite.h"


namespace luas
{
    extern sol::state Sol;
    extern bool CanRestartProgram;

    int SolStart();
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
