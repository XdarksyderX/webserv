/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPCookie.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 22:39:16 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/21 00:06:02 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPCookie.hpp"

HTTPCookie::HTTPCookie()
{
    time_t now = std::time(NULL);
    void *p = static_cast<void*>(&now);
    unsigned long memptr = reinterpret_cast<unsigned long>(p);
    unsigned long seed = static_cast<unsigned long>(now) ^ memptr;

    std::srand(seed);
	time = now;
	key = std::rand();
}

HTTPCookie::HTTPCookie(const HTTPCookie &other)
{
	key = other.key;
	time = other.time;
}

HTTPCookie::~HTTPCookie() {}

HTTPCookie &HTTPCookie::operator=(const HTTPCookie &other)
{
	if (this != &other)
	{
		key = other.key;
		time = other.time;
	}
	return *this;
}

int	HTTPCookie::getKey() const
{
	return key;
}

time_t	HTTPCookie::getTime() const
{
	return time;
}

std::string HTTPCookie::serializeCookie(const HTTPCookie &cookie)
{
	std::ostringstream oss;
	oss.write(reinterpret_cast<const char*>(&cookie.key), sizeof(cookie.key));
	oss.write(reinterpret_cast<const char*>(&cookie.time), sizeof(cookie.time));
	return oss.str();
}

HTTPCookie HTTPCookie::deserializeCookie(const std::string &cookie)
{
	HTTPCookie result;
	std::istringstream iss(cookie);
	iss.read(reinterpret_cast<char*>(&result.key), sizeof(result.key));
	iss.read(reinterpret_cast<char*>(&result.time), sizeof(result.time));
	return result;
}
