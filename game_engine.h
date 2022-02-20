#pragma once
#include "sprite.h"
#include "game_utils.h"

namespace gameEngine
{
	// 基底オブジェクト
	class Actor
	{
	protected:
		Sprite* mSpr;
		double mTime = 0;
	public:
		Sprite* GetSpr();
		double GetTime();
	protected:
		virtual void update();
		virtual void destructor();
	private:
		static void callUpdate(Sprite* hSpr);
		static void callDestructor(Sprite* hSpr);
	public:
		Actor();
	};

	
	class SelfDrawingActor : public Actor
	{
	protected:
		virtual void drawing(int hX, int hY) = 0;
	private:
		static void callDrawing(Sprite* hSpr, int hX, int hY);
	public:
		SelfDrawingActor();
	};



	namespace collider
	{
		enum class EType
		{
			RECTANGLE = 0,
		};

		class Shape
		{
		public:
			virtual inline EType Type() = 0;
		};

		class Rectangle : public Shape
		{
		public:
			EType Type() override;
			int ColX;
			int ColY;
			int ColWidth;
			int ColHeight;
			Rectangle(int colX, int colY, int colWidth, int colHeight);
		};
	}


	class CollideActor : public Actor
	{
	private:
		static std::vector<CollideActor*> colliders;
	public:
		static std::vector<CollideActor*> GetColliders();
	protected:
		double mX=0, mY=0;
		UINT mColbit;
	private:
		collider::Shape* mShape;
	public:
		auto GetX();
		auto GetY();
		auto GetColbit();
		auto GetShape();
		void SetX(double value);
		void SetY(double value);
		CollideActor(collider::Shape* shape, UINT colbit);

		CollideActor* GetHit();
		bool GetHitWith(CollideActor* actor);
	protected:
		void destructor() override;
	};

	

	namespace hit
	{
		bool CheckRectRect(collider::Rectangle* rect1, collider::Rectangle* rect2);
		bool CheckRectRect(double x1, double y1, int width1, int height1, double x2, double y2, int width2, int height2);

		CollideActor* GetHitRect(double x, double y, int width, int height, UINT colbit);
		std::pair<CollideActor*, int> GetHitRectFromIndex(double x, double y, int width, int height, UINT colbit,int firstIndex);
		bool GetHitRectWith(double x, double y, int width, int height, UINT colbit, CollideActor* col);
		std::vector<CollideActor*> GetHitRectAll(double x, double y, int width, int height, UINT colbit);
		CollideActor* GetHitCollideActor(CollideActor* actor);

	}
};


