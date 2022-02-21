#include "game_engine.h"

namespace gameEngine{
	Sprite* Actor::GetSpr()
	{
		return mSpr;
	}
	double Actor::GetTime()
	{
		return mTime;
	}
	void Actor::update()
	{
		mTime += 1;
	}
	void Actor::destructor()
	{}

	void Actor::callUpdate(Sprite* hSpr)
	{
		std::any_cast<Actor*>(hSpr->GetBelong())->update();
	}

	void Actor::callDestructor(Sprite* hSpr)
	{
		std::any_cast<Actor*>(hSpr->GetBelong())->destructor();
		delete std::any_cast<Actor*>(hSpr->GetBelong());
	}

	Actor::Actor()
	{
		mSpr = new Sprite();
		mSpr->SetUpdateMethod(callUpdate);
		mSpr->SetDestructorMethod(callDestructor);
		mSpr->SetBelong(this);
	}


}

namespace gameEngine
{
	void SelfDrawingActor::callDrawing(Sprite* hSpr, int hX, int hY)
	{
		std::any_cast<SelfDrawingActor*>(hSpr->GetBelong())->drawing(hX, hY);
	}
	SelfDrawingActor::SelfDrawingActor() : Actor()
	{
		mSpr->SetDrawingMethod(callDrawing);
	}
}

namespace gameEngine
{
	std::vector<CollideActor*> CollideActor::colliders = {};

	std::vector<CollideActor*> CollideActor::GetColliders()
	{
		return colliders;
	}
	auto CollideActor::GetX()
	{
		return mX;
	}
	auto CollideActor::GetY()
	{
		return mY;
	}
	auto CollideActor::GetColbit()
	{
		return mColbit;
	}
	auto CollideActor::GetShape()
	{
		return mShape;
	}
	void CollideActor::SetX(double v)
	{
		mX = v;
	}
	void CollideActor::SetY(double value)
	{
		mY = value;
	}

	CollideActor::CollideActor(collider::Shape* shape, UINT colbit) : Actor()
	{
		mShape = shape;
		mColbit = colbit;
		CollideActor::colliders.push_back(this);
	}

	CollideActor* CollideActor::GetHit()
	{
		return hit::GetHitCollideActor(this);
	}

	bool CollideActor::GetHitWith(CollideActor* actor)
	{
		switch (actor->GetShape()->Type())
		{
		case collider::EType::RECTANGLE:
		{
			auto rect = (collider::Rectangle*)mShape;
			return hit::GetHitRectWith(mX+rect->ColX, mY+rect->ColY, rect->ColWidth, rect->ColHeight, mColbit, actor);
		}
		default:
			break;
		}
		return false;
	}

	void CollideActor::destructor()
	{
		gameUtils::useful::Remove<CollideActor*>(colliders, this);
		Actor::destructor();
	}


}


namespace gameEngine
{
	collider::EType collider::Rectangle::Type()
	{
		return EType::RECTANGLE;
	}
	collider::Rectangle::Rectangle(int colX, int colY, int colWidth, int colHeight)
	{
		ColX = colX;
		ColY = colY;
		ColWidth = colWidth;
		ColHeight = colHeight;
	}
}

namespace gameEngine
{
	bool hit::CheckRectRect(collider::Rectangle* rect1, collider::Rectangle* rect2)
	{
		return CheckRectRect(rect1->ColX, rect1->ColY, rect1->ColWidth, rect1->ColHeight,
			rect2->ColX, rect2->ColY, rect2->ColWidth, rect2->ColHeight);
	}
	bool hit::CheckRectRect(double x1, double y1, int width1, int height1, double x2, double y2, int width2, int height2)
	{
		return (std::abs(x2 + width2 / 2.0 - (x1 + width1 / 2.0)) < (width1 + width2) / 2.0 && std::abs(y2 + height2 / 2.0 - (y1 + height1 / 2.0)) < (height1 + height2) / 2.0);
	}


	CollideActor* hit::GetHitRect(double x, double y, int width, int height, UINT colbit)
	{
		return GetHitRectFromIndex(x, y, width, height, colbit, 0).first;
	}

	std::pair<CollideActor*, int> hit::GetHitRectFromIndex(double x, double y, int width, int height, UINT colbit, int firstIndex)
	{
		CollideActor* ret = nullptr;
		int retI = -1;

		auto cols = CollideActor::GetColliders();
		for (int i = 0; i < cols.size(); ++i)
		{
			auto col = cols[i];
			if ((col->GetColbit() & colbit) != 0)
			{
				if (GetHitRectWith(x, y, width, height, colbit, col) == true)
				{
					ret = col;
					retI = i;
					break;
				}
			}
		}
		return std::pair<CollideActor*, int>{ret, retI};
	}

	bool hit::GetHitRectWith(double x, double y, int width, int height, UINT colbit, CollideActor* col)
	{
		auto shape = col->GetShape();
		switch (shape->Type())
		{// ’·•ûŒ`“¯Žm‚Ì“–‚½‚è”»’è
		case (collider::EType::RECTANGLE):
		{
			auto rect = (collider::Rectangle*)shape;
			if (CheckRectRect(col->GetX() + rect->ColX, col->GetY() + rect->ColY, rect->ColWidth, rect->ColHeight,
				x, y, width, height))
			{
				return true;
			}
			break;
		}
		default:
			break;
		}
		return false;
	}

	std::vector<CollideActor*> hit::GetHitRectAll(double x, double y, int width, int height, UINT colbit)
	{
		std::vector<CollideActor*> ret = {};
		int i = 0;
		while (true)
		{
			CollideActor* col = nullptr;
			auto res = GetHitRectFromIndex(x, y, width, height, colbit, i);
			col = res.first;
			i = res.second;
			ret.push_back(col);
			i = i + 1;
		}
		return ret;
	}

	CollideActor* hit::GetHitCollideActor(CollideActor* actor)
	{
		CollideActor* ret = nullptr;

		int i = 0;
		while (true)
		{
			CollideActor* col = nullptr;
			switch (col->GetShape()->Type())
			{
			case collider::EType::RECTANGLE:
			{
				auto rect = (collider::Rectangle*)actor->GetShape();
				auto res = hit::GetHitRectFromIndex(actor->GetX()+rect->ColX, actor->GetY()+rect->ColY,
					rect->ColWidth, rect->ColHeight, actor->GetColbit(), i);
				col = res.first;
				i = res.second;
				break;
			}
			default:
				break;
			}

			if (col == actor)
			{
				i = i + 1;
				continue;
			}
			ret = col;
			break;
		}
		return ret;

	}

	

	

}
