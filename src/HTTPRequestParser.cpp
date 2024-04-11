/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 14:45:31 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/11 20:58:30 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequestParser.hpp"
#include <iostream>

void	HTTPRequestParser::parseRequestLine(HTTPRequest &request, const std::string &requestLine)
{
	std::string::size_type start = 0;
	std::string::size_type end = requestLine.find(' ');

	if (end == std::string::npos)
		throw InvalidRequestHeader();

	request.setMethod(HTTPRequest::getMethodFromString(requestLine.substr(start, end - start)));
	start = end + 1;
	end = requestLine.find(' ', start);
	if (end == std::string::npos)
		throw InvalidRequestHeader();
	request.setUri(requestLine.substr(start, end - start));
	start = end + 1;
	request.setHttpVersion(requestLine.substr(start));
}

void	HTTPRequestParser::parseRequestHeaders(HTTPRequest &request, const std::string &headerLine)
{
	std::string::size_type end = headerLine.find(':');
	if (end == std::string::npos)
		throw InvalidRequestHeader();
	std::string key = headerLine.substr(0, end);
	std::string value = headerLine.substr(end + 2);
	request.getHeaders()[key] = value;
}

HTTPRequest	HTTPRequestParser::parseRequest(const std::string &rawRequest)
{
	HTTPRequest request;
	std::string::size_type start = 0;
	std::string::size_type end = rawRequest.find("\r\n");

	if (end == std::string::npos)
		throw InvalidRequestLine();
	parseRequestLine(request, rawRequest.substr(start, end - start));
	start = end + 2;
	end = rawRequest.find("\r\n", start);
	while (end != std::string::npos && start < end)
	{
		if (start == end)
			break;
		parseRequestHeaders(request, rawRequest.substr(start, end - start));
		start = end + 2;
		end = rawRequest.find("\r\n", start);
	}
	if (start < rawRequest.size())
		request.setBody(rawRequest.substr(start + 2));
	return request;
}

const char *HTTPRequestParser::InvalidRequestLine::what() const throw()
{
	return "Invalid request line";
}

const char *HTTPRequestParser::InvalidRequestHeader::what() const throw()
{
	return "Invalid request header";
}
