#pragma once
#include <string>
#include <SFML/Network.hpp>

struct Client
{
	sf::TcpSocket m_socket_;
	std::string m_username_;
};
