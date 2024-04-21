/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPMultiformData.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 15:27:34 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/21 22:26:14 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_MULTIFORM_DATA_HPP
# define HTTP_MULTIFORM_DATA_HPP

# include <vector>
# include <utility>
# include <sstream>
# include <iostream>

# include "HTTPRequest.hpp"
# include "utils.hpp"

/**
 * @class HTTPFormFile
 * @brief Represents a single file uploaded via HTTP form.
 *
 * This class provides an abstraction for a file uploaded through an HTTP form,
 * including details like the filename, content type, and content of the file.
 */
class HTTPFormFile
{
private:
    std::string filename;      ///< Name of the file.
    std::string contentType;   ///< MIME type of the file.
    std::string content;       ///< Content of the file.

public:
    /**
     * @brief Construct a new HTTP Form File object.
     * @param filename Name of the file.
     * @param contentType MIME type of the file.
     * @param content Content of the file.
     */
    HTTPFormFile(const std::string &filename, const std::string &contentType, const std::string &content);

    /**
     * @brief Copy constructor.
     * @param other Reference to another HTTPFormFile object to copy data from.
     */
    HTTPFormFile(const HTTPFormFile &other);

    /**
     * @brief Destructor.
     */
    ~HTTPFormFile();

    /**
     * @brief Copy assignment operator.
     * @param other Reference to another HTTPFormFile object to assign from.
     * @return Reference to this HTTPFormFile instance.
     */
    HTTPFormFile &operator=(const HTTPFormFile &other);

    /**
     * @brief Get the filename.
     * @return const reference to a std::string containing the filename.
     */
    const std::string &getFilename() const;

    /**
     * @brief Get the content type.
     * @return const reference to a std::string containing the MIME type of the file.
     */
    const std::string &getContentType() const;

    /**
     * @brief Get the content of the file.
     * @return const reference to a std::string containing the content of the file.
     */
    const std::string &getContent() const;
};


/**
 * @class HTTPMultiFormData
 * @brief Represents the multipart/form-data of an HTTP request.
 *
 * This class encapsulates the multipart/form-data of an HTTP request, including any files
 * uploaded as part of that request. It provides functionality to parse the multipart content
 * and retrieve the files.
 */
class HTTPMultiFormData
{
	private:
		HTTPRequest request; ///< The original HTTP request containing the multipart data.
		std::vector<HTTPFormFile> files; ///< List of files uploaded with the request.
		bool isMultiFormData; ///< Flag to indicate whether the request includes multipart/form-data.

		/**
		 * @brief Parses the multipart/form-data body to extract files.
		 * @param body The body of the HTTP request.
		 * @param boundary The boundary string used to separate parts in the multipart body.
		 * @return A vector of HTTPFormFile objects representing the files uploaded.
		 */
		static std::vector<HTTPFormFile> parseMultiFormData(const std::string &body, const std::string &boundary);

	public:
		/**
		 * @brief Construct a new HTTP Multi Form Data object from an HTTPRequest.
		 * @param request The HTTPRequest object containing multipart/form-data.
		 */
		HTTPMultiFormData(const HTTPRequest &request);

		/**
		 * @brief Copy constructor.
		 * @param other Reference to another HTTPMultiFormData object to copy data from.
		 */
		HTTPMultiFormData(const HTTPMultiFormData &other);

		/**
		 * @brief Destructor.
		 */
		~HTTPMultiFormData();

		/**
		 * @brief Copy assignment operator.
		 * @param other Reference to another HTTPMultiFormData object to assign from.
		 * @return Reference to this HTTPMultiFormData instance.
		 */
		HTTPMultiFormData &operator=(const HTTPMultiFormData &other);

		/**
		 * @brief Get the original HTTP Request.
		 * @return const reference to the HTTPRequest object.
		 */
		const HTTPRequest &getRequest() const;

		/**
		 * @brief Get the uploaded files.
		 * @return const reference to a vector of HTTPFormFile objects.
		 */
		const std::vector<HTTPFormFile> &getFiles() const;
};

#endif
