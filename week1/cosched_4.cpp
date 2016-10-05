#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream> 
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <mutex>
#include <thread>
#include <vector>
#define NUM_THREADS 2
// These are the producer threads. 
// It sends one message to the message queue and waits for the message to be consumed
// Since the producer sends only when the size of the queue is 0, the producer will 
// be blocked untill the consumer threads consume the message.
int main(int argc, char* argv[ ]) 
{
    std::vector<std::thread> threads(NUM_THREADS);
    std::mutex io_mutex;
    std::mutex io_mutex1;
    std::string msg_q;
    using namespace boost::interprocess;
    long int execCount[] = {0, 0};
    std::string mqName[] = {"two", "three"};
    time_t end = time(NULL) + 60;
    int buffer;
    size_t recvd_size;
    unsigned int priority;
    for (unsigned int i = 0; i < NUM_THREADS; i++)
    {
      thread_local int msg_num = 0;
      threads[i] = std::thread([&io_mutex, &mqName, &execCount, end, i, &buffer, &recvd_size, &priority, &io_mutex1]
      {
        while(time(NULL) <= end)
        {
          try 
          {
             {
                message_queue mq (open_or_create, mqName[i].c_str(),  1, sizeof(int));
                { 
                   std::lock_guard<std::mutex> iolock(io_mutex);
                   if((mq.get_num_msg() == 0) && (mq.try_send(&msg_num, sizeof(int), 0)))
                   {
                     //Increment execCount only on successful send just to keep track of dependency on receiver.
                     execCount[i]++;
                     msg_num++;
                     std::cout << "Thread id\t" << syscall( __NR_gettid ) <<  "\tCPU\t" << sched_getcpu() << "\tExec Count\t" << execCount[i] << "\tMessage Number\t" << msg_num << std::endl;
                     //int count = 0;
                     //while(count < INT_MAX) {count++;}
                   }
                }
                {
                   std::lock_guard<std::mutex> iolock(io_mutex1);
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
