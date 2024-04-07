/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 10:43:19 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/07 13:40:40 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPServer.hpp"

HTTPServer::HTTPServer(ServerConfig &serverConfig) : serverConfig(serverConfig)
{
	initializeServerSocket();
}

HTTPServer::~HTTPServer()
{
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

void	HTTPServer::listenForConnections()
{
	std::vector<struct pollfd> FDs;
	struct sockaddr_in clientAddress;
	socklen_t clientAddressLength = sizeof(clientAddress);
	int newSD;

	struct pollfd serverFD;
	serverFD.fd = socketFD;
	serverFD.events = POLLIN;
	FDs.push_back(serverFD);
	while (true)
	{
		if (poll(FDs.data(), FDs.size(), -1) < 0)
			throw ListenError();
		checkAndCloseInactiveConnections();
		for (size_t i = 0; i < FDs.size(); ++i)
		{
			if (FDs[i].revents & POLLIN)
			{
				if (FDs[i].fd == socketFD)
				{
					newSD = accept(socketFD, (struct sockaddr *)&clientAddress, &clientAddressLength);
					if (newSD < 0)
						throw AcceptError();
					FDs.push_back({newSD, POLLIN, 0});
					connections[newSD] = time(NULL);
				}
				else
				{
					acceptConnection(FDs[i].fd);
					connections[FDs[i].fd] = time(NULL);
				}
			}
		}
	}
}

HTTPRequest	HTTPServer::receiveRequest(int clientSocketFD)
{
	char buffer[1024];
	int bytesRead;
	std::string requestString;
	HTTPRequest request;

	while ((bytesRead = recv(clientSocketFD, buffer, sizeof(buffer), 0)) > 0)
		requestString.append(buffer, bytesRead);
	request = HTTPRequestParser::parseRequest(requestString);
	return request;
}

HTTPResponse	HTTPServer::processRequest(const HTTPRequest &request)
{
	
}

void	HTTPServer::acceptConnection(int clientSocketFD)
{
	
}

void	HTTPServer::checkAndCloseInactiveConnections()
{
	time_t currentTime = time(NULL);
	std::map<int, time_t>::iterator it;
	for (it = connections.begin(); it != connections.end();)
	{
		if (currentTime - it->second > CONNECTION_TIMEOUT)
		{
			close(it->first);
			it = connections.erase(it);
		}
		else
			++it;
	}
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

