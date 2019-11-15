#pragma once
#ifndef ENUM_UTIL_HPP
#define ENUM_UTIL_HPP

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <regex>
#include <algorithm>
#include <type_traits>
#include "macro_util.hpp"

namespace internal_use
{
    static std::vector<std::string> get_arguments(const std::string& source)
    {
        using iterator = std::regex_token_iterator<std::string::const_iterator>;
		std::vector<std::string> tokens;
		std::regex reg("[, \n]");
		std::transform(
			iterator(source.begin(), source.end(), reg, -1),
			iterator(),
			std::back_inserter(tokens),
			[](auto m) { return std::string(m.first, m.second); }
		);

		auto new_end = std::remove_if(tokens.begin(), tokens.end(), [](std::string const& str) { return (str.empty() ? true : false); });
		tokens.erase(new_end, tokens.end());

		return std::move(tokens);	
    }
}

/*This declares a zero-based enum with default type int*/
#define DECLARE_ENUM(enam_name, ...)  \
	enum enam_name { __VA_ARGS__, MAX_NUMBER_OF_##enam_name = NUMARGS(__VA_ARGS__)}; \
	static const std::vector<std::string> enam_name##ToStr = internal_use::get_arguments(#__VA_ARGS__); \

/*This declares a zero-based enum with type*/
#define DECLARE_TYPED_ENUM(enam_name, enum_type, ...)  \
	enum enam_name : enum_type { __VA_ARGS__ , MAX_NUMBER_OF_##enam_name = NUMARGS(__VA_ARGS__)}; \
	static const std::vector<std::string> enam_name##ToStr = internal_use::get_arguments(#__VA_ARGS__); \

/*This declares a zero-based enum class with type*/
#define DECLARE_TYPED_ENUM_CLASS(enam_name, enum_type, ...)  \
	enum class enam_name : enum_type { __VA_ARGS__ , MAX_NUMBER_OF_##enam_name = NUMARGS(__VA_ARGS__)}; \
	static const std::vector<std::string> enam_name##ToStr = internal_use::get_arguments(#__VA_ARGS__); \


#endif