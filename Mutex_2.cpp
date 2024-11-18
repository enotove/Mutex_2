// Mutex_2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <atomic>
#include <chrono>
#include <vector>
#include <thread>
#include <mutex>
using namespace std::chrono_literals;

HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);

std::mutex m;

void SetCource(short x, short y)
{
	SetConsoleCursorPosition(hStd, { x,y });
}

void progress(int id, int& over_number)
{
	
	m.lock();
	std::cout << '\n' << id << '\t' << std::this_thread::get_id() << '\t';
	std::cout << (char)254;
	std::this_thread::sleep_for(100ms);

	m.unlock();
	std::atomic<int> i{ 0 };
	
	auto start = std::chrono::steady_clock::now();
	
	for (; i < 6; i.fetch_add(1))
	{	
		m.lock();
		SetCource(16 + i.load(), id + 1);
		std::cout << (char)254;
		int random = rand() % 5;
		m.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(random));
		
	}
	
	auto end = std::chrono::steady_clock::now();
	
	
	std::chrono::duration<double, std::milli> time = end - start;
	SetCource(i.load() + 16 + 6, id + 1);
	std::cout << time.count();

}


int main()
{
	size_t count_threads = 4;
	int over_number = 8;
	std::vector<std::thread> threads;
	std::cout << "#\tid\tProgress Bar\tTime";
	for (std::atomic<int> i{ 0 }; i.load() < count_threads; i.fetch_add(1))
	{
		threads.push_back(std::thread(progress, i.load(), std::ref(over_number)));
	}
	for (auto& th : threads)
	{
		th.join();
	}
	
}
