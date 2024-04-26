/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erivero- <erivero-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:38:22 by erivero-          #+#    #+#             */
/*   Updated: 2024/04/26 17:30:36 by erivero-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "HTTPRequest.hpp"
#include "LocationConfig.hpp"
#include "utils.hpp"
#include <map>
#include <string>
#include <unistd.h> //for pipe
#include <signal.h> //for kill

class CGIHandler {
	private:
		LocationConfig	config;
	//	std::string	uri;
	//	std::string	query;
		bool		cgi;
		std::string	cgi_path;
		std::string	file_path;
		HTTPRequest	request;
		char **args;
		int		pipe_fd[2];
	public:
		CGIHandler(LocationConfig &config, const HTTPRequest &req);
		~CGIHandler(void);
		/* @brief Takes an uri and returns a sting with the file extension if any*/
		std::string getExtension(std::string uri);
		/* @brief Returns the path to execute a supported type of extension
			param A string with the file extension*/
		std::string getPath(std::string extension);
		/* @brief Takes an uri and return the file path*/
		std::string getFilePath(std::string uri);
		/* Sets the **char that execve will need to execute the requested file */
		char **setArgs(void);
		/* Sets cgi_path, file_path, args so everything is ready to execute*/
		void prepareCGI(void);
		/* Creates a fork to execute the file, saving the content in a pipe. Returns
		the pipe content in a string */
		std::string	 execCGI(void);
		std::vector<std::string> getArgs(void);

} ;