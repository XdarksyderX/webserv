/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 14:45:31 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/21 16:21:11 by migarci2         ###   ########.fr       */
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
	request.addHeader(key, value);
}

HTTPRequest HTTPRequestParser::parseRequest(const std::string &rawRequest)
{
    HTTPRequest request;
    std::string::size_type headerEnd = rawRequest.find("\r\n\r\n");
    if (headerEnd == std::string::npos)
        throw InvalidRequestLine();

    std::string headersPart = rawRequest.substr(0, headerEnd);
    std::string::size_type start = 0;
    std::string::size_type end = headersPart.find("\r\n");

    parseRequestLine(request, headersPart.substr(start, end - start));
    start = end + 2;
    while (start < headerEnd)
	{
        end = headersPart.find("\r\n", start);
        if (end == std::string::npos)
            end = headerEnd;
        if (start == end)
            break;
        parseRequestHeaders(request, headersPart.substr(start, end - start));
        start = end + 2;
    }
    if (headerEnd + 4 < rawRequest.size())
        request.setBody(rawRequest.substr(headerEnd + 4));
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
