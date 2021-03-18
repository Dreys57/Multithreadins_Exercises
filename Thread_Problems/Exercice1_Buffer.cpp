#include <iostream>
#include <queue>
#include <omp.h>

const int nb_of_elements = 30;

template <int N, typename T>
class BufferMutliThread
{
public:
	// Add an element to the back.
	void Add(T element)
	{
		queue_.push(element);

		count++;

		if(count == N)
		{
			full = true;
		}

		if(count == 1)
		{
			empty = false;
		}
	}
	// Pop en element from the front.
	T Get(T element)
	{
		queue_.pop();

		count--;

		if(count == 0)
		{
			empty = true;
		}

		if (count == (N - 1))
		{
			full = false;
		}

		return element;
	}
protected:
	// FIFO (first-in, first-out) queue.
	std::queue<T> queue_;
	int count = 0;
	bool full = false;
	bool empty = true;
};

void Producer(int thread_ID, BufferMutliThread<5, int>& buffer)
{
	for(int i = 0; i < nb_of_elements; ++i)
	{
		#pragma omp critical(CRIT)
		{
			buffer.Add(i);

			std::cout << "Producing \n" << i << std::endl;

			i++;
		}		
	}	
}

void Consumer(int thread_ID, BufferMutliThread<5, int>& buffer)
{
	for (int i = 0; i < nb_of_elements; ++i)
	{
		#pragma omp critical(CRIT)
		{
			int element = buffer.Get(i);

			std::cout << "Consuming \n" << element << std::endl;

			i++;
		}
	}
}

int main()
{
	int thread_ID;
	
	BufferMutliThread<5, int> buffer;
	#pragma omp parallel num_threads(4)
	{
		thread_ID = omp_get_thread_num();

		if(thread_ID % 2 == 1)
		{
			Producer(thread_ID, buffer);
		}
		else
		{
			Consumer(thread_ID, buffer);
		}
	}
	
}
