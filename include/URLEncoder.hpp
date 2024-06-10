/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URLEncoder.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 14:58:26 by migarci2          #+#    #+#             */
/*   Updated: 2024/06/10 14:59:01 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef URL_ENCODER_HPP
# define URL_ENCODER_HPP

# include <string>

class URLEncoder
{
	public:
		static std::string encode(const std::string &str);
		static std::string decode(const std::string &str);
};

#endif