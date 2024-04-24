/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erivero- <erivero-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 12:36:18 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/24 14:54:47 by erivero-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

# include <string>
# include <map>

/**
 * @enum HTTPMethod
 * @brief Defines the supported HTTP methods for requests.
 *
 * This enumeration defines the types of HTTP methods that can be used in HTTP requests.
 * These methods correspond to the various actions that can be performed on the resources
 * identified by URIs. The NONE value is used to represent an undefined or unsupported method.
 */
enum HTTPMethod
{
    GET,    ///< Represents an HTTP GET request, used to retrieve data.
    POST,   ///< Represents an HTTP POST request, used to submit data to be processed.
    PUT,    ///< Represents an HTTP PUT request, used to create or update a resource.
    DELETE, ///< Represents an HTTP DELETE request, used to delete a resource.
    NONE    ///< Used to represent an undefined or unsupported HTTP method.
};

/**
 * @class HTTPRequest
 * @brief Represents an HTTP request.
 *
 * This class encapsulates all components of an HTTP request, including
 * the method, URI, HTTP version, body, and headers.
 */
class HTTPRequest
{
	private:
		HTTPMethod method; ///< HTTP method (GET, POST, PUT, DELETE, NONE).
        std::string uri; ///< URI of the request.
        std::string query;
        std::string httpVersion; ///< HTTP version.
        std::string body; ///< Request body.
        std::map<std::string, std::string> headers; ///< HTTP request headers.

	public:
		HTTPRequest();
		HTTPRequest(const HTTPRequest &other);
		~HTTPRequest();
		HTTPRequest &operator=(const HTTPRequest &other);
		HTTPMethod getMethod() const;
		std::string getUri() const;
		std::string getQuery() const;
		std::string getHttpVersion() const;
		std::string getBody() const;
		const std::map<std::string, std::string> &getHeaders() const;
        std::map<std::string, std::string> &getHeaders();
		void setMethod(HTTPMethod method);
		void setUri(const std::string &uri);
		void setHttpVersion(const std::string &httpVersion);
		void setBody(const std::string &body);
        void setQuery(const std::string &query);

		/**
         * @brief Adds a header to the request.
         * @param key The header key.
         * @param value The header value.
         */
        void addHeader(const std::string &key, const std::string &value);

        /**
         * @brief Gets a header from the request.
         * @param key The header key to retrieve.
         * @return The header value if found, otherwise an empty string.
         */
        std::string getHeader(const std::string &key) const;

        /**
         * @brief Clears the request content, resetting all fields to their default values.
         */
        void clear();

        /**
         * @brief Converts an HTTP method string to an HTTPMethod enumeration value.
         * @param method The HTTP method string to convert.
         * @return The corresponding HTTPMethod value.
         */
        static HTTPMethod  getMethodFromString(const std::string &method);

        /**
         * @brief Converts an HTTPMethod enumeration value to an HTTP method string.
         * @param method The HTTPMethod value to convert.
        */
        static std::string getMethodString(HTTPMethod method);
};

#endif
