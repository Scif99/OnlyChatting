////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Network.hpp>

#include <iomanip>
#include <iostream>
#include <optional>
#include <utility>
#include <iterator>

#include <chrono>
#include <ctime>  
#include <string>
#include <thread>

#include "message.h"


void sendToServer(Client& client)
{
    while (true)
    {
        sf::Packet outgoing;
        std::string out;
        std::getline(std::cin, out);
        outgoing << client.m_username_<< out;
        client.m_socket_.send(outgoing);
    }

}

void receiveFromServer(Client& client)
{
    while (true)
    {
        // Receive a packet from the server
        sf::Packet incoming;
        if (client.m_socket_.receive(incoming) == sf::Socket::Done)
        {
            //Extract from packet
            std::string username;
            std::string message;
            if (incoming >>username >> message) //Extract from packet
            {
                std::cout << username << ": " << message << '\n';
            }
        }
    }
}

////////////////////////////////////////////////////////////
/// Create a client, connect it to a server, display the
/// welcome message and send an answer.
////////////////////////////////////////////////////////////
void runTcpClient(unsigned short port)
{
    // Ask for the server address
    sf::IpAddress server;
    std::cout << "Type the address or name of the server to connect to: ";
    std::cin >> server;
    std::cin.ignore();

    std::cout << "Please enter a username: \n";
    std::string name;
    std::cin >> name;

    // Create a socket for communicating with the server
    sf::TcpSocket socket;

    Client client;
    client.m_username_ = name;

    // Connect to the server
    std::cout << "connecting...\n";
    if (client.m_socket_.connect(server, port) == sf::Socket::Disconnected)
    {
        std::cout << "Error: Server is currently full.";
    }

    sf::Time t1 = sf::seconds(5.f);
    if (client.m_socket_.connect(server, port, t1) != sf::Socket::Done)
    {
        std::cout << "Error: timeout.\n";
        return; //error - timeout
    }
    client.m_socket_.connect(server, port,t1);
    std::cout  << client.m_username_<<" has connected to the server \n";

    //We handle input and output simultaneously by using two threads.
    std::thread t{ sendToServer, std::ref(client) };
    receiveFromServer(client);

}