#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#define NUM_THREADS 2
#define EXEC_TIMES 1000
using namespace std;

int main(int argc, const char** argv) 
{
  std::mutex coutmutex;
  std::vector<std::thread> threads(NUM_THREADS);
  int arr[NUM_THREADS][EXEC_TIMES] = {0};
  for(unsigned int i = 0; i < NUM_THREADS; i++) 
  {
    thread_local unsigned int j = 0; 
    threads[i] = std::thread([&coutmutex, &arr, i] 
    {
      while(j < EXEC_TIMES) 
      {
        {
          std::lock_guard<std::mutex> iolock(coutmutex);
          std::cout << "Thread #" << i << ": on CPU\t" << sched_getcpu() << "\n";
          arr[i][j++] = sched_getcpu();
        }
        // Add a sleep just to simulate random computation to be done by the thread
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      }
    });
  }

  for(auto& t : threads) {
    t.join();
  }
  for(unsigned int i = 0; i < NUM_THREADS; i++)
  {
    std::cout << "THREAD\t" << i << "\n\n";
    for (unsigned int j = 0; j < EXEC_TIMES; j++)
    {
       std::cout << arr[i][j] << ",";
    }
    std::cout << "\n";
  }
  return 0;
}
