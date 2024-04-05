/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 18:33:17 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/05 18:48:59 by migarci2         ###   ########.fr       */
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

/**
 * @brief Class for logging messages with different severity levels.
 */
class Logger
{
	public:
		static const unsigned short ERROR = 0; ///< Error level
		static const unsigned short WARNING = 1; ///< Warning level
		static const unsigned short INFO = 2; ///< Information level

		/**
		 * @brief Logs a message with a specified severity level.
		 * @param message Message to log.
		 * @param level Severity level of the message (default is INFO).
		 */
		static void log(const std::string &message, unsigned short level = INFO);

	private:
		Logger();
};

#endif
