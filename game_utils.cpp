
#include "game_utils.h"


namespace gameUtils
{
	Graph::Graph(int handler)
	{
		mHandler = handler;
	}

	bool Graph::operator==(Graph r)
	{
		return this->mHandler == r.mHandler;
	}


	int Graph::getHandler()
	{
		return mHandler;
	}

	Graph Graph::LoadGraph(const char* path)
	{
		int handler = DxLib::LoadGraph(path);
		return Graph(handler);
	}

	const Graph Graph::NONE = Graph(-1);

	
	namespace useful
	{

		Random::Random() : engine(device()), distribution(0, INT_MAX - 1)
		{
		}
		Random::Random(int seed) : engine(seed), distribution(0, INT_MAX - 1)
		{
		}
		int Random::Get(int max)
		{
			return distribution(engine) % max;
		}
		int Random::Get(int min, int max)
		{
			return min + (distribution(engine) % (max - min));
		}




	}


}

