/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erivero- <erivero-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 16:15:51 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/23 15:32:49 by erivero-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"

HTTPRequest::HTTPRequest() : method(NONE), uri(""), query(""), httpVersion("1.1"), body("") {}

HTTPRequest::HTTPRequest(const HTTPRequest &other)
    : method(other.method), uri(other.uri), httpVersion(other.httpVersion), body(other.body), headers(other.headers) {}

HTTPRequest::~HTTPRequest() {}

HTTPRequest &HTTPRequest::operator=(const HTTPRequest &other)
{
    if (this != &other)
    {
        method = other.method;
        uri = other.uri;
        httpVersion = other.httpVersion;
        body = other.body;
        headers = other.headers;
    }
    return *this;
}

HTTPMethod HTTPRequest::getMethod() const
{
    return method;
}

std::string HTTPRequest::getUri() const
{
    return uri;
}

std::string HTTPRequest::getHttpVersion() const
{
    return httpVersion;
}

std::string HTTPRequest::getBody() const
{
    return body;
}

const std::map<std::string, std::string> &HTTPRequest::getHeaders() const
{
    return headers;
}

std::map<std::string, std::string> &HTTPRequest::getHeaders()
{
    return headers;
}

void HTTPRequest::setMethod(HTTPMethod m)
{
    method = m;
}

void HTTPRequest::setUri(const std::string &u)
{
    uri = u;
}

void HTTPRequest::setQuery(const std::string &q)
{
    query = q;
}

void HTTPRequest::setHttpVersion(const std::string &version)
{
    httpVersion = version;
}

void HTTPRequest::setBody(const std::string &b)
{
    body = b;
}

void HTTPRequest::addHeader(const std::string &key, const std::string &value)
{
    headers[key] = value;
}

std::string HTTPRequest::getHeader(const std::string &key) const
{
    std::map<std::string, std::string>::const_iterator it = headers.find(key);
    return it != headers.end() ? it->second : "";
}

void HTTPRequest::clear()
{
    method = NONE;
    uri.clear();
    httpVersion.clear();
    body.clear();
    headers.clear();
}

HTTPMethod  HTTPRequest::getMethodFromString(const std::string &method)
{
    if (method == "GET")
        return GET;
    else if (method == "POST")
        return POST;
    else if (method == "PUT")
        return PUT;
    else if (method == "DELETE")
        return DELETE;
    else
        return NONE;
}

std::string HTTPRequest::getMethodString(HTTPMethod method)
{
    if (method == GET)
        return "GET";
    else if (method == POST)
        return "POST";
    else if (method == PUT)
        return "PUT";
    else if (method == DELETE)
        return "DELETE";
    else
        return "UNKNOWN";
}
