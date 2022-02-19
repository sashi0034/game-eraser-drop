#include "game_utils.h"


namespace gameUtils
{
	Graph::Graph(int handler)
	{
		mHandler = handler;
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

}










