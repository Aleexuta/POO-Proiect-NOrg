#pragma once
#include <regex>
#include <iostream>

enum class StrType
{
    Email,
    Username,
    Password
};
inline bool validateString(std::string str, StrType type)
{
	std::regex regex;
	switch (type)
	{
	case StrType::Email:
	{
		regex = ("([a-z0-9]+)[.-_]*([a-zA-Z0-9]+)@[a-z]+.[a-z]{2,4}");

	}
	break;
	case StrType::Username:
	{
		regex = ("[a-zA-Z.-_]{3,20}");
	}
	break;
	case StrType::Password:
	{
		regex = ("(? = .*[a - z])(? = .*[A - Z])(? = .*\d)(? = .*[@$!% *? &])[A - Za - z\d@$!% *? &]{ 8,20 }");
	}
	break;
	default:
		break;
	}

	if (std::regex_search(str, regex))
		return true;
	else
		return false;
}
