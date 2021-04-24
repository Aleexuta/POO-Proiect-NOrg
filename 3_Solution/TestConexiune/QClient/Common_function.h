#pragma once
#include <regex>
#include <iostream>
#include <fstream>
#include <string>
#define NRCOL 1
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
			regex = ("^.*(?=.{10,})(?=.*\d)(?=.*[a-z])(?=.*[A-Z])(?=.*[@#$%^&+=]).*$");
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
	inline std::string cript(std::string value, std::string key)
	{
		std::string tmp(key);
		while (key.size() < value.size())
			key += tmp;

		// And now for the encryption part
		for (std::string::size_type i = 0; i < value.size(); ++i)
			value[i] += key[i];
		return value;
	}
	inline std::string decript(std::string value, std::string key)
	{
		std::string tmp(key);
		while (key.size() < value.size())
			key += tmp;

		// And now for the encryption part
		for (std::string::size_type i = 0; i < value.size(); ++i)
			value[i] -= key[i];
		return value;
	}
	inline void createLoginFile(std::string email, std::string password)
	{
		//creaza un fisier cu email, password
		std::fstream f;
		f.open("automatLogin.txt", std::ios::out);
		f.clear();
		email = cript(email, "monkey");
		password = cript(password, "monkey");
		f << email << std::endl << password;
		f.close();

		//cripteaza fisierul

	}
	inline bool isAlreadyLogged()
	{
		//return true daca exista fiserul 
		std::fstream f;
		f.open("automatLogin.txt", std::ios::in);
		if (f.peek() == std::ifstream::traits_type::eof())
			return false;
		return true;
	}
	inline void LoginAutomatically()
	{
		//deschide fisierul si alege emailul si passwordul de acolo
		std::fstream f;
		f.open("automatLogin.txt", std::ios::in);
		std::string str = "{\"email\":\"";
		std::string x;
		f >> x;
		x = decript(x, "monkey");
		str += x;
		str += "\",\"password\":\"";
		f >> x;
		x = decript(x, "monkey");
		str += x;
		str += "\"}";
		QClient* main = QClient::getInstance();
		main->sendLoginMessage(str);
		main->IncomingMessages();
	}