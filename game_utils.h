#pragma once
#include <DxLib.h>
#include <string>
#include <vector>
#include <random>

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

	
	namespace useful
	{

		class Random
		{
			std::random_device device;
			std::default_random_engine engine;
			std::uniform_int_distribution<int> distribution;

		public:
			Random(void);
			Random(int seed);
			int Get(int max);
			int Get(int min, int max);
		};



		/*
		** template funtions
		*/
		template<typename T>
		int FindIndex(std::vector<T> arr, T target)
		{
			auto size = arr.size();
			if (size == 0) return -1;
			auto iter = std::find(arr.begin(), arr.end(), target);
			size_t index = std::distance(arr.begin(), iter);
			if (index == size)
			{
				return -1;
			}
			return index;
		}

		template<typename T>
		int Remove(std::vector<T> arr, T target)
		{
			int index = useful::FindIndex<T>(arr, target);
			if (index == -1) return -1;
			arr.erase(arr.begin() + index);
			return 0;
		}

	}
}


