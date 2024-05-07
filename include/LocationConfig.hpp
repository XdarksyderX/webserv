/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 17:20:30 by migarci2          #+#    #+#             */
/*   Updated: 2024/05/07 20:36:31 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_CONFIG_HPP
# define LOCATION_CONFIG_HPP

# include <string>
# include <iostream>
# include <map>
# include <vector>

# include "ServerConfig.hpp"
# include "HTTPRequest.hpp"


class ServerConfig;

/**
 * @brief LocationConfig class holds the configuration specific to a location (URI path) within a server.
 * 
 * This class encapsulates details such as the root directory for the location, allowed methods,
 * default index file, autoindex setting, and CGI configuration.
 */
class LocationConfig
{
	private:
		std::string name; ///< The name of the location.
		std::string root; ///< The root directory for this location.
		std::vector<HTTPMethod> allowMethods; ///< Allowed HTTP methods for this location.
		std::string index; ///< Default index file for this location.
		bool autoindex; ///< Flag to enable or disable directory listing.
		std::string uploadPath; ///< Path for file uploads.
		std::vector<std::string> cgiPaths; ///< Path to the CGI interpreter.
		std::vector<std::string> cgiExtensions; ///< Extension for CGI scripts.


	public:
		LocationConfig();
		LocationConfig(const std::string &rootPath, const std::string &name);
		LocationConfig(const LocationConfig &other);
		~LocationConfig();

		LocationConfig &operator=(const LocationConfig &other);

		// Getters
		std::string getName() const;
		std::string getRoot() const;
		std::vector<HTTPMethod> getAllowMethods() const;
		std::string getIndex() const;
		std::vector<std::string> getCgiPaths() const;
		std::vector<std::string> getCgiExtensions() const;
		bool getAutoindex() const;
		std::map<std::string, std::string> getCgiInfo() const;
		std::string getUploadPath() const;

		// Setters
		void setName(const std::string& name);
		void setRoot(const std::string& root);
		void setAllowMethods(const std::vector<HTTPMethod>& methods);
		void setIndex(const std::string& index);
		void setAutoindex(bool autoindex);
		void setUploadPath(const std::string &uploadPath);
		void setCgiPaths(const std::vector<std::string> &cgiPaths);
		void setCgiExtensions(const std::vector<std::string> &cgiExtensions);

		// Additional methods
		void addAllowMethod(HTTPMethod method);

		/**
		 * @brief Creates a default LocationConfig object with default values for /.
		 * @return A LocationConfig object with default values.
		 */
		static LocationConfig	createDefaultLocation(const ServerConfig &serverConfig);

		friend std::ostream& operator<<(std::ostream& os, const LocationConfig& config);
};

#endif
