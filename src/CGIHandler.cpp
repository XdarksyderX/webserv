/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erivero- <erivero-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:42:28 by erivero-          #+#    #+#             */
/*   Updated: 2024/04/25 15:50:54 by erivero-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"

CGIHandler::CGIHandler(LocationConfig &conf, HTTPRequest &req) {
	this->config = conf;
	this->cgi = true;
	this->request = req;
	prepareCGI();
}

CGIHandler::~CGIHandler() {

	if (this->cgi)
		delete [] args;
}

std::string CGIHandler::getExtension(std::string uri) {
 // parsing: whatever.py?arg1=potato
//	std::string uri = request.getUri();
	size_t dotPos = uri.find_last_of('.');
	size_t size = uri.find_last_of('?') - dotPos;
	if (dotPos != std::string::npos)
		return (uri.substr(dotPos, size));
	this->cgi = false;
	return ("");
}

std::string CGIHandler::getPath(std::string ext) {

//	std::string ext = getExtension();

	if (ext.empty())
		return ("");
	std::map<std::string, std::string> supportedExtensions = config.getCgiInfo();
	std::map<std::string, std::string>::iterator it;
	it = supportedExtensions.find(ext);
	if (it != supportedExtensions.end())
		return (it->second);
	throw(std::runtime_error("Non supported Extension")); //this is provisional
}

std::string CGIHandler::getFilePath(std::string uri) {
//  what I have:    /cgi-bin/myscript.py?a=42
//  what I want:    ./cgi-bin/myscript.py
	std::string file_path = "." + uri;
	size_t size = uri.find('?');
	if (size != std::string::npos)
		return (file_path.substr(0, size));
	return (file_path);
	/* I was gonna take the current dir to make a join 
	and get the absolute path, but we're not allowed to use getcwd()
	anyways, execve should work with this path, if it didn't, I can
	use stat() with the file*/
}

/* void	initEnv(HTTPRequest &request) {

	// execve(*argv, argv, env);

	// this should init some env variables like: 
	// CONTENT_LENGTH, CONTENT_TYPE, REQUEST_METHOD, REMOTE_ADDR,
	// SERVER_NAME, SERVER_PORT, SCRIPT_NAME, SCRIPT_FILENAME

	// some of them should be only seted in some of the methods, for example:
	// if we are in GET method, it will be moment to parse the query strings
	// to store it in a QUERY_STRING variable

	// Me dicen por el pinganillo (Yolanthe) que a execve en este caso no le 
	// hacen falta realmente las variables de entorno, se le puede mandar NULL
	// execve recibirá: el path, un doble puntero al path y los argumentos, y NULL
} */

char **CGIHandler::setArgs(void) {

	std::string query = request.getQuery();
	//if there are 2 '&' there will be 3 args, + 1 for the path
	int n = std::count(query.begin(), query.end(), '&') + 2;
	std::vector<std::string> args(n);
	args[0] = this->cgi_path;
	args[1] = this->file_path;
	size_t start = 0;
	for (int i = 2; i < n; ++i) {
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

void	CGIHandler::prepareCGI(void) {

	std::string uri = request.getUri();
	std::string ext = getExtension(uri);
	if (ext.empty())
		return ;
	this->cgi_path = getPath(ext);
	this->file_path = getFilePath(uri);
	this->args = setArgs();
}
