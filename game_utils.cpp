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

}










