/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 17:43:07 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/05 23:49:02 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <string>
# include <vector>
# include <stack>
# include <cstring>
# include <fstream>
# include <sstream>
# include <exception>

# include "ServerConfig.hpp"

/**
 * @class Parser
 * @brief Class responsible for parsing configuration files to construct ServerConfig objects.
 * 
 * The Parser opens and reads a configuration file, interpreting its content to construct
 * and return instances of ServerConfig that represent the server configurations found within the file.
 */
class Parser
{
    private:
        std::string configFilePath; ///< Path to the configuration file.
        std::ifstream configFile;   ///< File stream for reading the configuration.
        std::vector<ServerConfig> serverConfigs; ///< Vector of ServerConfig objects.

        ServerConfig parseServerBlock(); ///< Parses a server block from the configuration file.
        void processServerLine(const std::string &line, ServerConfig &serverConfig); ///< Processes a line within a server block.
        LocationConfig parseLocationBlock(ServerConfig &serverConfig); ///< Parses a location block within a server block.
        void processLocationLine(const std::string &line, LocationConfig &locationConfig); ///< Processes a line within a location block.
        static void parseLine(std::string &str); ///< Cleans a line from the configuration file removing comments and whitespace.

        // Methods to process specific ServerConfig directives.
        void processListenDirective(std::istringstream &iss, ServerConfig &serverConfig);
        void processHostDirective(std::istringstream &iss, ServerConfig &serverConfig);
        void processServerNameDirective(std::istringstream &iss, ServerConfig &serverConfig);
        void processRootDirective(std::istringstream &iss, ServerConfig &serverConfig);
        void processIndexDirective(std::istringstream &iss, ServerConfig &serverConfig);
        void processErrorPageDirective(std::istringstream &iss, ServerConfig &serverConfig);
        void processClientMaxBodySizeDirective(std::istringstream &iss, ServerConfig &serverConfig);
        void processLocationDirective(std::istringstream &iss, ServerConfig &serverConfig);

        // Methods to process specific LocationConfig directives.
        void processLocationRootDirective(std::istringstream &iss, LocationConfig &locationConfig);
        void processLocationIndexDirective(std::istringstream &iss, LocationConfig &locationConfig);
        void processAllowMethodsDirective(std::istringstream &iss, LocationConfig &locationConfig);
        void processAliasDirective(std::istringstream &iss, LocationConfig &locationConfig);
        void processAutoindexDirective(std::istringstream &iss, LocationConfig &locationConfig);
        void processReturnDirective(std::istringstream &iss, LocationConfig &locationConfig);
        void processCgiPathDirective(std::istringstream &iss, LocationConfig &locationConfig);
        void processCgiExtDirective(std::istringstream &iss, LocationConfig &locationConfig);

    public:
        Parser(const std::string& configFilePath); ///< Constructor that takes the path to the configuration file.
        Parser(const Parser &other); ///< Copy constructor.
        ~Parser(); ///< Destructor.

        Parser &operator=(const Parser &other); ///< Assignment operator.

        std::vector<ServerConfig> &parse(); ///< Parses the configuration file and returns a vector of ServerConfig objects.

        // Exception classes for various error scenarios encountered during parsing.
        class FileNotFoundException : public std::exception
        {
            public:
                virtual const char *what() const throw(); ///< Describes the file not found error.
        };

        class SyntaxErrorException : public std::exception
        {
            public:
                virtual const char *what() const throw(); ///< Describes the syntax error encountered.
        };

        class InvalidDirectiveException : public std::exception
        {
            public:
                virtual const char *what() const throw(); ///< Describes the invalid directive error.
        };

        class MissingMandatoryException : public std::exception
        {
            public:
                virtual const char *what() const throw(); ///< Describes the missing mandatory setting error.
        };
};

#endif

