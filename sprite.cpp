#define _USE_MATH_DEFINES

#include "sprite.h"
#include <math.h>
#include <random>



Sprite::Sprite()
{
    drawingMethod = Sprite::DrawingProcess::Rough;

}

int Sprite::nextIndex = 0;
int Sprite::anim1step[Sprite::ANIM_MAX];
Sprite* Sprite::sprites[Sprite::SPRITE_MAX];
std::pair<int, short> Sprite::sprites_Z[Sprite::SPRITE_MAX];


void Sprite::Init()
{
    for (int i = 0; i < SPRITE_MAX; i++)
    {
        Sprite::sprites[i] = new Sprite();
        Sprite::sprites_Z[i] = std::make_pair(i, 0);
    }

    nextIndex = 0;
    anim1step[ANIMTYPE_XY] = 3;
    anim1step[ANIMTYPE_UV] = 3;
    anim1step[ANIMTYPE_BLENDMODE] = 2;
    anim1step[ANIMTYPE_BLENDPAL] = 2;
    anim1step[ANIMTYPE_ROTATIONDEG] = 2;
}


void Sprite::End()
{

    Sprite::AllClear();
    for (int i = 0; i < SPRITE_MAX; i++)
    {
        delete sprites[i];
    }
}


int Sprite::Make()
{
    return Make(-1, 0, 0, 0, 0);
}
int Sprite::Make(int image)
{
    int x = 0, y = 0;
    DxLib::GetGraphSize(image, &x, &y);
    return Make(image, 0, 0, x, y);
}
int Sprite::Make(int image, int u, int v, int width, int height)
{
    for (int i = 0; i < SPRITE_MAX; i++)
    {
        int sp = (nextIndex + i) % SPRITE_MAX;

        if (!sprites[sp]->isUsed)
        {// 未使用なら割り当て
            delete sprites[sp];

            sprites[sp] = new Sprite();
            sprites_Z[sp] = std::make_pair(sp, 0);

            Sprite* spr = sprites[sp];
            spr->isUsed = true;
            spr->image = image; // image = -1 の場合は画像を表示しない
            spr->u = u;
            spr->v = v;
            spr->width = width;
            spr->height = height;

            nextIndex = sp + 1;

            return sp;
        }
    }
    return -1;
}

void Sprite::Reverse(int sp, bool isReverse)
{
    sprites[sp]->isReverse = isReverse;
}


void Sprite::Image(int sp, int image)
{
    Sprite* spr = sprites[sp];
    spr->image = image;
}
void Sprite::Image(int sp, int u, int v)
{
    Sprite* spr = sprites[sp];
    spr->u = u;
    spr->v = v;
}
void Sprite::Image(int sp, int u, int v, int width, int height)
{
    Image(sp, u, v);
    Sprite* spr = sprites[sp];
    spr->width = width;
    spr->height = height;
}
void Sprite::Image(int sp, int image, int u, int v, int width, int height)
{
    Image(sp, image);
    Image(sp, u, v, width, height);
}


void Sprite::Offset(int sp, double x, double y)
{
    Sprite* spr = sprites[sp];
    spr->x = x;
    spr->y = y;
}
void Sprite::Offset(int sp, double x, double y, short z)
{
    Offset(sp, x, y);
    Offset(sp, z);
}
void Sprite::Offset(int sp, short z)
{
    sprites_Z[sp] = std::make_pair(sp, z);
}


void Sprite::GetScreenXY(int sp, int* x, int* y)
{
    Sprite* spr = sprites[sp];
    *x = int(spr->x + GetLinkDifference_X(sp));
    *y = int(spr->y + GetLinkDifference_Y(sp));
}


void Sprite::RotationDeg(int sp, double deg)
{
    Sprite* spr = sprites[sp];
    spr->rotationRad = (deg / 180.0) * M_PI;
}
void Sprite::RotationRad(int sp, double rad)
{
    Sprite* spr = sprites[sp];
    spr->rotationRad = rad;
}


void Sprite::Belong(int sp, std::any instance)
{
    sprites[sp]->belong = instance;
}

std::any Sprite::GetBelong(int sp)
{
    return sprites[sp]->belong;
}

void Sprite::Link(int sp, int link)
{
    sprites[sp]->link = link;
}


double Sprite::GetLinkDifference_X(int sp)
{
    Sprite* spr = sprites[sp];
    int link = spr->link;
    if (link != -1)
    {
        return sprites[link]->x + GetLinkDifference_X(link);
    }
    else
    {
        return 0;
    }
}

double Sprite::GetLinkDifference_Y(int sp)
{
    Sprite* spr = sprites[sp];
    int link = spr->link;
    if (link != -1)
    {
        return sprites[link]->y + GetLinkDifference_Y(link);
    }
    else
    {
        return 0;
    }
}

void Sprite::Blend(int sp, int blendMode, int blendPal)
{
    BlendMode(sp, blendMode);
    BlendPal(sp, blendPal);
}
void Sprite::BlendMode(int sp, int blendMode)
{
    sprites[sp]->blendMode = blendMode;
}
void Sprite::BlendPal(int sp, int blendPal)
{
    sprites[sp]->blendPal = blendPal;
}


void Sprite::Update(int sp, void (*updateMethod)(int hSp))
{
    sprites[sp]->updateMethod = updateMethod;
}
void Sprite::Drawing(int sp, void (*drawingMethod)(int hSp, int hX, int hY))
{
    sprites[sp]->drawingMethod = drawingMethod;
}
void Sprite::Destructer(int sp, void (*destructerMethod)(int hSp))
{
    sprites[sp]->destructerMethod = destructerMethod;
}

void Sprite::Clear(int sp)
{
    Sprite* spr = sprites[sp];
    if (spr->isUsed)
    {
        if (spr->destructerMethod != nullptr)
        {
            //printf("delted %d\n", sp);
            spr->destructerMethod(sp);
        }
        spr->isUsed = false;
    }
    else
    {
        return;
    }
}
void Sprite::Clear(int sp, bool unprotectOnly)
{
    Sprite* spr = sprites[sp];
    if (unprotectOnly)
    {
        if (!spr->isProtect) spr->Clear(sp);
    }
    else
    {
        spr->Clear(sp);
    }
}

void Sprite::AllClear()
{
    AllClear(false);
}
void Sprite::AllClear(bool protectOnly)
{
    for (int i = 0; i < SPRITE_MAX; i++)
    {
        Sprite* spr = sprites[i];
        if (protectOnly)
        {
            if (!spr->isProtect) Clear(i);
        }
        else
        {
            Clear(i);
        }
    }
}

void Sprite::Protect(int sp, bool isProtect)
{
    sprites[sp]->isProtect = isProtect;
}



void Sprite::AllUpdate()
{
    for (int i = 0; i < SPRITE_MAX; i++)
    {
        Sprite* spr = sprites[i];
        if (spr->isUsed)
        {
            if (spr->updateMethod != nullptr) spr->updateMethod(i);
        }
    }
    //AnimationAllUpdate();
}


void Sprite::AllDrawing()
{

    // (修正)後でヒープに移動させる
    static std::pair<int, short> ol[SPRITE_MAX];
    memcpy(ol, sprites_Z, sizeof(sprites_Z[0]) * SPRITE_MAX);

    std::sort(&ol[0], &ol[0] + sizeof(ol) / sizeof(ol[0]),
        [](auto& left, auto& right) { return left.second > right.second; });

    for (int i = 0; i < SPRITE_MAX; i++)
    {
        int sp = ol[i].first;
        Sprite* spr = sprites[sp];
        if (spr->isUsed)
        {

            if (spr->image == -1) continue;

            int x, y;
            x = int(spr->x + GetLinkDifference_X(sp));
            y = int(spr->y + GetLinkDifference_Y(sp));

            x *= ROUGH_SCALE;
            y *= ROUGH_SCALE;
            DxLib::SetDrawBlendMode(spr->blendMode, spr->blendPal);

            if (spr->drawingMethod != nullptr)
            {
                spr->drawingMethod(sp, x, y);
            }
        }
    }
    DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

}

double Sprite::GetUsingRate()
{
    int use = 0;
    for (int i = 0; i < SPRITE_MAX; i++)
    {
        if (sprites[i]->isUsed) use++;
    }
    return ((double)use) / SPRITE_MAX;
}




void Sprite::DrawingProcess::Rough(int hSp, int hX, int hY)
{
    Draw(hSp, hX, hY, ROUGH_SCALE);
}
void Sprite::DrawingProcess::Twice(int hSp, int hX, int hY)
{
    Draw(hSp, hX, hY, 2);
}
void Sprite::DrawingProcess::DotByDot(int hSp, int hX, int hY)
{
    Draw(hSp, hX, hY, 1);
}
void Sprite::DrawingProcess::Draw(int sp, int x, int y, int scale)
{
    Sprite* spr = sprites[sp];
    int image = DxLib::DerivationGraph(spr->u, spr->v, spr->width, spr->height, spr->image);

    if (spr->rotationRad == 0)
    {
        DxLib::DrawRotaGraph3(x, y, 0, 0, scale, scale, 0, image, 1, spr->isReverse);
    }
    else
    {
        int x1 = spr->width / 2, y1 = spr->height / 2;
        double cos = std::cos(spr->rotationRad), sin = std::sin(spr->rotationRad);

        double x2 = x1 * cos - y1 * sin, y2 = x1 * sin + y1 * cos;

        double dx = x1 - x2, dy = y1 - y2;

        DxLib::DrawRotaGraph3(x + int(dx * scale), y + int(dy * scale),
            0, 0, // 回転中心
            scale, scale,
            spr->rotationRad, image,
            1, spr->isReverse);
    }
    DxLib::DeleteGraph(image);
}













