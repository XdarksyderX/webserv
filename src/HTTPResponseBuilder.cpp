/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponseBuilder.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 15:41:13 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/16 19:37:49 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponseBuilder.hpp"

const std::map<std::string, std::string> HTTPResponseBuilder::MIME_TYPES = HTTPResponseBuilder::createMIMEMap();
const std::map<int, std::string> HTTPResponseBuilder::STATUS_CODES = HTTPResponseBuilder::createStatusCodesMap();

void	HTTPResponseBuilder::assembleStatusLine(std::string &assembleResponse, const HTTPResponse &response)
{
	assembleResponse += "HTTP/";
	assembleResponse += response.getHttpVersion();
	assembleResponse += " ";
	assembleResponse += Logger::to_string(response.getStatusCode());
	assembleResponse += " ";
	assembleResponse += response.getStatusMessage();
	assembleResponse += "\r\n";
}

void HTTPResponseBuilder::assembleHeaders(std::string &assembleResponse, const HTTPResponse &response)
{
    const std::map<std::string, std::string> &headers = response.getHeaders();

    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
	{
        if (!it->first.empty() && !it->second.empty())
		{
			assembleResponse += it->first;
			assembleResponse += ": ";
			assembleResponse += it->second;
			assembleResponse += "\r\n";
		}
    }
}

std::string HTTPResponseBuilder::assembleResponse(const HTTPResponse &response)
{
	std::string assembleResponse;

	assembleStatusLine(assembleResponse, response);
	assembleHeaders(assembleResponse, response);
	assembleResponse += "\r\n";
	assembleResponse += response.getBody();
	return assembleResponse;
}

HTTPResponseBuilder::HTTPResponseBuilder(const ServerConfig &sC, const HTTPRequest &r) : serverConfig(sC), request(r)
{
	
}

const LocationConfig *HTTPResponseBuilder::findMatchingLocation()
{
    std::string bestMatchKey = "";
    const LocationConfig *bestMatch = NULL;

    const std::map<std::string, LocationConfig>& locations = serverConfig.getLocations();
    if (locations.empty())
        return NULL;
    std::map<std::string, LocationConfig>::const_iterator it;
    for (it = locations.begin(); it != locations.end(); ++it)
	{
        if (request.getUri().find(it->first) == 0 && it->first.length() > bestMatchKey.length())
		{
            bestMatchKey = it->first;
            bestMatch = &(it->second);
        }
    }
    return bestMatch;
}

std::string		HTTPResponseBuilder::simpleErrorPage(int errorCode)
{
	std::string response = "<!DOCTYPE html>\n<html>\n<head>\n<title>";
	response += Logger::to_string(errorCode);
	response += " ";
	response += STATUS_CODES.at(errorCode);
	response += "</title>\n</head>\n<body>\n<h1>";
	response += Logger::to_string(errorCode);
	response += " ";
	response += STATUS_CODES.at(errorCode);
	response += "</h1>\n</body>\n</html>\n";
	return response;
}

HTTPResponse	HTTPResponseBuilder::handleErrorPage(int errorCode)
{
	HTTPResponse response;
	response.setStatusCode(errorCode);
	response.setStatusMessage(STATUS_CODES.at(errorCode));
	std::map<int, std::string>::const_iterator it = serverConfig.getErrorPages().find(errorCode);
	std::map<int, std::string>::const_iterator itEnd = serverConfig.getErrorPages().end();
	if (it != itEnd)
	{
		std::string filePath = Utils::joinPaths(serverConfig.getRoot(),
								serverConfig.getErrorPages().at(errorCode));
		response.setBody(Utils::getFileContent(filePath));
		response.addHeader("Content-Type", MIME_TYPES.at(Utils::getExtensionFromFile(filePath)));
	}
	else
	{
		response.setBody(simpleErrorPage(errorCode));
		response.addHeader("Content-Type", "text/html");
	}
	response.addHeader("Content-Length", Logger::to_string(response.getBody().length()));
	response.addHeader("Date", Time::getHTTPFormatCurrentTime());
	response.addHeader("Server", "webserv");
	return response;
}

HTTPResponse HTTPResponseBuilder::handleDefaultRequest() {
    HTTPResponse response;
    if (request.getMethod() != GET)
        return handleErrorPage(405);

    std::string filePath = Utils::joinPaths(serverConfig.getRoot(), request.getUri());
    if (Utils::directoryExists(filePath))
        filePath = Utils::joinPaths(filePath, serverConfig.getIndex());

    if (!Utils::fileExists(filePath))
        return handleErrorPage(404);

    response.setStatusCode(200);
    response.setStatusMessage("OK");
    response.setBody(Utils::getFileContent(filePath));
    response.addHeader("Content-Type", MIME_TYPES.at(Utils::getExtensionFromFile(filePath)));
    response.addHeader("Content-Length", Logger::to_string(response.getBody().length()));  // Use std::to_string for standard C++
    response.addHeader("Date", Time::getHTTPFormatCurrentTime());
    response.addHeader("Server", "webserv");
    return response;
}


HTTPResponse	HTTPResponseBuilder::buildResponse()
{
	const LocationConfig *location = findMatchingLocation();
	if (location)
		(void) location;//return handleLocationRequest(*location);
	return handleDefaultRequest();
}
