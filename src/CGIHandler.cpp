/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erivero- <erivero-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:42:28 by erivero-          #+#    #+#             */
/*   Updated: 2024/04/22 16:50:55 by erivero-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"

CGIHandler::CGIHandler(LocationConfig &conf, std::string uri) {
	this->config = conf;
	this->_uri = uri;
}

std::string CGIHandler::getExtension(void) {

	size_t dotPos = _uri.find_last_of('.');
	if (dotPos != std::string::npos)
		return (_uri.substr(dotPos));
	return ("");
}

std::string CGIHandler::getPath(void) {

	std::string ext = getExtension();

	if (!ext.empty())
	{
		std::map<std::string, std::string> supportedExtensions = config.getCgiInfo();
		std::map<std::string, std::string>::iterator it;
		it = supportedExtensions.find(ext);
		if (it != supportedExtensions.end())
			return (it->second);
	}
	return ("");
}