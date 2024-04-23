/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erivero- <erivero-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:42:28 by erivero-          #+#    #+#             */
/*   Updated: 2024/04/23 15:26:53 by erivero-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"

CGIHandler::CGIHandler(LocationConfig &conf, HTTPRequest &req) {
	this->config = conf;
	this->request = req;
	this->path = getPath(); //if the path is empty there's no extension so it shouldn't do anything else
}

std::string CGIHandler::getExtension(void) {

	std::string uri = request.getUri();
	size_t dotPos = uri.find_last_of('.');
	if (dotPos != std::string::npos)
		return (uri.substr(dotPos));
	return ("");
}

std::string CGIHandler::getPath(void) {

	std::string ext = getExtension();

	if (ext.empty())
		return ("");
	std::map<std::string, std::string> supportedExtensions = config.getCgiInfo();
	std::map<std::string, std::string>::iterator it;
	it = supportedExtensions.find(ext);
	if (it != supportedExtensions.end())
		return (it->second);
	throw(std::runtime_error("Non supported Extension")); //this is provisional
}



void	initEnv(/* HTTPRequest &request */) {

	/* this should init some env variables like: 
	CONTENT_LENGTH, CONTENT_TYPE, REQUEST_METHOD, REMOTE_ADDR,
	SERVER_NAME, SERVER_PORT, SCRIPT_NAME, SCRIPT_FILENAME

	some of them should be only seted in some of the methods, for example:
	if we are in GET method, it will be moment to parse the query strings
	to store it in a QUERY_STRING variable */
}