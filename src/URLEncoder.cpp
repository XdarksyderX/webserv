/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URLEncoder.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 14:58:13 by migarci2          #+#    #+#             */
/*   Updated: 2024/06/10 15:01:18 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "URLEncoder.hpp"

std::string URLEncoder::encode(const std::string &str)
{
	std::string encoded;
	for (size_t i = 0; i < str.length(); i++)
	{
		if (isalnum(str[i]) || str[i] == '-' || str[i] == '_' || str[i] == '.' || str[i] == '~')
			encoded += str[i];
		else
		{
			encoded += '%';
			encoded += "0123456789ABCDEF"[((unsigned char)str[i]) / 16];
			encoded += "0123456789ABCDEF"[((unsigned char)str[i]) % 16];
		}
	}
	return encoded;
}

static int from_hex(char c)
{
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'A' && c <= 'F') return c - 'A' + 10;
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	return -1;
}

std::string URLEncoder::decode(const std::string &str)
{
	std::string decoded;
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == '%' && i + 2 < str.length())
		{
			int high = from_hex(str[++i]);
			int low = from_hex(str[++i]);
			if (high != -1 && low != -1)
				decoded += (high * 16 + low);
		}
		else
		{
			decoded += str[i];
		}
	}
	return decoded;
}
