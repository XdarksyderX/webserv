/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 18:39:37 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/06 11:51:25 by migarci2         ###   ########.fr       */
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
