/*
 * In this example, we're going to calculate the sum of 1...N using a single
 * producer and multiple consuemrs.  The producers distributes the region
 * in equal intervals and the consumers calculate sum of each interval. Finally,
 * we add the summation of each interval and output the result.
 */
#include<iostream>
#include<thread>

#include<libalgos/concurrent_queue.h>

using namespace PROJECT_NAMESPACE;

struct Region	{
	int start;
	int offset;
	int result;

	Region() : start(-1), offset(-1), result(0) {}
};

int main(int argc, char **argv)	{
	ConQueue<Region *> queue;
	int item;
	int num = 1000;
	int interval = 100;

	std::vector<Region>	regions(num / interval, Region());
	for(int i = 0, start = 1; start < num; i++, start += interval)	{
		regions[i].start = start;
		regions[i].offset = interval;
		queue.push(&regions[i]);
	}

	std::vector<std::thread> con_threads;
	for (int i = 0; i < 4; i++)	{
		std::thread thread([&queue]	{
						   Region *item;
							while (queue.pop(item))	{
								int sum = 0;
								int min = item->start;
								int max = min + item->offset;
								for (int j = min; j < max; j++)
									sum += j;
								item->result = sum;
						   }
		});
		con_threads.push_back(std::move(thread));
	}

	queue.finish();

	for (auto& thread: con_threads)
		thread.join();

	int sum = 0;
	for (auto& e: regions)
		sum += e.result;

	std::cout << "The sum is " << sum << std::endl;
	return 0;
}
