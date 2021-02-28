#include <queue>
#include <mutex>

template <class T>
class SynchronizedQueue
{
private:
	std::queue<T> _queue;
	std::mutex _mutex;

public:
	void enqueue(T& t)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_queue.push(t);
	}

	T dequeue()
	{
		std::unique_lock<std::mutex> lock(_mutex);

		if (_queue.empty()) 
		{
			return NULL;
		}

		T frontItem = _queue.front();
		_queue.pop();
		return frontItem;
	}

	bool empty()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		return _queue.empty();
	}
};