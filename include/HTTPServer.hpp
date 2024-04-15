/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 10:36:56 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/15 20:40:09 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_SERVER_HPP
# define HTTP_SERVER_HPP

# include <fcntl.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <exception>
# include <cstring>
# include <map>
# include <sys/time.h>

# include "HTTPRequestParser.hpp"
# include "HTTPResponseBuilder.hpp"
# include "Logger.hpp"
# include "ServerConfig.hpp"

/**
 * @class HTTPServer
 * @brief Manages the operations of an HTTP server, handling client connections, requests, and responses.
 *
 * HTTPServer encapsulates the functionality required for setting up a server socket, 
 * listening for incoming connections, processing HTTP requests, and sending responses. 
 * It uses a non-blocking approach to handle multiple connections simultaneously.
 */
class HTTPServer
{
	private:
		static const time_t CONNECTION_TIMEOUT = 1; ///< Timeout for closing inactive connections.
		int socketFD; ///< File descriptor for the server's listening socket.
		ServerConfig &serverConfig; ///< Configuration settings for the server.
		std::map<int, struct timeval> connections; ///< Active connections with their last activity time.

		/**
		 * @brief Initializes the server socket based on the server configuration.
		 */
		void	initializeServerSocket();

		/**
		 * @brief Checks and closes inactive connections that have exceeded the timeout.
		 */
		void	checkAndCloseInactiveConnections();

		/**
		 * @brief Receives and parses an HTTP request from the client.
		 * @param clientSocketFD The file descriptor for the client's socket.
		 * @return An HTTPRequest object representing the received request.
		 */
		HTTPRequest receiveRequest(int clientSocketFD);

		/**
		 * @brief Processes the HTTP request and generates an appropriate response.
		 * @param request An HTTPRequest object representing the client's request.
		 * @return An HTTPResponse object representing the server's response.
		 */
		HTTPResponse processRequest(const HTTPRequest &request);

		/**
		 * @brief Sends an HTTP response to the client.
		 * @param response An HTTPResponse object representing the server's response to send.
		 */
		void sendResponse(const HTTPResponse &response, int clientSocketFD);

	public:
		/**
		 * @brief Constructs an HTTPServer with a given server configuration.
		 * @param serverConfig A reference to a ServerConfig object.
		 */
		HTTPServer(ServerConfig &serverConfig);

		/**
		 * @brief Destructor for HTTPServer, cleans up the server's resources.
		 */
		~HTTPServer();

		/**
		 * @brief Gets the file descriptor for the server's listening socket.
		 * @return The file descriptor for the server's socket.
		 */
		int getSocketFD() const;

		/**
		 * @brief Accepts a new connection from a client.
		 */
		void	acceptConnection();

		/**
		 * @class SocketError
		 * @brief Exception thrown when a socket operation fails.
		 */
		class SocketError : public std::exception
		{
			public:
				const char *what() const throw();
		};

		/**
		 * @class BindError
		 * @brief Exception thrown when the server fails to bind to a port.
		 */
		class BindError : public std::exception
		{
			public:
				const char *what() const throw();
		};

		/**
		 * @class ListenError
		 * @brief Exception thrown when the server fails to listen on a port.
		 */
		class ListenError : public std::exception
		{
			public:
				const char *what() const throw();
		};

		/**
		 * @class SocketOptionError
		 * @brief Exception thrown when setting socket options fails.
		 */
		class SocketOptionError : public std::exception
		{
			public:
				const char *what() const throw();
		};

		/**
		 * @class AcceptError
		 * @brief Exception thrown when the server fails to accept a new connection.
		 */
		class AcceptError : public std::exception
		{
			public:
				const char *what() const throw();
		};
};

#endif
