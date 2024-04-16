/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 17:20:30 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/16 23:47:33 by migarci2         ###   ########.fr       */
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
		std::string root; ///< The root directory for this location.
		std::vector<HTTPMethod> allowMethods; ///< Allowed HTTP methods for this location.
		std::string index; ///< Default index file for this location.
		bool autoindex; ///< Flag to enable or disable directory listing.
		std::string returnPath; ///< Path for redirection.
		std::string alias; ///< Alias for the location.
		std::vector<std::string> cgiPaths; ///< Path to the CGI interpreter.
		std::vector<std::string> cgiExtensions; ///< Extension for CGI scripts.


	public:
		LocationConfig();
		LocationConfig(std::string rootPath);
		LocationConfig(const LocationConfig &other);
		~LocationConfig();

		LocationConfig &operator=(const LocationConfig &other);

		// Getters
		std::string getRoot() const;
		std::vector<HTTPMethod> getAllowMethods() const;
		std::string getIndex() const;
		bool getAutoindex() const;
		std::string getReturnPath() const;
		std::string getAlias() const;
		std::map<std::string, std::string> getCgiInfo() const;

		// Setters
		void setRoot(const std::string& root);
		void setAllowMethods(const std::vector<HTTPMethod>& methods);
		void setIndex(const std::string& index);
		void setAutoindex(bool autoindex);
		void setReturnPath(const std::string& path);
		void setAlias(const std::string& alias);
		std::vector<std::string> getCgiPaths() const;
		std::vector<std::string> getCgiExtensions() const;

		// Additional methods
		void addAllowMethod(HTTPMethod method);
		void addCgiPath(const std::string& interpreterPath);
		void addCgiExtension(const std::string& extension);

		/**
		 * @brief Creates a default LocationConfig object with default values for /.
		 * @return A LocationConfig object with default values.
		 */
		static LocationConfig	createDefaultLocation(const ServerConfig &serverConfig);

		friend std::ostream& operator<<(std::ostream& os, const LocationConfig& config);
};

#endif
