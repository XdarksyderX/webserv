/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erivero- <erivero-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:42:28 by erivero-          #+#    #+#             */
/*   Updated: 2024/05/01 17:57:55 by erivero-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"
// #include <fcntl.h>

CGIHandler::CGIHandler(LocationConfig &conf, const HTTPRequest &req) {
	this->config = conf;
	this->cgi = true;
	this->request = req;
	prepareCGI();
	std::cout << "Por dios Elisa no te olvides de gestionar los errores" << std::endl;
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
	std::cout << "on getPath, ext: \'" << ext << "\'\n";

/* 	it = supportedExtensions.begin();
	while (it != supportedExtensions.end())
	{
		std::cout << it->second << std::endl;
		it++;
	} */
	throw(std::runtime_error("Non supported Extension")); //this is provisional
}

std::string CGIHandler::getFilePath(std::string uri) {
//  what I have:    /cgi-bin/myscript.py?a=42
//  what I want:    ./cgi-bin/myscript.py
	std::string file_path = "/Users/erivero-/Desktop/webserv" + uri;
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

/* If it is POST method, query string must be setted with request body */
char **CGIHandler::setArgs(void) {

	std::string query;
	if (request.getMethod() == POST)
		query = request.getBody();
	else
		query = request.getQuery();
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
	argv[n] = NULL;
	return (argv);
}

void	CGIHandler::prepareCGI(void) {

	std::string uri = request.getUri();
	std::string ext = getExtension(uri);
	std::cout << "on prepareCGI, ext: \'" << ext << "\'\n";
	if (ext.empty())
		return ;
	this->cgi_path = getPath(ext);
	this->file_path = getFilePath(uri);
	if (!Utils::fileExists(file_path))
		throw(std::runtime_error("Requested File doesn't exist")); //this is provisional
	this->args = setArgs();
}

std::string readPipe(int pipe_fd[2]) {

	char buffer[BUFSIZ];
	std::string output;
	ssize_t bytesRead;
/* 	bytesRead = read(pipe_fd[0], buffer, BUFSIZ);
	std::cout << "bytesRead: " << bytesRead << std::endl; */
	while ((bytesRead = read(pipe_fd[0], buffer, BUFSIZ)) > 0) {
		output.append(buffer, bytesRead);
	}
	if (bytesRead < 0) {
		throw (std::runtime_error("Error reading from pipe"));
	}
	close(pipe_fd[0]);
	std::cout << output << std::endl;
	//at this point, output is empty and idk why TT__TT
	return (output);
}

void	waitTimeOut(int pid, int status)
{
/* 	if an infinite loop occurs during the execution of the file,
	it will stop the loop */
	std::time_t start = std::time(nullptr);
	std::time_t current = start;
	while (!waitpid(pid, &status, 0) && current - start < 15)
		current = std::time(nullptr);
	if (current - start > 14)
		kill(pid, SIGKILL);
}

std::string	CGIHandler::execCGI(void) {

	int status = 0;
	 //if (pipe(pipe_fd) < 0) i'll have to handle if pipe fails
	 //if (pid < 0) and if fork fails
	pipe(pipe_fd);
	pid_t pid = fork();
	if (!pid)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			throw(std::runtime_error("Redirection error idk"));
		if (dup2(pipe_fd[1], STDERR_FILENO) == -1)
			throw(std::runtime_error("Redirection error idk"));
        std::cout << "args[0]: " << args[0] << std::endl;  // print the file path
        std::cout << "args: " << args << std::endl;  // print the address of the array
		status = execve(args[0], args, NULL);
		if (status == -1) {
			perror(args[1]);  // print error
		}
		exit(status); //in case execve fails
	}
	std::cout << "status: " << status << std::endl;
	waitTimeOut(pid, status);
//	waitpid(pid, &status, 0); //I may add a counter like waitpid or 15 secs
	close(pipe_fd[1]);
	return (readPipe(pipe_fd));
}


