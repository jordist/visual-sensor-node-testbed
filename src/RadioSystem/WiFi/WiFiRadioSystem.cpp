/*
 * WiFiRadioSystem.cpp
 *
 *  Created on: 16/set/2014
 *      Author: greeneyes
 */

#include "WiFiRadioSystem.h"
#include <boost/bind.hpp>

WiFiRadioSystem::WiFiRadioSystem(tcp::resolver::query, std::string mode) :
io_service(),
acceptor(io_service),
socket(io_service),
connection_manager(),
new_connection(new Connection(io_service,
		connection_manager))
{
	tcp::resolver resolver(io_service);
	tcp::resolver::query query("localhost", "1234");
	tcp::endpoint endpoint = *resolver.resolve(query);
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

	if(mode.compare("server") == 0){
		//open te acceptor
		std::cout << "server initializing..." << std::endl;
		acceptor.open(endpoint.protocol());
		acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		acceptor.bind(endpoint);
		acceptor.listen();
		acceptor.async_accept(new_connection->socket(),
				boost::bind(&WiFiRadioSystem::handleAccept, this,
						boost::asio::placeholders::error));
	}

	if(mode.compare("client") == 0){
		std::cout << "connecting..." << std::endl;
		boost::asio::async_connect(socket, endpoint_iterator,
				boost::bind(&WiFiRadioSystem::handleConnect, this,
						boost::asio::placeholders::error, endpoint_iterator));
	}
}

void WiFiRadioSystem::startReceiver(){
	io_service.run();
}

void WiFiRadioSystem::handleConnect(const boost::system::error_code& error,
		tcp::resolver::iterator endpoint_iter){
	if (!socket.is_open())
	{
		std::cout << "Connect timed out\n";
	}

	else if (error)
	{
		std::cout << "Connect error: " << error.message() << "\n";

		socket.close();
	}
	// Otherwise we have successfully established a connection.
	else
	{
		std::cout << "Connected to " << endpoint_iter->endpoint() << "\n";

		// Start reading the header
		std::cout << "Start reading..." << std::endl;
		//start_read_header();
	}
}

void WiFiRadioSystem::handleAccept(const boost::system::error_code& e){
	if (!e)
	{
		std::cout << "connected..." << std::endl;
		connection_manager.start(new_connection);
		new_connection = new Connection(io_service,
				connection_manager);
		acceptor.async_accept(new_connection->socket(),
				boost::bind(&WiFiRadioSystem::handleAccept, this,
						boost::asio::placeholders::error));

	}
}
