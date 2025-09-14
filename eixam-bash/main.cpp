#include <boost/interprocess/ipc/message_queue.hpp>
#include <fstream>
#include <sstream>

using namespace boost::interprocess;

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;

    std::ifstream file(argv[1]);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string script = buffer.str();

    message_queue mq(open_or_create, "script_queue", 10, 4096);
    mq.send(script.data(), script.size(), 0);
    
    return 0;
}