#pragma once
#include <utility>
#include <DxLib.h>
#include <any>


class Sprite
{
    static const int ROUGH_SCALE = 3;
    static const int SPRITE_MAX = 4096;
    static const int ANIM_MAX = 8;
    static int anim1step[];
    static int nextIndex;

    static Sprite* sprites[];
    static std::pair<int, short> sprites_Z[];


    bool isUsed = false;
    double x = 0;
    double y = 0;
    int image = -1;
    int u = 0;
    int v = 0;
    int width = 0;
    int height = 0;
    bool isReverse = false;
    bool isProtect = false;
    int link = -1;
    double rotationRad = 0;

    int blendMode = DX_BLENDMODE_ALPHA;
    int blendPal = 255;


    std::any belong = nullptr;

    void (*updateMethod)(int hSP) = nullptr;
    void (*destructerMethod)(int hSp) = nullptr;
    void (*drawingMethod)(int hSp, int hX, int hY);


    Sprite();


public:

    static const int ANIMTYPE_XY = 1;
    static const int ANIMTYPE_UV = 2;
    static const int ANIMTYPE_BLENDMODE = 3;
    static const int ANIMTYPE_BLENDPAL = 4;
    static const int ANIMTYPE_ROTATIONDEG = 5;

    static void Init();
    static void End();

    static int Make();
    static int Make(int image);
    static int Make(int image, int u, int v, int width, int height);

    static void Reverse(int sp, bool isReverse);

    static void Image(int sp, int image);
    static void Image(int sp, int u, int v);
    static void Image(int sp, int u, int v, int width, int height);
    static void Image(int sp, int image, int u, int v, int width, int height);

    static void Offset(int sp, double x, double y);
    static void Offset(int sp, double x, double y, short z);
    static void Offset(int sp, short z);


    static void GetScreenXY(int sp, int* x, int* y);

    static void RotationDeg(int sp, double deg);
    static void RotationRad(int sp, double rad);


    static void Belong(int sp, std::any instance);
    static std::any GetBelong(int sp);

    static void Link(int sp, int link);

    static double GetLinkDifference_X(int sp);
    static double GetLinkDifference_Y(int sp);

    static void Blend(int sp, int blendMode, int blendPal);
    static void BlendMode(int sp, int blendMode);
    static void BlendPal(int sp, int blendPal);

    static void Update(int sp, void (*updateMethod)(int hSp));
    static void Drawing(int sp, void (*drawingMethod)(int hSp, int hX, int hY));
    static void Destructer(int sp, void (*destructerMethod)(int hSp));

    static void Clear(int sp);
    static void Clear(int sp, bool protectOnly);

    static void AllClear();
    static void AllClear(bool protectOnly);

    static void Protect(int sp, bool isProtect);

    static void AllUpdate();
    static void AllDrawing();

    static double GetUsingRate();


    class DrawingProcess
    {
        static int a;
    public:
        static void Rough(int hSp, int hX, int hY);
        static void Twice(int hSp, int hX, int hY);
        static void DotByDot(int hSp, int hX, int hY);
        static void Draw(int sp, int x, int y, int scale);
    };


};

class sprite
{
};

