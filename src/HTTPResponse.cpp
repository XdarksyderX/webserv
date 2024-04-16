/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 16:15:46 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/15 16:07:55 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse()
    : httpVersion("1.1"), statusCode(0), statusMessage(""), body("") {}

HTTPResponse::HTTPResponse(const HTTPResponse &other)
    : httpVersion(other.httpVersion), statusCode(other.statusCode), 
    statusMessage(other.statusMessage), body(other.body), headers(other.headers) {}


HTTPResponse::~HTTPResponse() {}

HTTPResponse &HTTPResponse::operator=(const HTTPResponse &other)
{
    if (this != &other)
    {
        statusCode = other.statusCode;
        statusMessage = other.statusMessage;
        httpVersion = other.httpVersion;
        body = other.body;
        headers = other.headers;
    }
    return *this;
}

int HTTPResponse::getStatusCode() const
{
    return statusCode;
}

std::string HTTPResponse::getStatusMessage() const
{
    return statusMessage;
}

std::string HTTPResponse::getHttpVersion() const
{
    return httpVersion;
}

std::string HTTPResponse::getBody() const
{
    return body;
}

std::map<std::string, std::string> HTTPResponse::getHeaders() const
{
    return headers;
}

void HTTPResponse::setHttpVersion(const std::string &version)
{
    httpVersion = version;
}

void HTTPResponse::setStatusCode(int code)
{
    statusCode = code;
}

void HTTPResponse::setStatusMessage(const std::string &message)
{
    statusMessage = message;
}

void HTTPResponse::setBody(const std::string &b)
{
    body = b;
}

void HTTPResponse::addHeader(const std::string &key, const std::string &value)
{
    headers[key] = value;
}

std::string HTTPResponse::getHeader(const std::string &key) const
{
    std::map<std::string, std::string>::const_iterator it = headers.find(key);
    return it != headers.end() ? it->second : "";
}

void HTTPResponse::clear()
{
    statusCode = 0;
    statusMessage = "NULL";
    httpVersion = "1.1";
    body.clear();
    headers.clear();
}
