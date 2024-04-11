/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 16:21:28 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/11 19:30:24 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerManager.hpp"

ServerManager::ServerManager() {}

ServerManager::ServerManager(const std::vector<HTTPServer *> &servers)
	: servers(servers)
{
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

void ServerManager::startServers()
{
	while (true)
	{
		if (poll(pollFDs.data(), pollFDs.size(), -1) < 0)
			throw PollError();
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

const char	*ServerManager::PollError::what() const throw()
{
	return "Poll error";
}

