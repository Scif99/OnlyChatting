#pragma once
#include <SFML/Network.hpp>

#include <vector>
#include <memory>
#include <iostream>
#include <functional>

#include "message.h"


constexpr unsigned short MAX_CLIENTS{ 4 };

class TcpServer
{
public:
	TcpServer(unsigned short port)
		: m_port_{port}
	{
		m_clients_.reserve(MAX_CLIENTS);
	}

	void run();
	void processRequest();
	void testClients();
	void echoToClients(sf::Packet& packet);// { std::for_each(std::begin(m_clients_), std::end(m_clients_), [](Client) {std::mem_fn(&sf::TcpSocket::send)} }
private:
	unsigned short m_port_;
	sf::SocketSelector m_selector_;
	std::vector<std::unique_ptr<Client>> m_clients_;
	sf::TcpListener m_listener_;

};
