/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:42:28 by erivero-          #+#    #+#             */
/*   Updated: 2024/05/08 17:42:00 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"

CGIHandler::CGIHandler(LocationConfig &conf, const HTTPRequest &req, std::string server_root)
{
	this->config = conf;
	this->cgi = true;
	this->request = req;
	this->root = server_root;
	prepareCGI();
}

CGIHandler::~CGIHandler()
{

	if (this->cgi)
		delete [] args;
}

std::string CGIHandler::getPath(std::string ext)
{
	if (ext.empty())
		return ("");
	std::map<std::string, std::string> supportedExtensions = config.getCgiInfo();
	std::map<std::string, std::string>::iterator it;
	it = supportedExtensions.find(ext);
	if (it != supportedExtensions.end())
		return (it->second);
	this->cgi = false;
	return ("");
}

std::string CGIHandler::prepareFilePath(std::string uri) 
{
	std::string clean_uri = Utils::cleanQueryFromPath(uri);
	std::string path = "./" + Utils::joinPaths(root, clean_uri);

	return (path);
}

std::vector<std::string> split(const std::string &s, char delimiter)
{
	std::vector<std::string> args;
	std::string arg;
	std::istringstream argStream(s);
	while (std::getline(argStream, arg, delimiter)) {
		args.push_back(arg);
	}
	return (args);
}

char **CGIHandler::setArgs(void)
{

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
	for (int i = 0; i < n; ++i)
	{
		argv[i] = new char[v_args[i].size() + 1];
		std::strcpy(argv[i], v_args[i].c_str());
	}
	argv[n] = NULL;
	return (argv);
}

void	CGIHandler::prepareCGI(void)
{
	std::string uri = request.getUri();
	std::string ext = Utils::getExtensionFromFile(uri);
	if (ext.empty() || ext == "html")
	{
		this->cgi = false;
		return ;
	}
	this->cgi_path = getPath(ext);
	this->file_path = prepareFilePath(uri);
	if (this->cgi_path.empty() || this->file_path.empty())
	{
		this->cgi = false;
		return ;
	}
	if (!Utils::fileExists(file_path))
		this->cgi = false;
	this->args = setArgs();
}

std::string readPipe(int pipe_fd[2])
{

	char buffer[BUFSIZ];
	std::string output;
	ssize_t bytesRead;
	while ((bytesRead = read(pipe_fd[0], buffer, BUFSIZ)) > 0)
		output.append(buffer, bytesRead);
	if (bytesRead < 0)
		throw CGIHandler::PipeReadError();
	close(pipe_fd[0]);
	return (output);
}

void	waitTimeOut(int pid, int status)
{
	time_t start = time(NULL);
	time_t current = start;
	while (!waitpid(pid, &status, WNOHANG) && current - start < 10)
		current = time(NULL);
	if (current - start > 9)
	{
		kill(pid, SIGKILL);
		throw CGIHandler::TimeoutError();
	}
}

std::string	CGIHandler::execCGI(void)
{

	int status = 0;
	if (pipe(pipe_fd) < 0)
		throw PipeError();
	pipe(pipe_fd);
	pid_t pid = fork();
	if (pid < 0)
		throw ForkError();
	if (!pid)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			throw RedirectionError();
		if (dup2(pipe_fd[1], STDERR_FILENO) == -1)
			throw RedirectionError();
		status = execve(args[0], args, NULL);
		if (status == -1)
			perror(args[1]);
		exit(status);
	}
	waitTimeOut(pid, status);
	close(pipe_fd[1]);
	return (readPipe(pipe_fd));
}

bool	CGIHandler::isCGI(void)
{
	return (this->cgi);
}

const char *CGIHandler::PipeError::what() const throw()
{
	return ("Error while creating a pipe");
}

const char *CGIHandler::PipeReadError::what() const throw()
{
	return ("Error reading from pipe");
}

const char *CGIHandler::ForkError::what() const throw()
{
	return ("Error while creating a fork");
}

const char *CGIHandler::RedirectionError::what() const throw()
{
	return ("Error while redirecting output");
}

const char *CGIHandler::TimeoutError::what() const throw()
{
	return ("Infinite loop ocurred during execution");
}
