/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponseBuilder.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 14:35:19 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/06 15:56:05 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSEBUILDER_HPP
# define HTTPRESPONSEBUILDER_HPP

# include <sstream>
# include "HTTPResponse.hpp"

class HTTPResponseBuilder
{
	private:
		HTTPResponseBuilder();
		static void	buildStatusLine(std::string &builResponse, const HTTPResponse &response);
		static void	buildHeaders(std::string &buildResponse, const HTTPResponse &response);

	public:
		static std::string buildResponse(const HTTPResponse &response);
};

#endif
