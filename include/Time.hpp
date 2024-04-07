/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Time.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 11:14:08 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/06 16:28:33 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIME_HPP
# define TIME_HPP

# include <ctime>
# include <string>

/**
 * @class Time
 * @brief Class that provides functionalities related to time.
 *
 * This class is designed to offer static methods related to time,
 * allowing to retrieve the current time in a specific format.
 */
class Time
{
	public:
		/**
		 * @brief Retrieves the current time as a string.
		 * 
		 * This static method returns the current time formatted as a string.
		 * The format used is "YYYY-MM-DD HH:MM:SS", which represents the year,
		 * month, day, hours, minutes, and seconds of the current time.
		 *
		 * @return std::string The current time formatted as a string.
		 */
		static std::string getCurrentTime();
		/**
		 */
		static std::string getHTTPFormatCurrentTime();
};


#endif
