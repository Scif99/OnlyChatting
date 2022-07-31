////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstdlib>
#include <iostream>
#include <thread>
#include <limits>
#include "tcp_server.h"



constexpr unsigned short PORT{ 50001 };

struct Message;
void runTcpClient(unsigned short port);

////////////////////////////////////////////////////////////
/// Entry point of application
/// \return Application exit code
////////////////////////////////////////////////////////////

int main()
{
    // Choose an arbitrary port for opening sockets
    const unsigned short port = 50001;

    std::cout << "Hello, welcome to the chat room.\n";

    // Client or server ?
    std::string who;
    std::cout << "Do you want to be a server (s) or a client (c)? ";
    std::cin >> who;
    while (who != "s" && who != "c")
    {
        std::cout << "Error: Invalid input\nDo you want to be a server(s) or a client(c) ? ";
        std::cin >> who;
    }
 

    // Test the TCP protocol
    if (who == "s")
    {
        TcpServer server(port);
        server.run();   

    }
    else 
    {
        runTcpClient(port);
    }
  


    // Wait until the user presses 'enter' key
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore(10000, '\n');
    std::cin.ignore(10000, '\n');

    return 0;
}