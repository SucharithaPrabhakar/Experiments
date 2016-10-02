#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream> 

int main(int argc, char* argv[ ]) 
{
    using namespace boost::interprocess;
    try { 
      // opening the message queue whose name is mq
      message_queue mq (open_only,     // only open
                                       "mq"               // name
                                        );
      size_t recvd_size; 
      unsigned int priority; 

      // now send the messages to the queue
      for (int i=0; i<20; ++i) { 
        int buffer; 
        mq.receive ((void*) &buffer, sizeof(int), recvd_size, priority); 
        if (recvd_size != sizeof(int))
	  throw "Received an inappropriate value"; 
        std::cout << buffer << " " << recvd_size << " " << priority << std::endl;
      } 
      message_queue::remove("mq"); //Remove the queue after receive
    } catch (interprocess_exception& e) { 
        std::cout << e.what( ) << std::endl;
    } 
}
