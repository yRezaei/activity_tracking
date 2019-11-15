
#include "tracing.hpp"
#include <thread>

void print()
{
    TRACE();
}

int main(int argc, char const *argv[])
{
    TRACE();
    


    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    print();

    return 0;
}
