/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 18:39:37 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/11 22:58:30 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

void Logger::log(const std::string &message, Level level, bool printTime)
{
	std::string currentTime = std::string("[") + Time::getCurrentTime() + "] ";
	switch (level)
	{
	case ERROR:
		std::cerr	<< RED << "[-] " << (printTime ? currentTime : "")
					<< message << RESET << std::endl;
		break;
	case WARNING:
		std::cout	<< YELLOW << "[!] " << (printTime ? currentTime : "")
					<< message << RESET << std::endl;
		break;
	case INFO:
		std::cout	<< CYAN << "[+] " << (printTime ? currentTime : "")
					<< message << RESET << std::endl;
		break;
	default:
		std::cerr	<< "Unknown log level: " << (printTime ? currentTime : "")
					<< message << std::endl;
		break;
	}
}

void Logger::logRequest(const HTTPRequest &request, const HTTPResponse &response, int clientSocketFD,
		const std::string &server, bool printTime)
{
	std::string ip = getClientSocketIP(clientSocketFD);
    std::string message = std::string("[") + server + "] "
							+ HTTPRequest::getMethodString(request.getMethod())
							+ " " + request.getUri()
							+ " from " + ip
							+ " - " + Logger::to_string(response.getStatusCode())
							+ " " + response.getStatusMessage();
    log(message, INFO, printTime);
}

std::string	Logger::getClientSocketIP(int clientSocketFD)
{
	struct sockaddr_in addr;
	socklen_t addrLen = sizeof(addr);
	getpeername(clientSocketFD, (struct sockaddr *)&addr, &addrLen);
	return inet_ntoa(addr.sin_addr);

}

std::string Logger::to_string(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

void	Logger::logHeader()
{
	std::string output;
	output += "\n█╗    ██╗███████╗██████╗ ███████╗███████╗██████╗ ██╗   ██╗\n";
	output += "██║    ██║██╔════╝██╔══██╗██╔════╝██╔════╝██╔══██╗██║   ██║\n";
	output += "██║ █╗ ██║█████╗  ██████╔╝███████╗█████╗  ██████╔╝██║   ██║\n";
	output += "██║███╗██║██╔══╝  ██╔══██╗╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝\n";
	output += "╚███╔███╔╝███████╗██████╔╝███████║███████╗██║  ██║ ╚████╔╝ \n";
	output += " ╚══╝╚══╝ ╚══════╝╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝  \n";
	
	std::cout << CYAN << output << RESET << std::endl;
}
