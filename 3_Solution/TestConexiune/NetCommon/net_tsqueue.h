#pragma once

#include "net_common.h"

namespace olc
{
	namespace net
	{
#define SCOPE std::scoped_lock lock(muxQueue);
		template<typename T>
		class tsqueue
		{
		public:
			tsqueue() = default;
			tsqueue(const tsqueue<T>&) = delete;
			virtual ~tsqueue() { clear(); }

		public:
			// Returns and maintains item at front of Queue
			const T& front()
			{
				//
				return deqQueue.front();
			}

			// Returns and maintains item at back of Queue
			const T& back()
			{
				//
				return deqQueue.back();
			}

			// Removes and returns item from front of Queue
			T pop_front()
			{
				//
				auto t = std::move(deqQueue.front());
				deqQueue.pop_front();
				return t;
			}

			// Removes and returns item from back of Queue
			T pop_back()
			{
				//
				auto t = std::move(deqQueue.back());
				deqQueue.pop_back();
				return t;
			}

			// Adds an item to back of Queue
			void push_back(const T& item)
			{
				//
				deqQueue.emplace_back(std::move(item));

				std::unique_lock<std::mutex> ul(muxBlocking);
				cvBlocking.notify_one();
			}

			// Adds an item to front of Queue
			void push_front(const T& item)
			{
				//
				deqQueue.emplace_front(std::move(item));

				std::unique_lock<std::mutex> ul(muxBlocking);
				cvBlocking.notify_one();
			}

			// Returns true if Queue has no items
			bool empty()
			{
				//
				return deqQueue.empty();
			}

			// Returns number of items in Queue
			size_t count()
			{
				//
				return deqQueue.size();
			}

			// Clears Queue
			void clear()
			{
				//
				deqQueue.clear();
			}

			void wait()
			{
				while (empty())
				{
					std::unique_lock<std::mutex> ul(muxBlocking);
					cvBlocking.wait(ul);
				}
			}

		protected:
			std::mutex muxQueue;
			std::deque<T> deqQueue;
			std::condition_variable cvBlocking;
			std::mutex muxBlocking;
		};
	}
}