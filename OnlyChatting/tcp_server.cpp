#include "tcp_server.h"
#include <memory>

//Echoes a packet sent by a single client back to every client currently connected
void TcpServer::echoToClients(sf::Packet& packet)
{
    
    for (auto& clnt : m_clients_)
    {
        if (clnt->send(packet) != sf::Socket::Done)
        {
            std::cout << "Message failed to send to " << clnt->getRemoteAddress();
        }
    }
}

void TcpServer::run()
{
    //Start listening
    if (m_listener_.listen(m_port_) != sf::Socket::Done)
    {
        std::cout << "Error: unable to listen on port " << m_port_ << '\n';
    }

    std::cout << "\nServer is listening to port " << m_port_ << ", waiting for connections... " << std::endl;
    m_selector_.add(m_listener_);

    
    while (true)
    {
        if (m_selector_.wait()) //Selector passively checks if any of its sockets have received data
        {
            if (m_selector_.isReady(m_listener_)) 
            {
                //Listener is ready to accept a new connection
                processRequest();
            }
            else
            {
                //A client is ready to receive some data
                ProcessClients();
            }
        }
    }
}

//Handles an incoming request from a client
void TcpServer::processRequest()
{
    auto client = std::make_unique<sf::TcpSocket>(); //construct a socket to handle this client
    if (m_listener_.accept(*client) == sf::Socket::Done)
    {
        const bool full{ m_clients_.size() == MAX_CLIENTS };
        std::string message = full ?  "Sorry, server is currently full\n"  : "Welcome to the server!\n";
        sf::Packet packet;
        packet << full<<message;
        client->send(packet);

        if (full) 
        {
            std::cout << "A client has attempted to connect from " << client->getRemoteAddress() << " but server is currently full\n";
            client->disconnect();
        }

        else
        {
            std::cout << "Client connected from: " << client->getRemoteAddress() << std::endl;
            m_selector_.add(*client); //Add to the selector so that it we will be notified when it sends something
            m_clients_.push_back(std::move(client));
            std::cout << "Server now hosts " << m_clients_.size() << " clients\n";
        }
    }
    else
    {
        //Error, connection failed
        std::cout << "Error: Connection failed\n";
    }
  
}

//Checks if any clients have receive data

void TcpServer::ProcessClients()
{
    auto to_be_removed = std::end(m_clients_); //In case we need to remove any clients this iteration

    for (auto it = std::begin(m_clients_); it != std::end(m_clients_);++it)
    {

        if (m_selector_.isReady(**it))
        {
            //One of the clients has sent some data
            sf::Packet packet;
            if ((*it)->receive(packet) == sf::Socket::Done)
            {
                //echo message back to all clients
                // Receive a message back from the client
                std::string message;

                if (packet >> message) //Extract from packet
                {
                    std::cout << message <<'\n';
                    echoToClients(packet); //Inline?
                }
                else { std::cout << "Error reading message\n"; }
            }
            else if ((*it)->receive(packet) == sf::Socket::Disconnected) //If a client has disconnected
            {
                //PROBLEM: CAnnot get username of client after it leaves!
                std::cout << "A user has disconnected from the server\n";
                m_selector_.remove(**it); //Remove socket associated with client from selector
                (*it)->disconnect();
                to_be_removed = it;
                std::cout << "There are now " << m_clients_.size() - 1 << " users in the server\n";

            }

        }

    }

    if (to_be_removed != std::end(m_clients_)) { m_clients_.erase(to_be_removed); } //Remove the client that disconnected, if at al
}