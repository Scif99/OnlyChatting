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



void sendToServer(sf::TcpSocket& socket)
{
    while (true)
    {
        sf::Packet outgoing;
        std::string out;
        std::getline(std::cin, out);
        outgoing << out;
        socket.send(outgoing);
    }

}

void receiveFromServer(sf::TcpSocket& socket)
{
    while (true)
    {
        // Receive a packet from the server
        sf::Packet incoming;
        if (socket.receive(incoming) == sf::Socket::Done)
        {
            //Extract from packet
            //std::string username;
            std::string message;
            if (incoming >> message) //Extract from packet
            {
                std::cout  << message << '\n';
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


    // Connect to the server
    std::cout << "connecting...\n";
    //if (socket.connect(server, port) == sf::Socket::Disconnected)
    //{
    //    std::cout << "Error: Server is currently full.";
    //}


    if (socket.connect(server, port) != sf::Socket::Done)
    {
        std::cout << "Error: Failed to connect to server.\n";
        return; //error - timeout
    }
    std::cout << "Connected to server\n";

    //We handle input and output simultaneously by using two threads.
    std::thread t{ sendToServer, std::ref(socket) };
    receiveFromServer(socket);

}