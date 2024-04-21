/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPMultiFormData.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 15:36:26 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/21 17:16:28 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPMultiformData.hpp"

HTTPFormFile::HTTPFormFile(const std::string &filename, const std::string &contentType, const std::string &content)
	: filename(filename), contentType(contentType), content(content) {}

HTTPFormFile::HTTPFormFile(const HTTPFormFile &other)
	: filename(other.filename), contentType(other.contentType), content(other.content) {}

HTTPFormFile::~HTTPFormFile() {}

HTTPFormFile &HTTPFormFile::operator=(const HTTPFormFile &other)
{
	if (this != &other)
	{
		filename = other.filename;
		contentType = other.contentType;
		content = other.content;
	}
	return *this;
}

const std::string &HTTPFormFile::getFilename() const
{
	return filename;
}

const std::string &HTTPFormFile::getContentType() const
{
	return contentType;
}

const std::string &HTTPFormFile::getContent() const
{
	return content;
}

std::vector<HTTPFormFile> HTTPMultiFormData::parseMultiFormData(const std::string &body, const std::string &boundary)
{
    std::vector<HTTPFormFile> files;
    std::string delimiter = boundary;
    std::string endDelimiter = delimiter + "--";
    size_t pos = 0, endPos = 0;

    while ((pos = body.find(delimiter, pos)) != std::string::npos)
    {
        pos += delimiter.length();
        if (body.substr(pos, 2) == "\r\n")
            pos += 2;
        endPos = body.find(delimiter, pos);
        if (endPos == std::string::npos)
		{
            endPos = body.find(endDelimiter, pos);
            if(endPos == std::string::npos) break;
        }
        std::string part = body.substr(pos, endPos - pos);
        pos = endPos;
        size_t headerEndPos = part.find("\r\n\r\n");
        if (headerEndPos != std::string::npos)
		{
            std::string headers = part.substr(0, headerEndPos);
            std::string content = part.substr(headerEndPos + 4);
            std::istringstream headerStream(headers);
            std::string header;
            std::string filename, contentType;
            while (std::getline(headerStream, header))
			{
                if (header.find("Content-Type:") != std::string::npos)
                    contentType = header.substr(header.find(":") + 2);
                else if (header.find("filename=") != std::string::npos)
				{
                    size_t start = header.find("filename=") + 10;
                    size_t end = header.find('"', start);
                    if (end != std::string::npos)
                        filename = header.substr(start, end - start);
                }
            }
            if (!filename.empty())
			{
				HTTPFormFile file(filename, contentType, content.substr(0, content.length() - 2));
				files.push_back(file);
			}
        }
    }
    return files;
}
HTTPMultiFormData::HTTPMultiFormData(const HTTPRequest &request)
{
	std::string contentType = request.getHeaders().at("Content-Type");
	isMultiFormData = false;
	if (contentType.find("multipart/form-data") != std::string::npos)
	{
		std::size_t boundaryPos = contentType.find("boundary=");
		if (boundaryPos != std::string::npos)
		{
			std::string boundary = contentType.substr(boundaryPos + 9);
			size_t firstQuote = boundary.find_first_of("\"");
			if (firstQuote != std::string::npos)
			{
				size_t lastQuote = boundary.find_last_of("\"");
				boundary = boundary.substr(firstQuote + 1, lastQuote - firstQuote - 1);
			}
			std::size_t semicolonPos = boundary.find(";");
			if (semicolonPos != std::string::npos)
			{
				boundary = boundary.substr(0, semicolonPos);
			}
			boundary.erase(0, boundary.find_first_not_of(" \t\r\n"));
			boundary.erase(boundary.find_last_not_of(" \t\r\n") + 1);
			if (!boundary.empty())
			{
				this->files = parseMultiFormData(request.getBody(), boundary);
				isMultiFormData = true;
			}
		}
	}
}

HTTPMultiFormData::HTTPMultiFormData(const HTTPMultiFormData &other)
	: request(other.request), files(other.files), isMultiFormData(other.isMultiFormData) {}

HTTPMultiFormData::~HTTPMultiFormData() {}

HTTPMultiFormData &HTTPMultiFormData::operator=(const HTTPMultiFormData &other)
{
	if (this != &other)
	{
		request = other.request;
		files = other.files;
		isMultiFormData = other.isMultiFormData;
	}
	return *this;
}

const HTTPRequest &HTTPMultiFormData::getRequest() const
{
	return request;
}

const std::vector<HTTPFormFile> &HTTPMultiFormData::getFiles() const
{
	return files;
}
