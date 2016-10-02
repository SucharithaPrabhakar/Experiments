#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream> 
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <mutex>

int main(int argc, char* argv[ ]) 
{
    using namespace boost::interprocess;
    std::mutex io_mutex;
    try 
    { 
      message_queue mq (open_or_create,
                        "mq", 
                         20, //allowing 20 ints in the queue
                         sizeof(int)
                        );
      for (int msg_num = 0; msg_num < 20; ++msg_num)
      { 
        {
           std::lock_guard<std::mutex> iolock(io_mutex);
           // Print the time at which the process is being scheduled
	   // and the thread on which it has been scheduled
           std::cout << "Thread id\t" << syscall( __NR_gettid ) <<  "\tCPU\t" << sched_getcpu() << std::endl;
           mq.send(&msg_num, sizeof(int), 0); // the 3rd argument is the priority
        } 
      }
    } catch (interprocess_exception& e) 
    { 
        std::cout << e.what( ) << std::endl;
    } 
}
