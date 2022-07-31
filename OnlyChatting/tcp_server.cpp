#include "tcp_server.h"
#include <memory>

//Echoes a packet sent by a single client back to every client currently connected
void TcpServer::echoToClients(sf::Packet& packet)
{
    
    for (auto& clnt : m_clients_)
    {
        clnt->send(packet);
    }
}

void TcpServer::run()
{
    //Start listening
    m_listener_.listen(m_port_);
    std::cout << "Server is listening to port " << m_port_ << ", waiting for connections... " << std::endl;
    m_selector_.add(m_listener_);

    //Selector passively checks if any of its sockets have received data
    while (true)
    {
        if (m_selector_.wait()) //check if any of the sockets are ready to recieve
        {
            if (m_selector_.isReady(m_listener_)) 
            {
                //Listener is ready to accept a new connection
                processRequest();
            }
            else
            {
                //A client is ready to receive some data
                testClients();
            }

        }
    }
}

//
void TcpServer::processRequest()
{
    if (m_clients_.size() < MAX_CLIENTS) //Ignore request if server is full
    {
        auto client = std::make_unique<sf::TcpSocket>(); //construct a socket to handle this client
        if (m_listener_.accept(*client) == sf::Socket::Done)
        {

            std::cout << "Client connected from: " << client->getRemoteAddress() << std::endl;
            m_selector_.add(*client); //Add to the selector so that it we will be notified when it sends something
            m_clients_.push_back(std::move(client));
            std::cout << "Server now hosts " << m_clients_.size() << " clients\n";
        }
        else
        {
            //Error, connection failed
            std::cout << "Error: Connection failed\n";
        }
    }
}


void TcpServer::testClients()
{
    //auto to_be_removed = std::end(m_clients_); //In case we need to remove any clients this iteration

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
                    std::cout << message << '\n'; //Send 
                    echoToClients(packet); //Inline?
                }
            }
            //else if (it->receive(packet) == sf::Socket::Disconnected) //If a client has disconnected
            //{
            //    //PROBLEM: CAnnot get username of client after it leaves!
            //    std::cout << (*it)->m_username_ << " has disconnected from the server\n";
            //    m_selector_.remove((*it)->m_socket_); //Remove socket associated with client from selector
            //    (*it)->m_socket_.disconnect();
            //    to_be_removed = it;
            //    std::cout << "There are now " << m_clients_.size() - 1 << " users in the server\n";

            //}

        }

    }

    //if (to_be_removed != std::end(m_clients_)) { m_clients_.erase(to_be_removed); } //Remove the client that disconnected, if at al
}