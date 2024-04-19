/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 16:05:43 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/20 00:12:41 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_MANAGER_HPP
# define SERVER_MANAGER_HPP

# include <vector>
# include <exception>
# include <poll.h>
# include <cerrno>

# include "HTTPServer.hpp"
# include "utils.hpp"

/**
 * @class ServerManager
 * @brief Class that manages multiple HTTP servers.
 *
 * This class is designed to manage multiple HTTP servers, allowing to start and stop them all at once.
 * It also provides functionalities to add and remove client socket file descriptors to the servers.
 * The class uses the poll system call to handle multiple servers and clients I/O operations simultaneously.
 */
class ServerManager
{
	private:
		std::vector<HTTPServer *> servers; ///< List of HTTP servers pointers managed by the manager.
		std::vector<struct pollfd> pollFDs; ///< List of poll file descriptors for the servers and clients.
		bool running; ///< Flag indicating if the servers are running.

		/**
		 * @brief Updates the poll file descriptors based on the servers.
		 */
		void updatePollFDs();

		/**
		 * @brief Finds the poll file descriptor for a given socket file descriptor.
		 * @param socketFD The socket file descriptor to find.
		 * @return The poll file descriptor for the given socket file descriptor.
		 */
		struct pollfd	findPollFD(int socketFD);

		/**
		 * @brief Finds the server that corresponds to a given client socket file descriptor.
		 * @param clientSocketFD The client socket file descriptor to find.
		 * @return The server that corresponds to the given client socket file descriptor.
		 */
		HTTPServer		*findClientServer(int clientSocketFD);
	
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
		 * @brief Adds a client socket file descriptor to the server manager.
		 * @param socketFD The client socket file descriptor to add.
		 */
		void addClient(int socketFD);

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

		/**
		 * @class SocketNotFound
		 * @brief Exception thrown when a socket file descriptor is not found.
		 */
		class SocketNotFound : public std::exception
		{
			const char	*what() const throw();
		};

		/**
		 * @class ServerNotFound
		 * @brief Exception thrown when a server is not found.
		 */
		class ServerNotFound : public std::exception
		{
			const char	*what() const throw();
		};
};

#endif