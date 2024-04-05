/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 18:39:37 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/05 20:32:28 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

void Logger::log(const std::string &message, unsigned short level)
{
	switch (level)
	{
	case ERROR:
		std::cerr << RED << "[-] " << message << RESET << std::endl;
		break;
	case WARNING:
		std::cout << YELLOW << "[!] " << message << RESET << std::endl;
		break;
	case INFO:
		std::cout << CYAN << "[+] " << message << RESET << std::endl;
		break;
	default:
		std::cerr << "Unknown log level: " << message << std::endl;
		break;
	}
}
