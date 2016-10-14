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
    std::string mqName = "queue2";
    int buffer;
    size_t recvd_size;
    unsigned int priority;
    time_t end = time(NULL) + 120;
    for (unsigned int i = 0; i < NUM_THREADS; i++)
    {
      thread_local int msg_num = 0;
      threads[i] = std::thread([&io_mutex, &mqName, &execCount, end, i, &buffer, &recvd_size, &priority]
      {
        while(time(NULL) <= end)
        {
          try 
          {
                if((i%2) == 1) //Thread 1 in this case, reader 
                { 
                  message_queue mq (open_only, mqName.c_str());
                  { 
                    std::lock_guard<std::mutex> iolock(io_mutex);
                    if((mq.get_num_msg() == 1) && mq.try_receive((void*) &buffer, sizeof(int), recvd_size, priority))
                    {
                      //Increment execCount only on successful send just to keep track of dependency on receiver.
                      execCount[i]++;
                      msg_num++;
                      std::cout << "Thread id\t" << syscall( __NR_gettid ) <<  "\tCPU\t" << sched_getcpu() << "\tExec Count\t" << execCount[i] << "\tReceived\t" << buffer << "\tMessage number\t" << msg_num << std::endl;
                    }
                  }
                }
                else if((i%2) == 0) //Thread 0 in this case, writer
                {
                   message_queue mq (open_or_create, mqName.c_str(),  1, sizeof(int));
                   {
                      std::lock_guard<std::mutex> iolock(io_mutex);
                      if((mq.get_num_msg() == 0) && (mq.try_send(&msg_num, sizeof(int), 0)))
                      {
                        //Increment execCount only on successful send just to keep track of dependency on receiver.
                        execCount[i]++;
                        msg_num++;
                        std::cout << "Thread id\t" << syscall( __NR_gettid ) <<  "\tCPU\t" << sched_getcpu() << "\tExec Count\t" << execCount[i] << "\tMessage Number\t" << msg_num << std::endl;
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
