/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 16:21:28 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/20 00:05:46 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerManager.hpp"

ServerManager::ServerManager()
{
	running = false;
}

ServerManager::ServerManager(const std::vector<HTTPServer *> &servers)
	: servers(servers)
{
	running = false;
}

ServerManager::ServerManager(const ServerManager &other)
	: servers(other.servers)
{
	running = false;
}

ServerManager &ServerManager::operator=(const ServerManager &other)
{
	if (this != &other)
	{
		servers = other.servers;
		running = false;
	}
	return *this;
}

ServerManager::~ServerManager() {}

void ServerManager::updatePollFDs()
{
	for (size_t i = pollFDs.size() - 1; i >= servers.size(); i--)
	{
		if (!Utils::isValidSocket(pollFDs[i].fd) || !findClientServer(pollFDs[i].fd))
			pollFDs.erase(pollFDs.begin() + i);
	}
}

void ServerManager::addServer(HTTPServer *server)
{
	servers.push_back(server);
	struct pollfd pollFD;
	pollFD.fd = server->getSocketFD();
	pollFD.events = POLLIN;
	pollFDs.push_back(pollFD);
}

void ServerManager::addClient(int socketFD)
{
	struct pollfd pollFD;
	pollFD.fd = socketFD;
	pollFD.events = POLLOUT;
	pollFDs.push_back(pollFD);
}

struct pollfd	ServerManager::findPollFD(int socketFD)
{
	for (size_t i = 0; i < pollFDs.size(); i++)
	{
		if (pollFDs[i].fd == socketFD)
			return pollFDs[i];
	}
	throw  SocketNotFound();
}

std::vector<HTTPServer *> ServerManager::getServers() const
{
	return servers;
}

void ServerManager::startServers()
{
	running = true;
	while (running)
	{
		updatePollFDs();
		int numEvents = poll(pollFDs.data(), pollFDs.size(), 1000);
		if (numEvents < 0 && errno != EINTR)
		{
			throw PollError();
		}
		else if (numEvents == 0)
		{
			continue;
		}

		for (size_t i = 0; i < pollFDs.size(); i++)
		{
			try
			{
				if (pollFDs[i].revents & POLLIN)
				{
					int clientFD = servers[i]->acceptConnection();
					if (clientFD >= 0)
						addClient(clientFD);
				}
				if (findPollFD(pollFDs[i].fd).revents & POLLOUT)
				{
					HTTPServer *server = findClientServer(pollFDs[i].fd);
					if (server == NULL)
						throw ServerNotFound();
					server->sendFirstPendingResponse(pollFDs[i].fd);
				}
			}
			catch (std::exception &e)
			{
				Logger::log(e.what(), Logger::ERROR, true);
			}
		}
	}
}

HTTPServer	*ServerManager::findClientServer(int clientSocketFD)
{
	for (size_t i = 0; i < servers.size(); i++)
	{
		if (servers[i]->isClient(clientSocketFD))
			return servers[i];
	}
	return NULL;
}

void	ServerManager::stopServers()
{
	running = false;
}

const char	*ServerManager::PollError::what() const throw()
{
	return "Poll error";
}

const char *ServerManager::SocketNotFound::what() const throw()
{
	return "Socket not found";
}

const char *ServerManager::ServerNotFound::what() const throw()
{
	return "Server not found";
}

