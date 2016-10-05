#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream> 
#include<linux/unistd.h>
#include<sys/syscall.h>
#include<mutex>
#include<vector>
#include<thread>
#define NUM_THREADS 2
int main(int argc, char* argv[ ]) 
{
    using namespace boost::interprocess;
    std::vector<std::thread> threads(NUM_THREADS);
    std::mutex io_mutex;
    long int execCount[] = {0, 0};
    std::string mqName[] = {"zero", "one", "two", "three"};
    size_t recvd_size;  
    unsigned int priority;
    int buffer;
    time_t end = time(NULL) + 60;
    for (unsigned int i = 0; i < NUM_THREADS; i++)
    {
      threads[i] = std::thread([&io_mutex, &mqName, &execCount, &buffer, &recvd_size, &priority, end, i]
      {
        while (time(NULL) <= end) 
	{
          try 
          {
             {
                std::lock_guard<std::mutex> iolock(io_mutex);
                int rand_num = rand()%4;
                message_queue mq (open_only, mqName[rand_num].c_str());
                for (int msg_num = 0; msg_num < 20; ++msg_num)
                {
		   if(mq.try_receive ((void*) &buffer, sizeof(int), recvd_size, priority))
                   {
                     if (recvd_size != sizeof(int))
                     {
                       throw "Received an inappropriate value";
                     }
                     std::cout << "Thread id\t" << syscall( __NR_gettid ) <<  "\tCPU\t" << sched_getcpu() << "\tExecCount\t" << ++execCount[i] <<  std::endl;
                     std::cout << buffer << " " << recvd_size << " " << priority << std::endl;
                   }
                }
             }
             // Add a sleep just to simulate random computation to be done by the thread
             std::this_thread::sleep_for(std::chrono::milliseconds(250));
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
  for(unsigned int j = 0; j < NUM_THREADS; j++)
  {
    std::cout << "Exec count of thread\t" << j << "is\t" << execCount[j] << std::endl;
  }
}
