/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 14:36:49 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/06 15:30:23 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTPARSER_HPP
# define HTTPREQUESTPARSER_HPP

# include <exception>
# include "HTTPRequest.hpp"

class HTTPRequestParser
{
	private:
		HTTPRequestParser();
		static void	parseRequestHeaders(HTTPRequest &request, const std::string &headerLine);
		static void	parseRequestLine(HTTPRequest &request, const std::string &requestLine);

	public:
		static HTTPRequest parseRequest(const std::string &rawRequest);
		class InvalidRequestLine : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};
		class InvalidRequestHeader : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};
		
};

#endif
