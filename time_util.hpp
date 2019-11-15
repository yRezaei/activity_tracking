#include <fstream>
#include <map>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <mutex>

namespace time_util
{
using Clock = std::chrono::system_clock;
const std::chrono::hours hours_24 = std::chrono::hours(24);

inline std::string get_time_stamp()
{
    const auto now_time = Clock::now();
    std::time_t now_c = Clock::to_time_t(now_time - hours_24);
    const auto now_msec = std::chrono::duration_cast<std::chrono::milliseconds>(now_time.time_since_epoch()) % 1000;
    std::stringstream stream;
    stream << std::put_time(std::localtime(&now_c), "%F %T")
           << '.' << std::setfill('0') << std::setw(3) << now_msec.count();
    return stream.str();
}

inline std::string get_time()
{
    const auto now_time = Clock::now();
    std::time_t now_c = Clock::to_time_t(now_time - hours_24);
    const auto now_msec = std::chrono::duration_cast<std::chrono::milliseconds>(now_time.time_since_epoch()) % 1000;
    std::stringstream stream;
    stream << std::put_time(std::localtime(&now_c), "%T")
           << '.' << std::setfill('0') << std::setw(3) << now_msec.count();
    return stream.str();
}

inline std::string get_date()
{
    std::time_t now_c = Clock::to_time_t(Clock::now() - hours_24);
    std::stringstream stream;
    stream << std::put_time(std::localtime(&now_c), "%F");
    return stream.str();
}
} // namespace time_util