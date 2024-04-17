/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponseBuilder.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 14:35:19 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/17 17:42:41 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_BUILDER_HPP
# define HTTP_RESPONSE_BUILDER_HPP

# include <sstream>
# include <fstream>

# include "Logger.hpp"
# include "HTTPResponse.hpp"
# include "HTTPRequest.hpp"
# include "ServerConfig.hpp"
# include "utils.hpp"

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
		const ServerConfig	&serverConfig; ///< Reference to the server configuration object.
		const HTTPRequest	&request; ///< Reference to the HTTP request object.

		/**
		 * @brief Assembles the status line of the HTTP response.
		 * @param assembleResponse A reference to a string where the status line will be appended.
		 * @param response A constant reference to the HTTPResponse object.
		 */
		static void	assembleStatusLine(std::string &assembleResponse, const HTTPResponse &response);

		/**
		 * @brief Assembles the headers of the HTTP response.
		 * @param assembleResponse A reference to a string where the headers will be appended.
		 * @param response A constant reference to the HTTPResponse object.
		 */
		static void	assembleHeaders(std::string &assembleResponse, const HTTPResponse &response);

		std::string findMatchingLocation();

		static std::map<std::string, std::string>	createMIMEMap();
		static std::map<int, std::string> 			createStatusCodesMap();

		HTTPResponse	handleGetRequest(const LocationConfig *location);
		HTTPResponse	handlePostRequest(const LocationConfig *location);
		HTTPResponse	handleDeleteRequest(const LocationConfig *location);
		
		HTTPResponse	handleErrorPage(int errorCode);
		std::string		simpleErrorPage(int errorCode);

	public:
        static const std::map<std::string, std::string> MIME_TYPES; ///< Map of file extensions to MIME types.
        static const std::map<int, std::string> STATUS_CODES; ///< Map of status codes to status messages.
	
		HTTPResponseBuilder(const ServerConfig &sC, const HTTPRequest &r);
		/**
		 * @brief Assembles a complete HTTP response string from an HTTPResponse object.
		 * @param response A constant reference to the HTTPResponse object to be converted.
		 * @return A string containing the raw HTTP response.
		 */
		static std::string assembleResponse(const HTTPResponse &response);

		/**
		 * @brief Builds an HTTP response based on the server configuration and request.
		 * @return An HTTPResponse object representing the response to the request.
		 */
		HTTPResponse	buildResponse();

		
};

#endif
