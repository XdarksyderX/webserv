/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponseBuilder.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erivero- <erivero-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 15:41:13 by migarci2          #+#    #+#             */
/*   Updated: 2024/07/17 13:29:17 by erivero-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponseBuilder.hpp"

const std::map<std::string, std::string> HTTPResponseBuilder::MIME_TYPES = HTTPResponseBuilder::createMIMEMap();
const std::map<int, std::string> HTTPResponseBuilder::STATUS_CODES = HTTPResponseBuilder::createStatusCodesMap();

void HTTPResponseBuilder::assembleStatusLine(std::string &assembleResponse, const HTTPResponse &response)
{
	assembleResponse += "HTTP/";
	assembleResponse += response.getHttpVersion();
	assembleResponse += " ";
	assembleResponse += Utils::to_string(response.getStatusCode());
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

std::string HTTPResponseBuilder::findMatchingLocation()
{
    std::string bestMatchKey = "";

    const std::map<std::string, LocationConfig>& locations = serverConfig.getLocations();
    if (locations.empty())
        return NULL;
    std::map<std::string, LocationConfig>::const_iterator it = locations.find("/");
    for (it = locations.begin(); it != locations.end(); ++it)
	{
        if (request.getUri().find(it->first) == 0 && it->first.length() > bestMatchKey.length())
            bestMatchKey = it->first;
    }

    return bestMatchKey;
}

void HTTPResponseBuilder::addCommonHeaders(const HTTPRequest &request, HTTPResponse &response)
{
	if (request.getHeader("Cookie") == "")
    {
        HTTPCookie cookie;
        std::string cookieHeader = "session=" + Utils::convertToBase64(HTTPCookie::serializeCookie(cookie)) + "; HttpOnly";
		response.addHeader("Set-Cookie", cookieHeader);
    }
	response.addHeader("Content-Length", Utils::to_string(response.getBody().length()));
	response.addHeader("Date", Time::getHTTPFormatCurrentTime());
	response.addHeader("Connection", "close");
	response.addHeader("Server", "webserv");
}

std::string HTTPResponseBuilder::simpleErrorPage(int errorCode)
{
	std::string response = "<!DOCTYPE html>\n<html>\n<head>\n<title>";
	response += Utils::to_string(errorCode);
	response += " ";
	response += STATUS_CODES.at(errorCode);
	response += "</title>\n</head>\n<body>\n<h1>";
	response += Utils::to_string(errorCode);
	response += " ";
	response += STATUS_CODES.at(errorCode);
	response += "</h1>\n</body>\n</html>\n";
	return response;
}

std::string HTTPResponseBuilder::getLocationUploadPath(const LocationConfig *location)
{
	std::string root = serverConfig.getUploadsDirectory();
	std::string uploadPath = location->getUploadPath();

	if (uploadPath.empty())
		return "";
	return Utils::joinPaths(root, uploadPath);
}

HTTPResponse HTTPResponseBuilder::handleErrorPage(int errorCode)
{
	HTTPResponse response;
	response.setStatusCode(errorCode);
	response.setStatusMessage(STATUS_CODES.at(errorCode));
	const std::map<int, std::string>& errorPages = serverConfig.getErrorPages();
	std::map<int, std::string>::const_iterator it = errorPages.find(errorCode);
	std::map<int, std::string>::const_iterator itEnd = errorPages.end();
	if (it != itEnd)
	{
		std::string filePath = Utils::joinPaths(serverConfig.getRoot(),
								serverConfig.getErrorPages().at(errorCode));
		response.setBody(Utils::getFileContent(filePath));
		response.addHeader("Content-Type", MIME_TYPES.at(Utils::getExtensionFromFile(filePath, false)));
	}
	else
	{
		response.setBody(simpleErrorPage(errorCode));
		response.addHeader("Content-Type", "text/html");
	}
	addCommonHeaders(request, response);
	return (response);
}

HTTPResponse HTTPResponseBuilder::handleGetRequest(const LocationConfig *location, CGIHandler &cgiHandler)
{
    HTTPResponse response;
    std::string root = serverConfig.getRoot();
    std::string resource = Utils::joinPaths(root, request.getUri());
    std::string indexedResource;
    std::string directory;
    resource = Utils::preventFileTraversal(resource);

    if (Utils::fileExists(resource))
    {
        if (cgiHandler.isCGI() && Utils::fileExists(resource))
        {
            try
            {
                response.setBody(cgiHandler.execCGI());
                response.addHeader("Content-Type", "text/html");
            }
            catch (const std::exception &e)
            {
                return handleErrorPage(500);
            }
        }
        else
            response.setBody(Utils::getFileContent(resource));
        try
        {
            if (response.getHeader("Content-Type") == "")
                response.addHeader("Content-Type", MIME_TYPES.at(Utils::getExtensionFromFile(resource, false)));
        }
        catch (const std::exception &e)
        {
            response.addHeader("Content-Type", "text/plain");
        }
    }
    else
    {

        indexedResource = Utils::joinPaths(resource, location->getIndex());
        if (Utils::fileExists(indexedResource))
        {
            response.setBody(Utils::getFileContent(indexedResource));
            try
            {
                response.addHeader("Content-Type", MIME_TYPES.at(Utils::getExtensionFromFile(indexedResource, false)));
            }
            catch (const std::exception &e)
            {
                response.addHeader("Content-Type", "text/plain");
            }
        }
        else
        {
			std::string allowedUploadURI = Utils::joinPaths("", "/" + location->getName() + "/" + location->getUploadPath());
            if (request.getUri().find(allowedUploadURI) != std::string::npos
                && location->getUploadPath() != "" && serverConfig.getUploadsDirectory() != "")
            {
                std::string uploadPath = getLocationUploadPath(location);
                resource = Utils::joinPaths(uploadPath, Utils::getNodeName(request.getUri()));
                if (Utils::fileExists(resource))
                {
                    response.setBody(Utils::getFileContent(resource));
                    try
                    {
                        response.addHeader("Content-Type", MIME_TYPES.at(Utils::getExtensionFromFile(resource, false)));
                    }
                    catch (const std::exception &e)
                    {
                        response.addHeader("Content-Type", "text/plain");
                    }
                    response.setStatusCode(200);
                    addCommonHeaders(request, response);
                    return response;
                }
                else if (Utils::directoryExists(resource))
                {
                    directory = resource;
                }
            }
            directory = Utils::joinPaths(root, request.getUri());
            if (!directory.empty() && location->getAutoindex() && Utils::directoryExists(directory))
            {
                response.setBody(Utils::createHTMLDirectoryListing(directory, request.getUri()));
                response.addHeader("Content-Type", "text/html");
            }
            else
            {
                return handleErrorPage(404);
            }
        }
    }
    response.setStatusCode(200);
    addCommonHeaders(request, response);
    return response;
}

HTTPResponse HTTPResponseBuilder::handlePostRequest(const LocationConfig *location)
{
    HTTPResponse response;
    std::string root = serverConfig.getUploadsDirectory();
    std::string uploadPath = location->getUploadPath();
    std::string resource = Utils::joinPaths(root, uploadPath);
    std::string fileName = Utils::getNodeName(request.getUri());
    std::string fullPath = Utils::joinPaths(resource, fileName);

    fullPath = Utils::preventFileTraversal(fullPath);

    std::string allowedUploadURI = Utils::joinPaths("/" + location->getName(), location->getUploadPath());
    std::string uriToCheck = request.getUri();
    std::string contentType = request.getHeader("Content-Type");
    if (contentType.find("multipart/form-data") != std::string::npos)
    {
        HTTPMultiFormData formData(request);
        std::vector<HTTPFormFile> files = formData.getFiles();
        for (size_t i = 0; i < files.size(); ++i)
        {
            std::string individualPath = Utils::joinPaths(resource, files[i].getFilename());
            individualPath = Utils::preventFileTraversal(individualPath);
            if (!Utils::createFile(individualPath, files[i].getContent()))
            {
                response.setStatusCode(500);
                response.setBody("Failed to create file.");
                response.addHeader("Content-Type", "text/plain");
                addCommonHeaders(request, response);
                return response;
            }
        }
        response.setStatusCode(201);
        response.setBody("Files created successfully.");
        response.addHeader("Content-Type", "text/plain");
        addCommonHeaders(request, response);
        return response;
    }
    if (uriToCheck.find(allowedUploadURI) == std::string::npos)
    {
        response.setStatusCode(403);
        response.setBody("Access denied: Cannot upload outside of the designated directory.");
        response.addHeader("Content-Type", "text/plain");
        addCommonHeaders(request, response);
        return response;
    }
    if (uriToCheck == allowedUploadURI || fileName.empty())
    {
        response.setStatusCode(400);
        response.setBody("No file name provided in the upload path.");
        response.addHeader("Content-Type", "text/plain");
        addCommonHeaders(request, response);
        return response;
    }
    if (Utils::createFile(fullPath, request.getBody()))
    {
        response.setStatusCode(201);
        response.setBody("File created successfully at " + URLEncoder::encode(fullPath));
        response.addHeader("Content-Type", "text/plain");
        response.addHeader("Location", URLEncoder::encode(fullPath));
        addCommonHeaders(request, response);
        return response;
    }
    else
    {
        response.setStatusCode(500);
        response.setBody("Failed to create file.");
        response.addHeader("Content-Type", "text/plain");
        addCommonHeaders(request, response);
        return response;
    }
}

HTTPResponse HTTPResponseBuilder::handleDeleteRequest(const LocationConfig *location)
{
	HTTPResponse response;
    std::string root = serverConfig.getUploadsDirectory();
    std::string uploadPath = location->getUploadPath();
    std::string resource = Utils::joinPaths(root, uploadPath);
    std::string fileName = Utils::getNodeName(request.getUri());
    std::string fullPath = Utils::joinPaths(resource, fileName);

    fullPath = Utils::preventFileTraversal(fullPath);

    std::string allowedUploadURI = Utils::joinPaths("/" + location->getName(), location->getUploadPath());
    std::string uriToCheck = request.getUri();
    if (uriToCheck.find(allowedUploadURI) == std::string::npos)
    {
        response.setStatusCode(403);
        response.setBody("Access denied: Cannot delete files outside of the designated directory.");
        response.addHeader("Content-Type", "text/plain");
        addCommonHeaders(request, response);
        return response;
    }
    if (uriToCheck == allowedUploadURI || fileName.empty())
    {
        response.setStatusCode(400);
        response.setBody("No file name provided in the path.");
        response.addHeader("Content-Type", "text/plain");
        addCommonHeaders(request, response);
        return response;
    }
    if (Utils::deleteFile(fullPath))
    {
        response.setStatusCode(200);
        response.setBody("File deleted successfully from " + URLEncoder::encode(fullPath));
        response.addHeader("Content-Type", "text/plain");
        response.addHeader("Location", URLEncoder::encode(fullPath));
    }
    else
    {
        response.setStatusCode(500);
        response.setBody("Failed to delete file. It may not exist or there is a permission error.");
        response.addHeader("Content-Type", "text/plain");
    }
    addCommonHeaders(request, response);
    return response;
}

HTTPResponse HTTPResponseBuilder::buildResponse()
{
	if (request.getHeader("Content-Length") != "")
	{
		size_t contentLength = static_cast<size_t>(std::atoi(request.getHeader("Content-Length").c_str()));
		if (contentLength > serverConfig.getClientMaxBodySize() 
			|| contentLength != request.getBody().length())
        {
			return handleErrorPage(413);
        }
	}
	LocationConfig location;
	std::string locationName = findMatchingLocation();
	if (locationName.empty())
	{
		const std::map<std::string, LocationConfig>& locations = serverConfig.getLocations();
		std::map<std::string, LocationConfig>::const_iterator it = locations.find("/");
		std::map<std::string, LocationConfig>::const_iterator itEnd = locations.end();
		if (it == itEnd)
			return handleErrorPage(404);
		else
			location = it->second;
    }
	else
		location = serverConfig.getLocations().at(locationName);
	if (!Utils::hasElement(location.getAllowMethods(), request.getMethod()))
		return handleErrorPage(405);
	CGIHandler  cgi_handler(location, request, serverConfig.getRoot());
	if (request.getMethod() == GET)
		return handleGetRequest(&location, cgi_handler);
    else if (request.getMethod() == POST && cgi_handler.isCGI())
		return handleGetRequest(&location, cgi_handler);
	else if ((request.getMethod() == POST || request.getMethod() == PUT) &&
		location.getUploadPath() != "" && serverConfig.getUploadsDirectory() != "")
		return handlePostRequest(&location);
	else if (request.getMethod() == DELETE)
		return handleDeleteRequest(&location);
	return handleErrorPage(405);
}
