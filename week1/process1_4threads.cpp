#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#define NUM_THREADS 4
using namespace std;

int main(int argc, const char** argv) 
{
  std::mutex coutmutex;
  std::vector<std::thread> threads(NUM_THREADS);
  for (unsigned int i = 0; i < NUM_THREADS; i++) 
  {
    threads[i] = std::thread([&coutmutex, i] 
    {
      while (true) 
      {
        {
          std::lock_guard<std::mutex> iolock(coutmutex);
          std::cout << "Thread #" << i << ": on CPU " << sched_getcpu() << "\n";
        }
        // Add a sleep just to simulate random computation to be done by the thread
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      }
    });
  }

  for (auto& t : threads) {
    t.join();
  }
  return 0;
}
