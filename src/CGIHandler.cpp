/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erivero- <erivero-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:42:28 by erivero-          #+#    #+#             */
/*   Updated: 2024/05/07 15:43:56 by erivero-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"

CGIHandler::CGIHandler(LocationConfig &conf, const HTTPRequest &req, std::string server_root) {
	this->config = conf;
	this->cgi = true;
	this->request = req;
	this->root = server_root;
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
	size_t queryPos = uri.find_last_of('?');
	if (dotPos != std::string::npos && queryPos != std::string::npos)
		return (uri.substr(dotPos, queryPos - dotPos));
	else if (dotPos != std::string::npos)
		return (uri.substr(dotPos));
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

std::string CGIHandler::prepareFilePath(std::string uri) 
{
//  what I have:    /cgi-bin/myscript.py?a=42
//  what I want:    ./cgi-bin/myscript.py
	std::string clean_uri = Utils::cleanQueryFromPath(uri);
	std::string path = "./" + Utils::joinPaths(root, clean_uri);

	return (path);
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
void ft_debuggin(char **args)
{
	int i = 0;
	std::cout << "\033[35m[ Debugging ]\033[0m\n";
	while (args[i]) {
		std::cout << i << ": " << args[i] << std::endl;
		i++;
	}
}
/* If it is POST method, query string must be setted with request body */
std::vector<std::string> split(const std::string &s, char delimiter) {
	std::vector<std::string> args;
	std::string arg;
	std::istringstream argStream(s);
	while (std::getline(argStream, arg, delimiter)) {
		args.push_back(arg);
	}
	return (args);
}

char **CGIHandler::setArgs(void) {

	std::string query;
	if (request.getMethod() == POST)
		query = request.getBody();
	else
		query = request.getQuery();
	std::vector<std::string> v_args = split(query, '&');
 	v_args.insert(v_args.begin(), this->file_path);
	v_args.insert(v_args.begin(), this->cgi_path);
	int n = v_args.size();
	char **argv = new char*[n + 1];
	for (int i = 0; i < n; ++i) {
		argv[i] = new char[v_args[i].size() + 1];
		std::strcpy(argv[i], v_args[i].c_str());
	}
	argv[n] = NULL;
	return (argv);
}

void	CGIHandler::prepareCGI(void) {

	std::string uri = request.getUri();
	std::string ext = Utils::getExtensionFromFile(uri);
	if (ext.empty() || ext == "html") {
		this->cgi = false;
		return ;
	}
	this->cgi_path = getPath("." + ext);
	this->file_path = prepareFilePath(uri);
	std::cout << "file path: \'" << file_path << "\'\n";
	if (!Utils::fileExists(file_path))
		throw(std::runtime_error("Requested File doesn't exist")); //this is provisional
	this->args = setArgs();
	ft_debuggin(args);
}

std::string readPipe(int pipe_fd[2]) {

	char buffer[BUFSIZ];
	std::string output;
	ssize_t bytesRead;
	while ((bytesRead = read(pipe_fd[0], buffer, BUFSIZ)) > 0) {
		output.append(buffer, bytesRead);
	}
	if (bytesRead < 0) {
		throw (std::runtime_error("Error reading from pipe"));
	}
	close(pipe_fd[0]);
	std::cout << "on readPipe output: " << output << std::endl;
	return (output);
}

void	waitTimeOut(int pid, int status)
{
/* 	if an infinite loop occurs during the execution of the file,
	it will stop the loop */
	std::time_t start = std::time(0);
	std::time_t current = start;
	while (!waitpid(pid, &status, WNOHANG) && current - start < 10)
		current = std::time(0);
	if (current - start > 9)
	{
		kill(pid, SIGKILL);
		throw (std::runtime_error("[TIMEOUT] Infinite loop ocurred during execution"));
	}
}

std::string	CGIHandler::execCGI(void) {

	int status = 0;
	 if (pipe(pipe_fd) < 0)
		throw (std::runtime_error("Error while creating a pipe"));
	pipe(pipe_fd);
	pid_t pid = fork();
	if (pid < 0)
		throw (std::runtime_error("Error while creating a fork"));
	if (!pid)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			throw(std::runtime_error("Error while redirecting output"));
		if (dup2(pipe_fd[1], STDERR_FILENO) == -1)
			throw(std::runtime_error("Error while redirecting output"));
		status = execve(args[0], args, NULL);
		if (status == -1) {
			perror(args[1]);  // print error
		}
		exit(status); //in case execve fails
	}
	std::cout << "status: " << status << std::endl;
	waitTimeOut(pid, status);
	close(pipe_fd[1]);
	return (readPipe(pipe_fd));
}
