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

		bool operator ==(Graph r);

		static Graph LoadGraph(const char* path);
		static const Graph NONE ;
	};


	
}

