#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include "Algorithm.hpp"

using namespace boost::interprocess;

int wait_for_script() {
    message_queue::remove("script_queue"); // cleanup old queues

    message_queue mq(open_or_create, "script_queue", 10, 4096);
    char buf[4096];
    size_t recv_size;
    unsigned int priority;

    mq.receive(buf, sizeof(buf), recv_size, priority);
    std::string script(buf, recv_size);
    std::cout << "Got script:\n" << script << "\n";
    return 0;
}
