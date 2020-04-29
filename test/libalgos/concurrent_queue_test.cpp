#include<iostream>
#include<thread>

#include "gtest/gtest.h"
#include<libalgos/concurrent_queue.h>

namespace PROJECT_NAMESPACE {

	TEST(ConcurrentQueue, initializationTest) {
		ConQueue<int> queue;
		int item;

		queue.push(1);
		queue.push(2);

		queue.finish();

		/* Failed push after finish is called. */
		EXPECT_FALSE(queue.push(3));

		EXPECT_TRUE(queue.pop(item));
		EXPECT_EQ(item, 1);
		EXPECT_TRUE(queue.pop(item));
		EXPECT_EQ(item, 2);

		/* Failed pop */
		EXPECT_FALSE(queue.pop(item));

		queue.WaitForWorkersToFinish();
	}

	struct Region	{
		int start;
		int offset;
		int result;

		Region() : start(-1), offset(-1), result(0) {}
	};

	TEST(ConcurrentQueue, SingleProducerMultipleConsumers) {
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

		EXPECT_EQ(sum, num * (num + 1) / 2);
	}
}
