/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponseBuilder.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 14:35:19 by migarci2          #+#    #+#             */
/*   Updated: 2024/06/10 15:14:16 by migarci2         ###   ########.fr       */
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
# include "HTTPMultiformData.hpp"
# include "HTTPCookie.hpp"
# include "CGIHandler.hpp"
# include "URLEncoder.hpp"

/**
 * @class HTTPResponseBuilder
 * @brief A builder class that constructs a raw HTTP response string from an HTTPResponse object.
 *
 * This class provides static methods to convert an HTTPResponse object into
 * a raw HTTP response string. It handles the creation of the status line, headers,
 * and assembles them with the response body based on the server configuration and the
 * specific HTTP request received.
 */
class HTTPResponseBuilder
{
	private:
		const ServerConfig &serverConfig; ///< Reference to the server configuration object.
		const HTTPRequest &request; ///< Reference to the HTTP request object.

		/**
		 * @brief Assembles the status line of the HTTP response.
		 * @param assembleResponse A reference to a string where the status line will be appended.
		 * @param response A constant reference to the HTTPResponse object.
		 * @details This method formats and appends the HTTP status line to the response string.
		 */
		static void assembleStatusLine(std::string &assembleResponse, const HTTPResponse &response);

		/**
		 * @brief Assembles the headers of the HTTP response.
		 * @param assembleResponse A reference to a string where the headers will be appended.
		 * @param response A constant reference to the HTTPResponse object.
		 * @details This method formats and appends the HTTP headers according to the content and requirements of the response.
		 */
		static void assembleHeaders(std::string &assembleResponse, const HTTPResponse &response);

		/**
		 * @brief Finds the best matching location configuration for the current request.
		 * @return A string representing the matched location path.
		 * @details This method matches the request URI with configured locations to determine the best handling strategy.
		 */
		std::string findMatchingLocation();

		/**
		 * @brief Creates a map of MIME types based on file extensions.
		 * @return A map linking file extensions to their corresponding MIME type.
		 */
		static std::map<std::string, std::string> createMIMEMap();

		/**
		 * @brief Creates a map of HTTP status codes to their descriptive phrases.
		 * @return A map linking status codes with their corresponding messages.
		 */
		static std::map<int, std::string> createStatusCodesMap();

		/**
		 * @brief Handles a multipart/form-data request based on the location configuration.
		 * @param location Pointer to the LocationConfig object.
		 * @return An HTTPResponse object tailored to the multipart/form-data request.
		 */
		HTTPResponse handleMultiFormDataRequest(const LocationConfig *location);

		/**
		 * @brief Handles a GET request based on the location configuration.
		 * @param location Pointer to the LocationConfig object.
		 * @param CGIHandler Pointer to the CGIHandler object.
		 * @return An HTTPResponse object tailored to the GET request.
		 */
		HTTPResponse handleGetRequest (const LocationConfig *location, CGIHandler &cgiHandler);

		/**
		 * @brief Handles a POST request based on the location configuration.
		 * @param location Pointer to the LocationConfig object.
		 * @param CGIHandler Pointer to the CGIHandler object.
		 * @return An HTTPResponse object tailored to the POST request.
		 */
		HTTPResponse handlePostRequest(const LocationConfig *location/* , CGIHandler &cgiHandler */);

		/**
		 * @brief Handles a DELETE request based on the location configuration.
		 * @param location Pointer to the LocationConfig object.
		 * @return An HTTPResponse object tailored to the DELETE request.
		 */
		HTTPResponse handleDeleteRequest(const LocationConfig *location);

		/**
		 * @brief Generates a standard error page for a given error code.
		 * @param errorCode The HTTP status code that indicates an error.
		 * @return An HTTPResponse object containing the error page.
		 */
		HTTPResponse handleErrorPage(int errorCode);

		/**
		 * @brief Generates a simple error page for a given error code.
		 * @param errorCode The HTTP status code.
		 * @return A string containing the HTML for a simple error page.
		 */
		std::string simpleErrorPage(int errorCode);

		/**
		 * @brief Retrieves the upload path for files based on the location configuration.
		 * @param location Pointer to the LocationConfig object.
		 * @return A string containing the path where files should be uploaded.
		 */
		std::string getLocationUploadPath(const LocationConfig *location);

		/**
		 * @brief Adds common headers to the HTTP response, such as Server, Date, etc.
		 * @param request Reference to the HTTPRequest object.
		 * @param response Reference to the HTTPResponse object to which headers will be added.
		 */
		void addCommonHeaders(const HTTPRequest &request, HTTPResponse &response);

	public:
		static const std::map<std::string, std::string> MIME_TYPES; ///< Map of file extensions to MIME types.
		static const std::map<int, std::string> STATUS_CODES; ///< Map of status codes to status messages.

		/**
		 * @brief Constructor that initializes a new HTTPResponseBuilder with a server configuration and request.
		 * @param sC Reference to the ServerConfig object.
		 * @param r Reference to the HTTPRequest object.
		 */
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
		HTTPResponse buildResponse();
};


#endif
