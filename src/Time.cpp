/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Time.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 11:16:44 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/06 16:29:37 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Time.hpp"

std::string Time::getCurrentTime()
{
	time_t rawtime;
	struct tm *timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
	return std::string(buffer);
}

std::string Time::getHTTPFormatCurrentTime()
{
	time_t rawtime;
	struct tm *timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = gmtime(&rawtime);

	strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
	return std::string(buffer);
}
