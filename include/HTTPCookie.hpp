/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPCookie.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 22:37:23 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/21 22:26:20 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCOOKIE_HPP
# define HTTPCOOKIE_HPP

# include <string>
# include <cstdlib>
# include <ctime>

# include "utils.hpp"

/**
 * @class HTTPCookie
 * @brief Represents an HTTP Cookie with an integer key and a timestamp.
 *
 * This class provides a way to handle HTTP cookies with an identifier and a
 * timestamp indicating when the cookie was created or last modified. It includes
 * functionalities to serialize and deserialize cookie objects to and from strings.
 */
class HTTPCookie
{
	private:
		int     key;    ///< Key of the cookie as an integer.
		time_t  time;   ///< Time when the cookie was set, represented as a time_t type.

	public:
		/**
		 * @brief Default constructor that initializes a new HTTPCookie object.
		 */
		HTTPCookie();

		/**
		 * @brief Copy constructor.
		 * @param other Another HTTPCookie object to be used as source to initialize the data members of the new object.
		 */
		HTTPCookie(const HTTPCookie &other);

		/**
		 * @brief Destructor.
		 */
		~HTTPCookie();

		/**
		 * @brief Copy assignment operator.
		 * @param other Another HTTPCookie object to be assigned to the current object.
		 * @return Reference to the current object (*this).
		 */
		HTTPCookie &operator=(const HTTPCookie &other);

		/**
		 * @brief Gets the key of the cookie.
		 * @return The key of the cookie as an integer.
		 */
		int getKey() const;

		/**
		 * @brief Gets the timestamp of the cookie.
		 * @return The time_t value representing the time the cookie was set.
		 */
		time_t getTime() const;

		/**
		 * @brief Serializes the cookie into a string.
		 * @param cookie A reference to the HTTPCookie object to serialize.
		 * @return A std::string representing the serialized cookie.
		 */
		static std::string serializeCookie(const HTTPCookie &cookie);

		/**
		 * @brief Deserializes a string into an HTTPCookie object.
		 * @param cookie A string representing the serialized cookie.
		 * @return An HTTPCookie object deserialized from the string.
		 */
		static HTTPCookie deserializeCookie(const std::string &cookie);
	};

#endif
