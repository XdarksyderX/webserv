/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erivero- <erivero-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:38:22 by erivero-          #+#    #+#             */
/*   Updated: 2024/04/24 15:04:39 by erivero-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "HTTPRequest.hpp"
#include "LocationConfig.hpp"
#include <map>
#include <string>

class CGIHandler {
	private:
		LocationConfig	config;
	//	std::string	uri;
	//	std::string	query;
		std::string	path;
		HTTPRequest	request;
		char **args;
	public:
		CGIHandler(LocationConfig &config, HTTPRequest &req);
		~CGIHandler(void);
		//if it has an extension, will return it, if not, will return an empty string
		std::string getExtension(void);
		/* @brief if the uri contains one of the supported extensions, will get the path to it */
		std::string getPath(void);
		/* @brief Sets a **char with the path and the arguments to execute */
		char **setArgs(void);

} ;