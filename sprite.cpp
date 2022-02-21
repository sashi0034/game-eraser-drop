#define _USE_MATH_DEFINES

#include "sprite.h"
#include <math.h>
#include <random>
#include <iostream>




std::vector<Sprite*> Sprite::sprites = std::vector<Sprite*>();

void Sprite::Init()
{
    sprites = std::vector<Sprite*>();
}


void Sprite::End()
{
    Sprite::DisposeAll();
}


Sprite::Sprite() : Sprite(Graph::NONE, 0, 0, 0, 0)
{}
Sprite::Sprite(Graph image) : Sprite(image, 0, 0, 0, 0)
{
    int x = 0, y = 0;
    DxLib::GetGraphSize(image.getHandler(), &x, &y);
    this->width = x;
    this->height = y;
}
Sprite::Sprite(Graph image, int u, int v, int w, int h)
{
    this->image = image;
    this->u = u;
    this->v = v;
    this->width = w;
    this->height = h;
    this->drawingMethod = Sprite::DrawingProcess::Rough;

    sprites.push_back(this);
}



void Sprite::SetFlip(bool isFlip)
{
    this->isFlip = isFlip;
}


void Sprite::SetImage(Graph image)
{
    this->image = image;
}
void Sprite::SetImage(int u, int v)
{
    this->u = u;
    this->v = v;
}
void Sprite::SetImage(int u, int v, int width, int height)
{
    this->SetImage(u, v);
    this->width = width;
    this->height = height;
}
void Sprite::SetImage(Graph image, int u, int v, int width, int height)
{ 
    this->SetImage(image);
    this->SetImage(u, v, width, height);
}


void Sprite::SetXY(double x, double y)
{
    this->x = x;
    this->y = y;
}
void Sprite::SetZ(double z)
{
    this->z = z;
}
double Sprite::GetZ()
{
    return this->z;
}


void Sprite::GetScreenXY(int* x, int* y)
{
    double x1, y1;
    this->GetLinkDifferenceXY(&x1, &y1);

    *x = int(this->x + x1);
    *y = int(this->y + y1);
}


void Sprite::SetRotationDeg(double deg)
{
    this->rotationRad = (deg / 180.0) * M_PI;
}
void Sprite::SetRotationRad(double rad)
{
    this->rotationRad = rad;
}


void Sprite::SetBelong(std::any instance)
{
    this->belong = instance;
}

std::any Sprite::GetBelong()
{
    return this->belong;
}

void Sprite::SetLinkXY(const Sprite* linkSpr)
{
    this->linkXY = linkSpr;
}


void Sprite::GetLinkDifferenceXY(double* x, double* y)
{
    const Sprite* linkSpr = this->linkXY;
    if (linkSpr != nullptr)
    {
        double x1, y1;
        Sprite::GetLinkDifferenceXY(&x1, &y1);
        *x = linkSpr->x + x1;
        *y = linkSpr->y + y1;
        return;
    }
    else
    {
        *x = 0;
        *y = 0;
        return;
    }
}


void Sprite::SetBlend(int blendMode, int blendPal)
{
    this->SetBlendMode(blendMode);
    this->SetBlendPal(blendPal);
}
void Sprite::SetBlendMode(int blendMode)
{
    this->blendMode = blendMode;
}
void Sprite::SetBlendPal(int blendPal)
{
    this->blendPal = blendPal;
}


void Sprite::SetUpdateMethod(void (*updateMethod)(Sprite* hSp))
{
    this->updateMethod = updateMethod;
}
void Sprite::SetDrawingMethod(void (*drawingMethod)(Sprite* hSp, int hX, int hY))
{
    this->drawingMethod = drawingMethod;
}
void Sprite::SetDestructorMethod(void (*destructorMethod)(Sprite* hSp))
{
    this->destructorMethod = destructorMethod;
}


void Sprite::SetProtect(bool isProtect)
{
    this->isProtect = isProtect;
}

void Sprite::Dispose(Sprite* spr)
{
    if (spr == nullptr) return;
    if (spr->destructorMethod != nullptr)
    {
        spr->destructorMethod(spr);
    }

    spr->sprites[findIndex(spr)] = nullptr;
}


void Sprite::Dispose(Sprite* spr, bool isUnprotectOnly)
{
    if (isUnprotectOnly)
    {
        if (!spr->isProtect) Dispose(spr);
    }
    else
    {
        Dispose(spr);
    }
}

void Sprite::DisposeAll()
{
    DisposeAll(false);
}
void Sprite::DisposeAll(bool isProtectOnly)
{
    for (Sprite* spr : sprites)
    {
        if (isProtectOnly)
        {
            if (!spr->isProtect) Dispose(spr);
        }
        else
        {
            Dispose(spr);
        }
    }
    garbageCollect();
}

int Sprite::findIndex(Sprite* spr)
{
    auto size = sprites.size();
    if (size == 0) return -1;

    auto iter = std::find(sprites.begin(), sprites.end(), spr);
    size_t index = std::distance(sprites.begin(), iter);
    if (index == size)
    {
        return -1;
    }
    return index;
}
void Sprite::garbageCollect()
{
    while (true)
    {
        int index = findIndex(nullptr);
        if (index == -1) break;
        sprites.erase(sprites.begin() + index);
    }
}




void Sprite::UpdateAll()
{
    for (Sprite* spr : sprites)
    {
        if (spr != nullptr)
        {
            if (spr->updateMethod != nullptr) spr->updateMethod(spr);
        }
    }
    garbageCollect();
}


void Sprite::DrawingAll()
{
    std::sort(sprites.begin(), sprites.end(),
        [](Sprite* left, Sprite* right) { return left->GetZ() > right->GetZ(); });

    for (Sprite* spr : sprites)
    {
        if (spr != nullptr)
        {
            if (spr->image == Graph::NONE) continue;

            int x, y;
            double x1, y1;
            spr->GetLinkDifferenceXY(&x1, &y1);

            x = int(spr->x + x1);
            y = int(spr->y + y1);

            x *= ROUGH_SCALE;
            y *= ROUGH_SCALE;
            DxLib::SetDrawBlendMode(spr->blendMode, spr->blendPal);

            if (spr->drawingMethod != nullptr)
            {
                spr->drawingMethod(spr, x, y);
            }
        }
    }
    DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

}




void Sprite::DrawingProcess::Rough(Sprite* hSpr, int hX, int hY)
{
    Draw(hSpr, hX, hY, ROUGH_SCALE);
}
void Sprite::DrawingProcess::Twice(Sprite* hSpr, int hX, int hY)
{
    Draw(hSpr, hX, hY, 2);
}
void Sprite::DrawingProcess::DotByDot(Sprite* hSpr, int hX, int hY)
{
    Draw(hSpr, hX, hY, 1);
}
void Sprite::DrawingProcess::Draw(Sprite* hSpr, int x, int y, int scale)
{
    int image = DxLib::DerivationGraph(hSpr->u, hSpr->v, hSpr->width, hSpr->height, hSpr->image.getHandler());

    if (hSpr->rotationRad == 0)
    {
        DxLib::DrawRotaGraph3(x, y, 0, 0, scale, scale, 0, image, 1, hSpr->isFlip);
    }
    else
    {
        int x1 = hSpr->width / 2, y1 = hSpr->height / 2;
        double cos = std::cos(hSpr->rotationRad), sin = std::sin(hSpr->rotationRad);

        double x2 = x1 * cos - y1 * sin, y2 = x1 * sin + y1 * cos;

        double dx = x1 - x2, dy = y1 - y2;

        DxLib::DrawRotaGraph3(x + int(dx * scale), y + int(dy * scale),
            0, 0, // ‰ñ“]’†S
            scale, scale,
            hSpr->rotationRad, image,
            1, hSpr->isFlip);
    }
    DxLib::DeleteGraph(image);
}













