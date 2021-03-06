#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream> 
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <mutex>
#include <thread>
#include <vector>
#define NUM_THREADS 4

int main(int argc, char* argv[ ]) 
{
    std::vector<std::thread> threads(NUM_THREADS);
    std::mutex io_mutex;
    std::string msg_q;
    using namespace boost::interprocess;
    long int execCount[] = {0, 0, 0, 0};
    std::string mqName[] = {"zero", "one", "two", "three"};
    time_t end = time(NULL) + 60;
    for (unsigned int i = 0; i < NUM_THREADS; i++)
    {
      threads[i] = std::thread([&io_mutex, &mqName, &execCount, end, i]
      {
        while(time(NULL) <= end)
        {
          try 
          {
             {
                message_queue mq (open_or_create, mqName[i].c_str(),  20, sizeof(int));
                for (int msg_num = 0; msg_num < 20; ++msg_num)
                { 
                   std::lock_guard<std::mutex> iolock(io_mutex);
                   if(mq.try_send(&msg_num, sizeof(int), 0)) //Third arg is prio
                   {
                     //Increment execCount only on successful send just to keep track of dependency on receiver.
                     execCount[i]++;
                     std::cout << "Thread id\t" << syscall( __NR_gettid ) <<  "\tCPU\t" << sched_getcpu() << "\tExec Count\t" << execCount[i] << std::endl;
                   }
               }
               // Add a sleep just to simulate random computation to be done by the thread
               std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
         } 
         catch (interprocess_exception& e) 
         { 
           std::cout << e.what( ) << std::endl;
         }
       } 
    });
  }

  for (auto& t : threads) {
    t.join();
  }
  for(unsigned int i = 0; i < NUM_THREADS; i++)
  {
    std::cout << "Exec count of thread\t" << i << "is\t" << execCount[i] << std::endl;
  }
  return 0;
}
