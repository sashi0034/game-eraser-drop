#include "main.h"
#include <iostream>

#define LOOP    (DxLib::ProcessMessage() != -1 && (!CanRestartProgram))





int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    {//�f�o�b�O�p�R���\�[���E�C���h�E�̕\��
        AllocConsole();
        FILE* stream;
        freopen_s(&stream, "CONIN$", "r", stdin);
        freopen_s(&stream, "CONOUT$", "w", stdout);
    }

    return game::Process();
    
}









namespace game
{
    int Process()
    {
        // �E�C���h�E���[�h�ŋN��
        DxLib::ChangeWindowMode(TRUE);

        // �t���X�N���[�����[�h���̃��j�^�[�̉𑜓x���ő�ɂ��郂�[�h�ɐݒ�
        DxLib::SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_NATIVE);

        // �t���X�N���[�����[�h���̉�ʂ̊g�僂�[�h���ŋߓ_���[�h�ɐݒ�
        DxLib::SetFullScreenScalingMode(DX_FSSCALINGMODE_NEAREST);

        DxLib::SetGraphMode(FULL_WIDTH, FULL_HEIGHT, 16);

        DxLib::SetAlwaysRunFlag(1); //��A�N�e�B�u��Ԃł�������

        if (DxLib::DxLib_Init() == -1)        // DX���C�u��������������
        {
            return -1;        // �G���[���N�����璼���ɏI��
            
        }
        Sprite::Init();

        Img = new resorce::Image(); //�摜�ǂݍ���


    restart:
        
        if (lua::SolStart()==-1) return -1;

        printf("game is start\n");

        //�V�[��
        SceneTransition();

        if (CanRestartProgram)
        {// �ċN��
            CanRestartProgram = false;
            Sprite::DisposeAll();
            printfDx("�ċN������\n");
            goto restart;
        }

        printf("game is end\n");

        Sprite::End();
        DxLib::DxLib_End();        // �c�w���C�u�����g�p�̏I������


        return 0;
        // �\�t�g�̏I��
    }


    int SceneTransition()
    {
        //new test::Scene();
        while (LOOP)
        {
            //new top::Scene();
            new main::Scene();
        }

        return 0;
    }


    void LoopBasicUpdate()
    {
        Sprite::UpdateAll();
        DxLib::SetDrawScreen(DX_SCREEN_BACK);
        DxLib::ClearDrawScreen();
        Sprite::DrawingAll();
        DxLib::ScreenFlip();
    }
}


namespace game
{
    namespace lua
    {
        int SolStart()
        {
            // Lua�ǂݍ���
            /*Lua = luaL_newstate();
            luaL_openlibs(Lua);
            if (luaL_dofile(Lua, "resorce.lua"))
            {
                OutputDebugString("Lua�̓ǂݍ��݂Ɏ��s���܂���");
                return -1;
            }
            lua::DefineSpriteFunc();*/
            Sol.open_libraries(sol::lib::base, sol::lib::package);
            DefineSpriteFunc();

            new lua::LuaDebugManager();
            return 0;
        }
        void DefineSpriteFunc()
        {

            lua::Sol.new_usertype<Sprite>(
                "Sprite",
                sol::constructors<Sprite()>(),
                "SetXY", &Sprite::SetXY);

            Sol.script_file("test.lua");
        }
    }


    namespace lua
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
            HANDLE hFile = FindFirstFile(R"(C:\Users\satos\source\repos\lua_test\resorce.lua)", &findData); // ��΃p�X���w��

            if (hFile == INVALID_HANDLE_VALUE)
            {
                printf("�ǂݍ��ݎ��s\n");
            }
            else
            {
                FindClose(hFile);

                // FindFirstFile�ɂ�UTC�^�C���X�^���v���擾(100�i�m�b�P��)
                // FileTimeToLocalFileTime�ɂ�UTC���烍�[�J�����Ԃɕϊ�
                // FileTimeToSystemTime�ɂ�100�i�m�b�P�ʂ�N���������b�~���b�ɕϊ�
                // �X�V����
                SYSTEMTIME lastWriteTime;
                FileTimeToLocalFileTime(&findData.ftLastWriteTime, &fileTime);
                FileTimeToSystemTime(&fileTime, &lastWriteTime);
                
                if (!mHasLastWriteTime)
                {// ������
                    mLastWriteTime = lastWriteTime;
                    mHasLastWriteTime = true;
                }

                if ((mLastWriteTime.wMilliseconds != lastWriteTime.wMilliseconds)
                    || (mLastWriteTime.wSecond != lastWriteTime.wSecond))
                {//�@�X�V���Ă��Ȃ�
                    printf("�v���O�����̕ύX���m�F���܂����B�ċN�����܂�\n");
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
}














namespace game
{






    //���C���V�[��
    namespace main
    {
        Scene::Scene()
        {
            new Test();
            new BackGround();
            
            Loop();

            delete this;
        }
        void Scene::Loop()
        {

            while (LOOP)
            {
                LoopBasicUpdate();
            }
        }
    }






    // Test
    namespace main
    {
        Test* Test::Sole = nullptr;


        // �e�X�g
        Test::Test() : Actor()
        {
            Test::Sole = this;
            mSpr->SetImage(Img->Chicken, 0, 0, 32, 32);
            mSpr->SetXY(20, 20);
            mSpr->SetZ(-20);

            OtherSp = new Sprite(Img->Test, 0, 0, 128, 64);
            OtherSp->SetXY(100, 50);
            OtherSp->SetZ(-200);

            SolState = lua::Sol["Test"];
            std::string res = lua::Sol["Test"]["new"](SolState, mSpr);
            std::cout << res;
            
        }

        void Test::update()
        {
            std::string res = lua::Sol["Test"]["Update"](SolState);
            std::cout << res;

            
            Actor::update();
        }
    }





    // BackGround
    namespace main
    {
        BackGround* BackGround::Sole = nullptr;


        // �w�i
        BackGround::BackGround() : Actor()
        {
            Image = DxLib::MakeScreen(ROUGH_WIDTH, ROUGH_HEIGHT, TRUE);
            mSpr->SetImage(Graph(Image), 0, 0, ROUGH_WIDTH, ROUGH_HEIGHT);
            DxLib::SetDrawScreen(Image);
            
            for (int x = 0; x < ROUGH_WIDTH; x+=32)
            {
                for (int y = 0; y < ROUGH_HEIGHT; y+=32)
                {
                    DxLib::DrawGraph(x, y, Img->Tile32.getHandler(), TRUE);
                }
            }

            mSpr->SetZ(4000);
            BackGround::Sole = this;

        }
        void BackGround::update()
        {
            Actor::update();
        }

    }




#if 0
    // �e���v��
    namespace main
    {
        Templa::Templa()
        {
            Image = Img->Templa;

            Sp = Sprite::Make(Image, 0, 0, 16, 16);
            Sprite::Offset(Sp, X, Y);
            Sprite::Belong(Sp, this);
            Sprite::Update(Sp, Templa::CallUpdate);
            Sprite::destructor(Sp, Templa::Calldestructor);
        }

        void Templa::Update()
        {
            DxLib::printfDx("Count = %d\n", Count);
            Count++;
        }


        void Templa::CallUpdate(int hSp)
        {
            std::any_cast<Templa*>(Sprite::GetBelong(hSp))->Update();
        }
        void Templa::Calldestructor(int hSp)
        {
            delete std::any_cast<Templa*>(Sprite::GetBelong(hSp));
        }

    }

    // �e���v��(�蓮�`��)
    namespace main
    {
        Templa::Templa()
        {
            Sp = Sprite::Make();
            Sprite::Offset(Sp, -4000);
            Sprite::Belong(Sp, this);
            Sprite::Update(Sp, Templa::CallUpdate);
            Sprite::Drawing(Sp, Templa::CallDrawing);
            Sprite::destructor(Sp, Templa::Calldestructor);
        }

        void Templa::Update()
        {
            DxLib::printfDx("Count = %d\n", Count);
            Count++;
        }

        void Templa::Drawing(int hX, int hY)
        {

        }

        void Templa::CallUpdate(int hSp)
        {
            std::any_cast<Templa*>(Sprite::GetBelong(hSp))->Update();
        }
        void Templa::CallDrawing(int hSp, int hX, int hY)
        {
            std::any_cast<Templa*>(Sprite::GetBelong(hSp))->Drawing(hX, hY);
        }
        void Templa::Calldestructor(int hSp)
        {
            delete std::any_cast<Templa*>(Sprite::GetBelong(hSp));
        }

    }

    // �R���C�_�[�t���e���v��
    namespace main
    {
        Templa::Templa() : hit::Collider(new hit::Collider::Rectangle(0, 0, 16, 16), MASK_TEMPLA)
        {
            Image = Img->Templa;

            Sp = Sprite::Make(Image, 0, 0, 16, 16);
            Sprite::Offset(Sp, X, Y);
            Sprite::Belong(Sp, this);
            Sprite::Update(Sp, Templa::CallUpdate);
            Sprite::destructor(Sp, Templa::Calldestructor);
        }

        void Templa::Update()
        {
            DxLib::printfDx("Count = %d\n", Count);
            Count++;
        }


        void Templa::CallUpdate(int hSp)
        {
            std::any_cast<Templa*>(Sprite::GetBelong(hSp))->Update();
        }
        void Templa::Calldestructor(int hSp)
        {
            delete std::any_cast<Templa*>(Sprite::GetBelong(hSp));
        }

    }


#endif // 0





}


















































    