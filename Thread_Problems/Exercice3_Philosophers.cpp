#include <array>
#include <iostream>
#include <mutex>
#include <thread>

struct Chopsticks {
	
	Chopsticks() = default;
	std::mutex mutex;
};

int main() {
	
	const int nb_of_philosophers = 5;
	std::array<Chopsticks, nb_of_philosophers> chopsticks;
	std::array<std::thread, nb_of_philosophers> philosophers;
	
	auto eat = [](Chopsticks& left_chopsticks, Chopsticks& right_chopsticks, int philosopher_number) {
		
		std::unique_lock<std::mutex> left_lock(left_chopsticks.mutex);
		std::unique_lock<std::mutex> right_lock(right_chopsticks.mutex);

		std::cout << "Philosopher " << philosopher_number << " is eating" << std::endl;

		std::chrono::milliseconds timeout(1500);
		std::this_thread::sleep_for(timeout);

		std::cout << "Philosopher " << philosopher_number << " has finished eating" << std::endl;
	};

	// Philosopher 1 starts eating.
	std::cout << "Philosopher 1 is thinking" << std::endl;
	philosophers[0] = std::thread(eat, std::ref(chopsticks[0]), std::ref(chopsticks[nb_of_philosophers - 1]), 1);

	for(int i = 1; i < nb_of_philosophers; ++i)
	{
		std::cout << "Philosopher " << (i + 1) << " is thinking" << std::endl;
		philosophers[i] = std::thread(eat, std::ref(chopsticks[i]), std::ref(chopsticks[i - 1]), (i + 1));
	}

	for(auto& philosopher : philosophers)
	{
		philosopher.join();
	}

	return 0;
}