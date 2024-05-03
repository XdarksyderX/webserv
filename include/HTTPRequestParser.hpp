/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erivero- <erivero-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 14:36:49 by migarci2          #+#    #+#             */
/*   Updated: 2024/05/02 16:29:57 by erivero-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST_PARSER_HPP
# define HTTP_REQUEST_PARSER_HPP

# include <exception>
# include "HTTPRequest.hpp"

/**
 * @class HTTPRequestParser
 * @brief A parser class that interprets raw HTTP requests.
 *
 * This class provides static methods to parse raw HTTP request strings
 * into an HTTPRequest object. It processes the request line and headers
 * and populates the HTTPRequest object with parsed data.
 */
class HTTPRequestParser
{
	private:
		/**
		 * @brief Private constructor to prevent instantiation.
		 */
		HTTPRequestParser();

		/**
		 * @brief Parses the headers of the HTTP request.
		 * @param request A reference to the HTTPRequest object to populate.
		 * @param headerLine A string representing a single header line.
		 */
		static void	parseRequestHeaders(HTTPRequest &request, const std::string &headerLine);

		/**
		 * @brief Parses the request line of the HTTP request.
		 * @param request A reference to the HTTPRequest object to populate.
		 * @param requestLine A string representing the request line.
		 */
		static void	parseRequestLine(HTTPRequest &request, const std::string &requestLine);

	public:
		/**
		 * @brief Parse the query string if it exist.
		 * @param request A string representing the request line.
		 * @param requestLine The query string.
		 */
		static const std::string	parseQueryString(const std::string &uri);
		/**
		 * @brief Parses a raw HTTP request string.
		 * @param rawRequest A string containing the raw HTTP request.
		 * @return An HTTPRequest object populated with the parsed data.
		 */
		static HTTPRequest parseRequest(const std::string &rawRequest);

		/**
		 * @class InvalidRequestLine
		 * @brief An exception indicating an invalid request line.
		 */
		class InvalidRequestLine : public std::exception
		{
			public:
				/**
				 * @brief Provides an error message.
				 * @return A character string describing the error.
				 */
				virtual const char *what() const throw();
		};

		/**
		 * @class InvalidRequestHeader
		 * @brief An exception indicating an invalid request header.
		 */
		class InvalidRequestHeader : public std::exception
		{
			public:
				/**
				 * @brief Provides an error message.
				 * @return A character string describing the error.
				 */
				virtual const char *what() const throw();
		};
};

#endif
