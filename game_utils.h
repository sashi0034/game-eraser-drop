#pragma once
#include <DxLib.h>
#include <string>

namespace gameUtils
{
	class Graph
	{
	private:
		int mHandler;
	public:
		Graph(int handler);
		int getHandler();
		static Graph LoadGraph(const char* path);
	};


	
}

