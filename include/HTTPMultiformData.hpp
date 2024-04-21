/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPMultiformData.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 15:27:34 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/21 16:44:07 by migarci2         ###   ########.fr       */
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

class HTTPFormFile
{
	private:
		std::string filename;
		std::string contentType;
		std::string content;

	public:
		HTTPFormFile(const std::string &filename, const std::string &contentType, const std::string &content);
		HTTPFormFile(const HTTPFormFile &other);
		~HTTPFormFile();
		HTTPFormFile &operator=(const HTTPFormFile &other);

		const std::string &getFilename() const;
		const std::string &getContentType() const;
		const std::string &getContent() const;

};

class HTTPMultiFormData
{
	private:
		HTTPRequest request; // The original HTTP request.
		std::vector<HTTPFormFile> files; // Files uploaded in the request.
		bool isMultiFormData; // Indicates whether the request is a multipart/form-data request.

		static std::vector<HTTPFormFile> parseMultiFormData(const std::string &body, const std::string &boundary);

	public:
		HTTPMultiFormData(const HTTPRequest &request);
		HTTPMultiFormData(const HTTPMultiFormData &other);
		~HTTPMultiFormData();
		HTTPMultiFormData &operator=(const HTTPMultiFormData &other);

		const HTTPRequest &getRequest() const;
		const std::vector<HTTPFormFile> &getFiles() const;
};

#endif
