/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 16:05:43 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/11 23:33:26 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_MANAGER_HPP
# define SERVER_MANAGER_HPP

# include <vector>
# include <exception>
# include <poll.h>
# include <cerrno>

# include "HTTPServer.hpp"

class ServerManager
{
	private:
		std::vector<HTTPServer *> servers; ///< List of HTTP servers pointers managed by the manager.
		std::vector<struct pollfd> pollFDs; ///< List of poll file descriptors for the servers.
		bool running; ///< Flag indicating if the servers are running.

		/**
		 * @brief Updates the poll file descriptors based on the servers.
		 */
		void updatePollFDs();
	
	public:
		ServerManager();
		ServerManager(const std::vector<HTTPServer *> &servers);
		ServerManager(const ServerManager &other);
		
		ServerManager &operator=(const ServerManager &other);

		~ServerManager();

		/**
		 * @brief Adds an HTTP server to the manager.
		 * @param server The HTTP server pointer to add.
		 */
		void addServer(HTTPServer *server);

		/**
		 * @brief Starts all HTTP servers managed by the manager.
		 */
		void startServers();

		std::vector<HTTPServer *> getServers() const;

		/**
		 * @brief Stops all HTTP servers managed by the manager.
		 */
		void stopServers();

		/**
		 * @class PollError
		 * @brief Exception thrown when an error occurs during polling.
		 */
		class PollError : public std::exception
		{
			const char	*what() const throw();
		};
};

#endif