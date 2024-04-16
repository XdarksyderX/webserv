/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 13:03:47 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/15 16:07:51 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

# include <string>
# include <map>

/**
 * @class HTTPResponse
 * @brief Represents an HTTP response.
 *
 * This class encapsulates all components of an HTTP response, including
 * the protocol version, status code, status message, response body, and headers.
 */
class HTTPResponse
{
    private:
        std::string httpVersion; ///< HTTP protocol version.
        int statusCode; ///< HTTP status code.
        std::string statusMessage; ///< HTTP status message.
        std::string body; ///< Response message body.
        std::map<std::string, std::string> headers; ///< HTTP response headers.

    public:

        HTTPResponse();
        HTTPResponse(const HTTPResponse &other);
        ~HTTPResponse();
        HTTPResponse &operator=(const HTTPResponse &other);

        std::string getHttpVersion() const;
        int getStatusCode() const;
        std::string getStatusMessage() const;
        std::string getBody() const;
        std::map<std::string, std::string> getHeaders() const;

        void setHttpVersion(const std::string &httpVersion);
        void setStatusCode(int statusCode);
        void setStatusMessage(const std::string &statusMessage);
        void setBody(const std::string &body);

        /**
         * @brief Adds a header to the response.
         * @param key The header key.
         * @param value The header value.
         */
        void addHeader(const std::string &key, const std::string &value);

        /**
         * @brief Retrieves a header from the response.
         * @param key The header key to retrieve.
         * @return The header value if found, otherwise an empty string.
         */
        std::string getHeader(const std::string &key) const;

        /**
         * @brief Clears the response content, resetting all fields to their default values.
         */
        void clear();
};

#endif
