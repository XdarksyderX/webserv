/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erivero- <erivero-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:38:22 by erivero-          #+#    #+#             */
/*   Updated: 2024/04/22 16:50:14 by erivero-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "HTTPRequest.hpp"
#include "LocationConfig.hpp"
#include <map>
#include <string>

class CGIHandler {
	private:
		LocationConfig config;
		std::string _uri;
	public:
		CGIHandler(LocationConfig &config, std::string uri);
		std::string getExtension(void);
		std::string getPath(void);

} ;