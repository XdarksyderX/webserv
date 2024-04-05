/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 17:43:07 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/05 21:14:41 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <string>
# include <vector>
# include <cstring>
# include <fstream>
# include <sstream>
# include <exception>

# include "ServerConfig.hpp"

/**
 * @class Parser
 * @brief Class for parsing configuration files and building corresponding configuration objects.
 * 
 * The Parser is responsible for opening and reading a configuration file, interpreting its content
 * to construct and return an instance of ServerConfig that reflects the found configurations.
 */
class Parser
{
    private:
        std::string configFilePath; ///< Path to the configuration file.
        std::ifstream configFile; ///< File stream to read the configuration file.
		std::vector<ServerConfig> serverConfigs; ///< Vector of ServerConfig objects.
        const static char commentChar = '#'; ///< Character used to denote comments in the configuration file.
        /**
         * @brief Parse a server block from the configuration file.
         * 
         * This function parses a block of configuration specific to a server,
         * extracting and setting various server-related parameters such as port,
         * host, server name, root directory, index file, and specific location
         * configurations.
         * 
         * @return ServerConfig The parsed server configuration.
         */
        ServerConfig parseServerBlock();

        /**
         * @brief Parse a location block from the configuration file.
         * 
         * This function parses a block of configuration specific to a location (URI path)
         * within a server, extracting and setting various parameters such as root directory,
         * allowed methods, index file, autoindex setting, and redirection rules.
         * 
         * @param serverConfig Reference to the current server configuration object.
         * @return LocationConfig The parsed location configuration.
         */
        LocationConfig parseLocationBlock(ServerConfig &serverConfig);

        /**
         * @brief Process and clean a line of configuration.
         * 
         * This static function takes a string and processes it by removing comments,
         * trailing semicolons, and leading/trailing whitespace. It's used to preprocess
         * each line of the configuration file before parsing.
         * 
         * @param str The line of configuration to be processed and cleaned.
         */
        static void parseLine(std::string &str);

    public:
        /**
         * @brief Constructor for Parser that takes the path to the configuration file.
         * @param configFilePath Path to the configuration file to be parsed.
         */
        Parser(const std::string& configFilePath);

        /**
         * @brief Copy constructor.
         * @param other Another Parser object from which to copy data.
         */
        Parser(const Parser &other);

        /**
         * @brief Destructor for Parser.
         */
        ~Parser();

        /**
         * @brief Assignment operator.
         * @param other Another Parser object from which to copy data.
         * @return Reference to this Parser object.
         */
        Parser &operator=(const Parser &other);

        /**
         * @brief Parses the configuration file and returns a ServerConfig object.
         * @return A ServerConfig vector object with the parsed server configurations from the file.
         * @throws FileNotFoundException If the configuration file cannot be opened.
         * @throws SyntaxErrorException If there is a syntax error in the configuration file.
         * @throws InvalidDirectiveException If an invalid directive is encountered.
         * @throws InvalidValueException If an invalid value is found for a configuration.
         * @throws MissingMandatoryException If a mandatory configuration is missing.
         * @throws BlockMismatchException If there is a mismatch in the configuration blocks.
         * @throws ConfigurationException If there is a general error in the configuration.
         */
        std::vector<ServerConfig>	&parse();

        /**
         * @class FileNotFoundException
         * @brief Exception thrown when the configuration file cannot be found or opened.
         */
        class FileNotFoundException : public std::exception
        {
            public:
                /**
                 * @brief Provides a description of the error.
                 * @return A character string describing the error.
                 */
                virtual const char *what() const throw();
        };

        /**
         * @class SyntaxErrorException
         * @brief Exception thrown when there is a syntax error in the configuration file.
         */
        class SyntaxErrorException : public std::exception
        {
            public:
                /**
                 * @brief Provides a description of the error.
                 * @return A character string describing the error.
                 */
                virtual const char *what() const throw();
        };

        /**
         * @class InvalidDirectiveException
         * @brief Exception thrown when an invalid directive is found in the configuration file.
         */
        class InvalidDirectiveException : public std::exception
        {
            public:
                /**
                 * @brief Provides a description of the error.
                 * @return A character string describing the error.
                 */
                virtual const char *what() const throw();
        };

        /**
         * @class InvalidValueException
         * @brief Exception thrown when an invalid value is found for a configuration setting.
         */
        class InvalidValueException : public std::exception
        {
            public:
                /**
                 * @brief Provides a description of the error.
                 * @return A character string describing the error.
                 */
                virtual const char *what() const throw();
        };

        /**
         * @class MissingMandatoryException
         * @brief Exception thrown when a mandatory configuration setting is missing.
         */
        class MissingMandatoryException : public std::exception
        {
            public:
                /**
                 * @brief Provides a description of the error.
                 * @return A character string describing the error.
                 */
                virtual const char *what() const throw();
        };

        /**
         * @class BlockMismatchException
         * @brief Exception thrown when there is a mismatch in the configuration blocks.
         */
        class BlockMismatchException : public std::exception
        {
            public:
                /**
                 * @brief Provides a description of the error.
                 * @return A character string describing the error.
                 */
                virtual const char *what() const throw();
        };

        /**
         * @class ConfigurationException
         * @brief Exception thrown when there is a general error in the configuration.
         */
        class ConfigurationException : public std::exception
        {
            public:
                /**
                 * @brief Provides a description of the error.
                 * @return A character string describing the error.
                 */
                virtual const char *what() const throw();
        };
};

#endif
