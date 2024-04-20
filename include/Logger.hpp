/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 18:33:17 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/20 22:47:02 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
# define LOGGER_HPP

# define RESET   "\033[0m"
# define RED     "\033[31m"      //Red
# define YELLOW  "\033[33m"      //Yellow
# define CYAN    "\033[36m"      //Cyan

# include <iostream>
# include <string>
# include <sstream>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/socket.h>

# include "Time.hpp"
# include "HTTPRequest.hpp"
# include "HTTPResponse.hpp"
# include "utils.hpp"

/**
 * @brief Class for logging messages with different severity levels.
 */
class Logger
{
	public:
		/**
		 * @brief Enum for different severity levels.
		 */
		enum Level
		{
        	ERROR,   ///< Error level
        	WARNING, ///< Warning level
        	INFO     ///< Information level
		};

		/**
		 * @brief Logs a message with a specified severity level.
		 * @param message Message to log.
		 * @param level Severity level of the message (default is INFO).
		 * @param printTime Whether to print the current time before the message (default is false).
		 */
		static void log(const std::string &message, Level level = INFO, bool printTime = false, bool printEndline = true);

		/**
		 * @brief Prints webserv header.
		 */
		static void	logHeader();

		/**
		 * @brief Logs an HTTP request.
		 * @param request The HTTP request to log.
		 * @param clientSocketFD The file descriptor of the client socket.
		 * @param port The port number of server
		 * @param printTime Whether to print the current time before the message (default is false).
		*/
		static void	logRequest(const HTTPRequest &request, const HTTPResponse &response, int clientSocketFD,
					const std::string &server, bool printTime);

		/**
		 * @brief Gets the IP address of a client socket.
		 * @param clientSocketFD The file descriptor of the client socket.
		 */
		static std::string	getClientSocketIP(int clientSocketFD);

		/**
		 * @brief Switch verbose mode
		 */
		static void	switchPrint();

	private:
		Logger();
		static bool printEnabled;
};

#endif
