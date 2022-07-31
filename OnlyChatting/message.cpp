#pragma once
#include <SFML/Network.hpp>

#include <string>

struct Message
{
	std::string m_name_;
	std::string m_text_;
};

sf::Packet& operator<<(sf::Packet& packet, const Message& message)
{
	return packet << message.m_name_ << message.m_text_;
}


sf::Packet& operator>(sf::Packet& packet,  Message& message)
{
	packet >> message.m_name_ >> message.m_text_;
	return packet;
}