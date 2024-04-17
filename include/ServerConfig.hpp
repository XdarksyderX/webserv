/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 17:18:42 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/17 11:12:44 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIG_HPP
# define SERVER_CONFIG_HPP

# include <iostream>
# include <string>
# include <map>

# include "LocationConfig.hpp"

class LocationConfig;

/**
 * @brief ServerConfig class holds the configuration for a single server.
 * 
 * This class encapsulates all the server configuration details such as port, host,
 * server name, root directory, and specific location configurations.
 */
class ServerConfig
{
    private:
        unsigned short port; ///< Port number where the server will listen.
        std::string host; ///< Hostname or IP address where the server is hosted.
        std::string serverName; ///< Name of the server.
        std::string root; ///< Root directory for the server files.
        std::string index; ///< Default index file name.
        std::map<int, std::string> errorPages; ///< Mapping of error codes to error page paths.
        size_t clientMaxBodySize; ///< Maximum allowed size for client request body.
        std::map<std::string, LocationConfig> locations; ///< Location-specific configurations.
        std::string  uploadsDirectory; ///< Directory where uploaded files are stored.

    public:
        ServerConfig();
        ServerConfig(const ServerConfig &other);
        ~ServerConfig();

        ServerConfig &operator=(const ServerConfig &other);

        // Getters
        unsigned short getPort() const;
        std::string getHost() const;
        std::string getServerName() const;
        std::string getRoot() const;
        std::string getIndex() const;
        std::map<int, std::string> getErrorPages() const;
        size_t getClientMaxBodySize() const;
        std::map<std::string, LocationConfig> getLocations() const;
        std::string getUploadsDirectory() const;

        // Setters
        void setPort(unsigned short p);
        void setHost(const std::string& h);
        void setServerName(const std::string& name);
        void setRoot(const std::string& r);
        void setIndex(const std::string& i);
        void setErrorPages(const std::map<int, std::string>& pages);
        void setClientMaxBodySize(size_t size);
        void setLocations(const std::map<std::string, LocationConfig>& locs);
        void setUploadsDirectory(const std::string& dir);

        /**
         * @brief Add or update an error page path for a specific error code.
         * 
         * @param errorCode The HTTP error code.
         * @param path The path to the error page.
         */
        void addErrorPage(int errorCode, const std::string& path);

        /**
         * @brief Add or update a location configuration.
         * 
         * @param path The URI path for the location.
         * @param config The configuration for this location.
         */
        void addLocation(const std::string& path, const LocationConfig& config);
        
        /**
         * @brief Output stream operator overload to print the server configuration.
         */
        friend std::ostream& operator<<(std::ostream& os, const ServerConfig& config);

};

#endif
