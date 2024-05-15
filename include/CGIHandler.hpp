/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:38:22 by erivero-          #+#    #+#             */
/*   Updated: 2024/05/15 20:37:31 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include <map>
# include <string>
# include <unistd.h>
# include <csignal>
# include <sys/wait.h>
# include <sys/types.h>
# include <exception>

# include "HTTPRequest.hpp"
# include "LocationConfig.hpp"
# include "utils.hpp"

/**
 * @class CGIHandler
 * @brief Handles the execution of CGI scripts based on HTTP requests and server configurations.
 *
 * This class is responsible for managing CGI processes including setting up the environment, 
 * parsing the HTTP request, determining the script to execute, handling input and output through pipes, 
 * and executing the script while capturing its output.
 */
class CGIHandler
{
    private:
        LocationConfig  config;      ///< Configuration data specific to a server location.
        std::string     root;        ///< Root directory for the server.
        std::string     cgi_path;    ///< Path to the CGI executable.
        std::string     file_path;   ///< Path to the file being processed.
        HTTPRequest     request;     ///< HTTP request being handled.
        char            **args;      ///< Arguments for the CGI script.
        char            **env;       ///< Environment variables for the CGI script.
        int             pipe_fd[2];  ///< File descriptors for the pipe used in CGI script execution.
        bool            cgi;         ///< Flag to check if the current request should be handled as CGI.

    public:
        /**
         * @brief Constructor for CGIHandler.
         * @param config Reference to the LocationConfig object containing location-specific settings.
         * @param req The HTTP request that needs CGI processing.
         * @param root The root directory from which files are served.
         */
        CGIHandler(LocationConfig &config, const HTTPRequest &req, std::string root);

        /**
         * @brief Destructor for CGIHandler.
         */
        ~CGIHandler(void);

        /**
         * @brief Determines the executable path for a given file extension.
         * @param extension A string containing the file extension.
         * @return A string containing the path to the executable associated with the extension.
         */
        std::string getPath(std::string extension);

        /**
         * @brief Prepares the file path from a given URI.
         * @param uri The URI from which the file path will be extracted.
         * @return A string containing the file path derived from the URI.
         */
        std::string prepareFilePath(std::string uri);

        /**
         * @brief Sets up the arguments for the execve system call used to execute the CGI script.
         * @return An array of char pointers representing the arguments for execve.
         */
        char **setArgs(void);


        /**
         * @brief Sets up the environment variables for the CGI script execution.
         */
        void setEnv(void);

        /**
         * @brief Prepares the CGI environment, including paths and arguments for execution.
         */
        void prepareCGI(void);

        /**
         * @brief Executes the CGI script using fork and execve, capturing output via a pipe.
         * @return A string containing the output from the CGI script execution.
         */
        std::string execCGI(void);

        /**
         * @brief Checks if the current request is to be handled as a CGI request.
         * @return True if the request is a CGI request, false otherwise.
         */
        bool isCGI(void);

        class PipeError : std::exception
        {
            public:
                virtual const char *what() const throw();
        };

        class ForkError : std::exception
        {
            public:
                virtual const char *what() const throw();
        };

        class RedirectionError : std::exception
        {
            public:
                virtual const char *what() const throw();
        };

        class TimeoutError : std::exception
        {
            public:
                virtual const char *what() const throw();
        };

        class PipeReadError : std::exception
        {
            public:
                virtual const char *what() const throw();
        };
};

#endif