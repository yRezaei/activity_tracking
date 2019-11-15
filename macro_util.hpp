#ifndef MACRO_UTIL_HPP
#define MACRO_UTIL_HPP

#include <cstring>
#include <tuple>

#ifdef _WIN32
	#define __SHORT_FILE_NAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
	#define __SHORT_FILE_NAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define NUMARGS(...) std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value

#endif // !MACRO_UTILITY_HPP