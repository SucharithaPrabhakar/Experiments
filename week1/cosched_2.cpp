#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream> 
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <mutex>
#include <thread>
#include <vector>
#define NUM_THREADS 2

int main(int argc, char* argv[ ]) 
{
    std::vector<std::thread> threads(NUM_THREADS);
    std::mutex io_mutex;
    std::string msg_q;
    using namespace boost::interprocess;
    long int execCount[] = {0, 0};
    std::string mqName[] = {"zero", "one"};
    int buffer;
    size_t recvd_size;
    unsigned int priority;
    time_t end = time(NULL) + 240;
    for (unsigned int i = 0; i < NUM_THREADS; i++)
    {
      threads[i] = std::thread([&io_mutex, &mqName, &execCount, end, i, &buffer, &recvd_size, &priority]
      {
        while(time(NULL) <= end)
        {
          try 
          {
             {
                message_queue mq (open_only, mqName[i].c_str());
                { 
                   std::lock_guard<std::mutex> iolock(io_mutex);
                   if((mq.get_num_msg() == 1) && mq.try_receive((void*) &buffer, sizeof(int), recvd_size, priority))
                   {
                     //Increment execCount only on successful send just to keep track of dependency on receiver.
                     execCount[i]++;
                     std::cout << "Thread id\t" << syscall( __NR_gettid ) <<  "\tCPU\t" << sched_getcpu() << "\tExec Count\t" << execCount[i] << "\tReceived\t" << buffer << std::endl;
                     //int count = 0;
                     //while(count < INT_MAX) {count++;}
                   }
               }
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
