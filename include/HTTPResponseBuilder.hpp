/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponseBuilder.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 14:35:19 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/07 13:42:54 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_BUILDER_HPP
# define HTTP_RESPONSE_BUILDER_HPP

# include <sstream>
# include "HTTPResponse.hpp"

/**
 * @class HTTPResponseBuilder
 * @brief A builder class that constructs a raw HTTP response string from an HTTPResponse object.
 *
 * This class provides static methods to convert an HTTPResponse object into
 * a raw HTTP response string. It handles the creation of the status line and headers,
 * and assembles them with the response body.
 */
class HTTPResponseBuilder
{
	private:
		/**
		 * @brief Private constructor to prevent instantiation.
		 */
		HTTPResponseBuilder();

		/**
		 * @brief Builds the status line of the HTTP response.
		 * @param buildResponse A reference to a string where the status line will be appended.
		 * @param response A constant reference to the HTTPResponse object.
		 */
		static void	buildStatusLine(std::string &buildResponse, const HTTPResponse &response);

		/**
		 * @brief Builds the headers of the HTTP response.
		 * @param buildResponse A reference to a string where the headers will be appended.
		 * @param response A constant reference to the HTTPResponse object.
		 */
		static void	buildHeaders(std::string &buildResponse, const HTTPResponse &response);

	public:
		/**
		 * @brief Builds a complete HTTP response string from an HTTPResponse object.
		 * @param response A constant reference to the HTTPResponse object to be converted.
		 * @return A string containing the raw HTTP response.
		 */
		static std::string buildResponse(const HTTPResponse &response);
};

#endif
