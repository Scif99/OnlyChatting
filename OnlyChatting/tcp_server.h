#pragma once
#include <SFML/Network.hpp>

#include <array>
#include <memory>
#include <iostream>
#include <functional>


//Note, as of SFML 2.5, sf::TcpSocket is neither copyable nor moveable, so we need to store unique_ptr<sf::TcpSocket> in the vector.

constexpr unsigned short MAX_CLIENTS{ 2 };

class TcpServer
{
public:
	TcpServer(unsigned short port)
		: m_port_{ port }
	{
		m_clients_.reserve(MAX_CLIENTS);
	}

	//non-copyable
	TcpServer(const TcpServer& server) = delete;
	TcpServer& operator=(const TcpServer& server) = delete;

	//moveable?
	//TcpServer(TcpServer&& server) = delete;
	//TcpServer& operator=(TcpServer&& server) = delete;

	void run();
	void processRequest(); //Process an incoming request from a client
	void testClients();
	void echoToClients(sf::Packet& packet);// { std::for_each(std::begin(m_clients_), std::end(m_clients_), [](Client) {std::mem_fn(&sf::TcpSocket::send)} }
private:
	unsigned short m_port_;
	sf::SocketSelector m_selector_;
	std::vector<std::unique_ptr<sf::TcpSocket>> m_clients_; //The extra element is so that, when server is full, we can send a packet to client to say server is full.
	sf::TcpListener m_listener_;


};
