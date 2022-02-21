#include "luas.h"

namespace luas
{
    sol::state Sol;
    bool CanRestartProgram = false;

    int SolStart()
    {

        Sol.open_libraries(sol::lib::base, sol::lib::package);
        DefineSpriteFunc();

        new luas::LuaDebugManager();
        return 0;
    }
    void DefineSpriteFunc()
    {

        luas::Sol.new_usertype<Sprite>(
            "Sprite",
            sol::constructors<Sprite()>(),
            "setXY", &Sprite::SetXY);

        Sol.script_file("test.lua");
    }
}

namespace luas
{
    LuaDebugManager::LuaDebugManager()
    {
        Spr = new Sprite();
        Spr->SetBelong(this);
        Spr->SetUpdateMethod(LuaDebugManager::CallUpdate);
        Spr->SetDestructorMethod(LuaDebugManager::Calldestructor);

    }

    void LuaDebugManager::Update()
    {
        WIN32_FIND_DATA findData;
        FILETIME fileTime;
        HANDLE hFile = FindFirstFile(R"(C:\Users\satos\source\repos\lua_test\resorce.lua)", &findData); // 絶対パスを指定

        if (hFile == INVALID_HANDLE_VALUE)
        {
            printf("読み込み失敗\n");
        }
        else
        {
            FindClose(hFile);

            // FindFirstFileにてUTCタイムスタンプを取得(100ナノ秒単位)
            // FileTimeToLocalFileTimeにてUTCからローカル時間に変換
            // FileTimeToSystemTimeにて100ナノ秒単位を年月日時分秒ミリ秒に変換
            // 更新日時
            SYSTEMTIME lastWriteTime;
            FileTimeToLocalFileTime(&findData.ftLastWriteTime, &fileTime);
            FileTimeToSystemTime(&fileTime, &lastWriteTime);

            if (!mHasLastWriteTime)
            {// 初期化
                mLastWriteTime = lastWriteTime;
                mHasLastWriteTime = true;
            }

            if ((mLastWriteTime.wMilliseconds != lastWriteTime.wMilliseconds)
                || (mLastWriteTime.wSecond != lastWriteTime.wSecond))
            {//　更新してたなら
                printf("プログラムの変更を確認しました。再起動します\n");
                CanRestartProgram = true;
                mLastWriteTime = lastWriteTime;
            }


        }


    }


    void LuaDebugManager::CallUpdate(Sprite* hSpr)
    {
        std::any_cast<LuaDebugManager*>(hSpr->GetBelong())->Update();
    }
    void LuaDebugManager::Calldestructor(Sprite* hSpr)
    {
        delete std::any_cast<LuaDebugManager*>(hSpr->GetBelong());
    }

}
