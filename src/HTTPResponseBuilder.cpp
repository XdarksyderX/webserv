/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponseBuilder.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 15:41:13 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/06 15:56:19 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponseBuilder.hpp"

void	HTTPResponseBuilder::buildStatusLine(std::string &buildResponse, const HTTPResponse &response)
{
	buildResponse += "HTTP/";
	buildResponse += response.getHttpVersion();
	buildResponse += " ";
	std::stringstream ss;
	ss << response.getStatusCode();
	buildResponse += ss.str();
	buildResponse += " ";
	buildResponse += response.getStatusMessage();
	buildResponse += "\r\n";
}

void	HTTPResponseBuilder::buildHeaders(std::string &buildResponse, const HTTPResponse &response)
{
	std::map<std::string, std::string>::const_iterator it;
    for (it = response.getHeaders().begin(); it != response.getHeaders().end(); ++it)
	{
        buildResponse += it->first;
        buildResponse += ": ";
        buildResponse += it->second;
        buildResponse += "\r\n";
    }
}

std::string HTTPResponseBuilder::buildResponse(const HTTPResponse &response)
{
	std::string buildResponse;

	buildStatusLine(buildResponse, response);
	buildHeaders(buildResponse, response);
	buildResponse += "\r\n";
	buildResponse += response.getBody();
	return buildResponse;
}