/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPCookie.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 22:37:23 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/21 00:05:49 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCOOKIE_HPP
# define HTTPCOOKIE_HPP

# include <string>
# include <cstdlib>
# include <ctime>

# include "utils.hpp"

class HTTPCookie
{
	private:
		int			key;
		time_t		time;
	
	public:
		HTTPCookie();
		HTTPCookie(const HTTPCookie &other);
		~HTTPCookie();

		HTTPCookie &operator=(const HTTPCookie &other);

		int			getKey() const;
		time_t		getTime() const;

		static std::string	serializeCookie(const HTTPCookie &cookie);
		static HTTPCookie	deserializeCookie(const std::string &cookie);
};

#endif
