/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erivero- <erivero-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:42:28 by erivero-          #+#    #+#             */
/*   Updated: 2024/04/24 15:04:37 by erivero-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"

CGIHandler::CGIHandler(LocationConfig &conf, HTTPRequest &req) {
	this->config = conf;
	this->request = req;
	this->path = getPath(); //if the path is empty there's no extension so it shouldn't do anything else
	if (!this->path.empty()) {
		this->args = this->setArgs();

	}
}

std::string CGIHandler::getExtension(void) {
 // parsing: whatever.py?arg1=potato
	std::string uri = request.getUri();
	size_t dotPos = uri.find_last_of('.');
	size_t size = uri.find_last_of('?') - dotPos;
	if (dotPos != std::string::npos)
		return (uri.substr(dotPos, size));
	return ("");
}

CGIHandler::~CGIHandler() {

	if (!this->path.empty())
		delete [] args;
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

/* void	initEnv(HTTPRequest &request) {

	// execve(*argv, argv, env);

	// this should init some env variables like: 
	// CONTENT_LENGTH, CONTENT_TYPE, REQUEST_METHOD, REMOTE_ADDR,
	// SERVER_NAME, SERVER_PORT, SCRIPT_NAME, SCRIPT_FILENAME

	// some of them should be only seted in some of the methods, for example:
	// if we are in GET method, it will be moment to parse the query strings
	// to store it in a QUERY_STRING variable

	// Me dicen por el pinganillo (yolanthe) que a execve en este caso no le 
	// hacen falta realmente las variables de entorno, se le puede mandar NULL
	// execve recibirá: el path, un doble puntero al path y los argumentos, y NULL
} */

char **CGIHandler::setArgs(void) {

	std::string query = request.getQuery();
	//if there are 2 '&' there will be 3 args, + 1 for the path
	int n = std::count(query.begin(), query.end(), '&') + 2;
	std::vector<std::string> args(n);
	args[0] = path;
	size_t start = 0;
	for (int i = 1; i < n; ++i) {
		size_t end = query.find('&', start);
		if (end == std::string::npos) {
			args[i] = query.substr(start);
		} else {
			args[i] = query.substr(start, end - start);
		}
		start = end + 1; // +1 to skip the &
	}

	char **argv = new char*[n + 1]; // + 1 for the NULL
	for (int i = 0; i < n; ++i) {
		argv[i] = new char[args[i].size() + 1];
		std::strcpy(argv[i], args[i].c_str());
	}
	argv[n + 1] = NULL;
	return (argv);
}
