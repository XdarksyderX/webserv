/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 10:43:19 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/21 16:39:54 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPServer.hpp"

HTTPServer::HTTPServer(ServerConfig &serverConfig) : serverConfig(serverConfig)
{
	initializeServerSocket();
}

HTTPServer::~HTTPServer()
{
	std::map<int, struct timeval>::iterator it = connections.begin();
	while (it != connections.end())
	{
        std::map<int, struct timeval>::iterator current_it = it++;
		close(current_it->first);
		connections.erase(current_it);
    }	
	close(socketFD);
}

void HTTPServer::initializeServerSocket()
{
    struct sockaddr_in serverAddress;

    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD < 0)
        throw SocketError();

    int opt = 1;
    if (setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
        throw SocketOptionError();

    int flags = fcntl(socketFD, F_GETFL, 0);
    if (flags < 0)
        throw SocketOptionError();
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(serverConfig.getHost().c_str());
    serverAddress.sin_port = htons(serverConfig.getPort());

    if (bind(socketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
        throw BindError();

    if (listen(socketFD, 5) < 0)
        throw ListenError();
}

int	HTTPServer::getSocketFD() const
{
	return socketFD;
}

HTTPRequest HTTPServer::receiveRequest(int clientSocketFD)
{
    const size_t buffer_size = 1024;
    char buffer[buffer_size];
    int bytesRead;
    std::string requestString;
    HTTPRequest request;

    while ((bytesRead = recv(clientSocketFD, buffer, buffer_size, 0)) > 0)
    {
        requestString.append(buffer, bytesRead);
        if (requestString.find("\r\n\r\n") != std::string::npos)
        {
            size_t contentLengthPos = requestString.find("Content-Length: ");
            if (contentLengthPos != std::string::npos)
            {
                size_t start = contentLengthPos + 16;
                size_t end = requestString.find("\r\n", contentLengthPos);
                int contentLength = atoi(requestString.substr(start, end - start).c_str());
                size_t headersEnd = requestString.find("\r\n\r\n") + 4;
                int bodyLength = requestString.length() - headersEnd;
                while (bodyLength < contentLength)
                {
                    bytesRead = recv(clientSocketFD, buffer, buffer_size, 0);
                    if (bytesRead < 0)
                    {
                        close(clientSocketFD);
                        throw SocketError();
                    }
                    requestString.append(buffer, bytesRead);
                    bodyLength += bytesRead;
                }
            }
            break;
        }
    }
    if (bytesRead < 0)
    {
        close(clientSocketFD);
        throw SocketError();
    }
    request = HTTPRequestParser::parseRequest(requestString);
    return request;
}

void HTTPServer::sendResponse(const HTTPResponse &response, int clientSocketFD)
{
    std::string responseString = HTTPResponseBuilder::assembleResponse(response);
    const char* pData = responseString.c_str();
    size_t bytesToSend = responseString.length();
    ssize_t bytesSent;

    while (bytesToSend > 0)
    {
        bytesSent = send(clientSocketFD, pData, bytesToSend, 0);
        if (bytesSent < 0)
        {
            close(clientSocketFD);
            throw SocketError();
        }
        bytesToSend -= bytesSent;
        pData += bytesSent;
    }
}

HTTPResponse	HTTPServer::processRequest(const HTTPRequest &request)
{
	HTTPResponseBuilder responseBuilder(serverConfig, request);
	return responseBuilder.buildResponse();
}

int	HTTPServer::acceptConnection()
{
	struct sockaddr_in clientAddress;
	socklen_t clientAddressLength = sizeof(clientAddress);
	int clientSocketFD = accept(socketFD, (struct sockaddr *)&clientAddress, &clientAddressLength);
	if (clientSocketFD < 0)
		throw AcceptError();

	HTTPRequest request = receiveRequest(clientSocketFD);
    std::string host = serverConfig.getHost() + ":" + Utils::to_string(serverConfig.getPort());
    if (request.getHeader("Host").empty() || request.getHeader("Host") != host)
    {
        if (serverConfig.getHost().substr(0, 3) != "127"
            && request.getHeader("Host") != "localhost:" + Utils::to_string(serverConfig.getPort()))
        {
            close(clientSocketFD);
            return -1;
        }
    }
    updateClientActivity(clientSocketFD);
	HTTPResponse response = processRequest(request);
    responsesToSend[clientSocketFD].push(std::make_pair(request, response));
    return clientSocketFD;
}

void    HTTPServer::sendFirstPendingResponse(int clientSocketFD)
{
    if (responsesToSend.find(clientSocketFD) == responsesToSend.end())
        return;
    std::queue<HTTPRequestResponsePair> &clientResponsesToSend = this->responsesToSend[clientSocketFD];
    if (!responsesToSend.empty())
    {    
        HTTPRequestResponsePair connection = clientResponsesToSend.front();
        HTTPRequest request = connection.first;
        HTTPResponse response = connection.second;
        sendResponse(response, clientSocketFD);
        
        std::string server = serverConfig.getHost()
                    + ":"
                    + Utils::to_string(serverConfig.getPort());
        Logger::logRequest(request, response, clientSocketFD, server, true);
        clientResponsesToSend.pop();
        updateClientActivity(clientSocketFD);
        checkAndCloseInactiveConnections();
        if (clientResponsesToSend.empty())
            responsesToSend.erase(clientSocketFD);
    }
}

void    HTTPServer::updateClientActivity(int clientSocketFD)
{
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    connections[clientSocketFD] = currentTime;
}

void HTTPServer::checkAndCloseInactiveConnections()
{
    static struct timeval lastTime;
    if (lastTime.tv_sec == 0 && lastTime.tv_usec == 0)
        gettimeofday(&lastTime, NULL);

    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    long elapsedTime = (currentTime.tv_sec - lastTime.tv_sec) * 1000000L 
                        + (currentTime.tv_usec - lastTime.tv_usec);
    if (elapsedTime < CONNECTION_TIMEOUT)
        return;
    for (std::map<int, struct timeval>::iterator it = connections.begin(); it != connections.end();)
    {
        std::map<int, struct timeval>::iterator current_it = it++;
        long connectionAge = (currentTime.tv_sec - current_it->second.tv_sec) * 1000000L
                            + (currentTime.tv_usec - current_it->second.tv_usec);
        if (connectionAge > CONNECTION_TIMEOUT)
        {
            close(current_it->first);
            connections.erase(current_it);
        }
    }
    lastTime = currentTime;
}

bool    HTTPServer::isClient(int clientSocketFD) const
{
    std::map<int, struct timeval>::const_iterator it = connections.find(clientSocketFD);
    std::map<int, struct timeval>::const_iterator end = connections.end();
    return it != end;
}

const char	*HTTPServer::SocketError::what() const throw()
{
	return "Error creating socket";
}

const char	*HTTPServer::SocketOptionError::what() const throw()
{
	return "Error setting socket options";
}

const char	*HTTPServer::BindError::what() const throw()
{
	return "Error binding socket";
}

const char	*HTTPServer::AcceptError::what() const throw()
{
	return "Error accepting connection";
}

const char	*HTTPServer::ListenError::what() const throw()
{
	return "Error listening on socket";
}

