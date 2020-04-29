#pragma once
#include<cassert>
#include<condition_variable>
#include<mutex>
#include<queue>

/*
 * Bounded/Unbounded concurrent queue.
 *
 * User can specify the size of the queue.  If not specified, it's treated as an
 * unbounded queue.
 */
namespace PROJECT_NAMESPACE {
	template <typename T>
	class ConQueue {
		/* Member variables */
		std::queue<T> _queue;		/* the queue */
		std::size_t	_boundedSize;	/* the maximum size of the queue, 0 for
									unbounded queue */
		bool		_done;			/* if true, workers should exit immediately */

		/* Protects the member variables */
		std::mutex	_mutex;

		std::condition_variable	_readerCV;
		std::condition_variable	_writerCV;
		std::condition_variable	_finishCV;

		bool isFull() {
			if (_boundedSize == 0)
				return false;
			if (_queue.size() < _boundedSize)
				return false;
			return true;
		}

		public:

		ConQueue(std::size_t boundedSize = 0) : _boundedSize(boundedSize), _done(false) {}

		template <typename Arg>
		bool push(Arg&& item)	{
			{
				/*
				 * unique_lock gurantees an unlocked status on destruction (even
				 * if not called explicitely.
				 */
				std::unique_lock<std::mutex> lock(_mutex);
				while(isFull() && !_done)	{
					_writerCV.wait(lock);
				}

				if (_done)
					return false;

				_queue.push(std::forward<Arg>(item));
			}

			_readerCV.notify_one();

			return true;
		}

		bool pop(T &item)	{
			{
				/*
				 * unique_lock gurantees an unlocked status on destruction (even
				 * if not called explicitely.
				 */
				std::unique_lock<std::mutex> lock(_mutex);
				while(_queue.empty() && !_done)	{
					_readerCV.wait(lock);
				}

				if (_queue.empty())
					return false;

				item = _queue.front();
				_queue.pop();
			}

			_writerCV.notify_one();
			return true;
		}

		/*
		 * Once finish() is called, no further elements will be pushed into the
		 * queue.  However, workers that are trying to evict elements from the
		 * queue will be able to do so until the queue is empty.
		 */
		void finish()	{
			{
				std::lock_guard<std::mutex> lock(_mutex);
				assert(!_done);
				_done = true;
			}

			_readerCV.notify_all();
			_writerCV.notify_all();
			_finishCV.notify_all();
		}

		/* Wait for all workers to finish */
		void WaitForWorkersToFinish()	{
			std::unique_lock<std::mutex> lock(_mutex);
			while(!_done)	{
				_finishCV.wait(lock);
			}
		}
	};
}
