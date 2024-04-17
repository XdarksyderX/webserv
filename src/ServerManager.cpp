/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 16:21:28 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/17 18:11:38 by migarci2         ###   ########.fr       */
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
	updatePollFDs();
}

ServerManager::ServerManager(const ServerManager &other)
	: servers(other.servers)
{
	updatePollFDs();
}

ServerManager &ServerManager::operator=(const ServerManager &other)
{
	if (this != &other)
	{
		servers = other.servers;
		running = false;
		updatePollFDs();
	}
	return *this;
}

ServerManager::~ServerManager() {}

void ServerManager::updatePollFDs()
{
	pollFDs.resize(servers.size());
	for (size_t i = 0; i < servers.size(); i++)
	{
		pollFDs[i].fd = servers[i]->getSocketFD();
		pollFDs[i].events = POLLIN;
	}
}

void ServerManager::addServer(HTTPServer *server)
{
	servers.push_back(server);
	updatePollFDs();
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
		if (poll(pollFDs.data(), pollFDs.size(), -1) < 0)
			continue;
		for (size_t i = 0; i < pollFDs.size(); i++)
		{
			if (pollFDs[i].revents & POLLIN)
			try
			{
				servers[i]->acceptConnection();
			}
			catch (std::exception &e)
			{
				Logger::log(e.what(), Logger::ERROR, true);
			}
		}
	}
}

void	ServerManager::stopServers()
{
	running = false;
}

const char	*ServerManager::PollError::what() const throw()
{
	return "Poll error";
}

