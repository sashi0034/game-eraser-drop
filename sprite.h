#pragma once
#include <utility>
#include <any>
#include <vector>
#include "DxLib.h"
#include "game_utils.h"

using namespace gameUtils;

class Sprite
{
    static const int ROUGH_SCALE = 3;

    static std::vector<Sprite*> sprites;

    bool isUsed = false;
    double x = 0;
    double y = 0;
    double z = 0;
    Graph image = Graph::NONE;
    int u = 0;
    int v = 0;
    int width = 0;
    int height = 0;
    bool isFlip = false;
    bool isProtect = false;
    const Sprite* linkXY = nullptr;
    double rotationRad = 0;
    int blendMode = DX_BLENDMODE_ALPHA;
    int blendPal = 255;
    std::any belong = nullptr;

    void (*updateMethod)(Sprite* hSP) = nullptr;
    void (*destructorMethod)(Sprite* hSp) = nullptr;
    void (*drawingMethod)(Sprite* hSp, int hX, int hY);

    static int findIndex(Sprite* spr);
    static void garbageCollect();
public:
    Sprite();
    Sprite(Graph image);
    Sprite(Graph image, int u, int v, int w, int h);

    static void Init();
    static void End();

    void SetFlip(bool isFlip);

    void SetImage(Graph image);
    void SetImage(int u, int v);
    void SetImage(int u, int v, int width, int height);
    void SetImage(Graph image, int u, int v, int width, int height);

    void SetXY(double x, double y);
    void SetZ(double z);
    double GetZ();


    void GetScreenXY(int* x, int* y);

    void SetRotationDeg(double deg);
    void SetRotationRad(double rad);

    void SetBelong(std::any instance);
    std::any GetBelong();

    void SetLinkXY(const Sprite* linkSpr);

    void GetLinkDifferenceXY(double* x, double* y);

    void SetBlend(int blendMode, int blendPal);
    void SetBlendMode(int blendMode);
    void SetBlendPal(int blendPal);

    void SetUpdateMethod(void (*updateMethod)(Sprite* hSp));
    void SetDrawingMethod(void (*drawingMethod)(Sprite* hSp, int hX, int hY));
    void SetDestructorMethod(void (*destructorMethod)(Sprite* hSp));

    void SetProtect(bool isProtect);

    static void Dispose(Sprite* spr);
    static void Dispose(Sprite* spr, bool isProtectOnly);

    static void DisposeAll();
    static void DisposeAll(bool isProtectOnly);


    static void UpdateAll();
    static void DrawingAll();


    class DrawingProcess
    {
    public:
        static void Rough(Sprite* hSpr, int hX, int hY);
        static void Twice(Sprite* hSpr, int hX, int hY);
        static void DotByDot(Sprite* hSpr, int hX, int hY);
        static void Draw(Sprite* hSpr, int x, int y, int scale);
    };


};

class sprite
{
};

