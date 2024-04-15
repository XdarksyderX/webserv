/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 10:43:19 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/15 20:54:04 by migarci2         ###   ########.fr       */
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

HTTPRequest	HTTPServer::receiveRequest(int clientSocketFD)
{
	char buffer[1024];
	int bytesRead;
	std::string requestString;
	HTTPRequest request;

	bytesRead = recv(clientSocketFD, buffer, 1024, 0);
	while (bytesRead > 0 && requestString.find("\r\n\r\n") == std::string::npos)
	{
        requestString.append(buffer, bytesRead);
        if (requestString.find("\r\n\r\n") != std::string::npos)
			break ;
        bytesRead = recv(clientSocketFD, buffer, sizeof(buffer), 0);
    }

	request = HTTPRequestParser::parseRequest(requestString);
	return request;
}

void HTTPServer::sendResponse(const HTTPResponse &response, int clientSocketFD)
{
    std::string responseString = HTTPResponseBuilder::buildResponse(response);
    const char* pData = responseString.c_str();
    size_t bytesToSend = responseString.length();
    ssize_t bytesSent;

    while (bytesToSend > 0)
    {
        bytesSent = send(clientSocketFD, pData, bytesToSend, 0);
        bytesToSend -= bytesSent;
        pData += bytesSent;
    }
}

HTTPResponse	HTTPServer::processRequest(const HTTPRequest &request)
{
	(void) request;
	HTTPResponse response;
	response.setHttpVersion("1.1");
	response.setStatusCode(200);
	response.setStatusMessage("OK");
	response.setBody("Hello, world!");
	response.addHeader("Content-Type", "text/plain");
	response.addHeader("Content-Length", Logger::to_string(response.getBody().length()));
	return response;
}

void	HTTPServer::acceptConnection()
{
	struct sockaddr_in clientAddress;
	socklen_t clientAddressLength = sizeof(clientAddress);
	int clientSocketFD = accept(socketFD, (struct sockaddr *)&clientAddress, &clientAddressLength);
	if (clientSocketFD < 0)
		throw AcceptError();

	HTTPRequest request = receiveRequest(clientSocketFD);
	HTTPResponse response = processRequest(request);

	std::string server = serverConfig.getHost()
					+ ":"
					+ Logger::to_string(serverConfig.getPort());
	sendResponse(response, clientSocketFD);
	Logger::logRequest(request, response, clientSocketFD, server, true);
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	connections[clientSocketFD] = currentTime;
	checkAndCloseInactiveConnections();
}

void HTTPServer::checkAndCloseInactiveConnections()
{
    static struct timeval lastTime;
    if (lastTime.tv_sec == 0 && lastTime.tv_usec == 0)
        gettimeofday(&lastTime, NULL); // Initialize lastTime on first call

    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);

    // Calculate elapsed time in microseconds
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

