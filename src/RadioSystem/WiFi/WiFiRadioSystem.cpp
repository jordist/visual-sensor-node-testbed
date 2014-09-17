/*
 * WiFiRadioSystem.cpp
 *
 *  Created on: 16/set/2014
 *      Author: greeneyes
 */

#include "WiFiRadioSystem.h"
#include <boost/bind.hpp>

//SERVER MODE
WiFiRadioSystem::WiFiRadioSystem(const std::string& address, const std::string& port) :
io_service_(),
acceptor_(io_service_),
socket_(io_service_),
connection_manager_(),
new_connection_(new Connection(io_service_,connection_manager_/*, request_handler_*/))
{
	// Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
	boost::asio::ip::tcp::resolver resolver(io_service_);
	boost::asio::ip::tcp::resolver::query query(address, port);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
	acceptor_.open(endpoint.protocol());
	acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	acceptor_.bind(endpoint);
	acceptor_.listen();

	std::cout << "ready for accepting connections..." << std::endl;
	acceptor_.async_accept(new_connection_->socket(),
			boost::bind(&WiFiRadioSystem::handle_accept, this,
					boost::asio::placeholders::error));
}

WiFiRadioSystem::WiFiRadioSystem(tcp::resolver::query query):
			io_service_(),
			acceptor_(io_service_),
			socket_(io_service_){
	std::cout << "ready..." << std::endl;
	connectToCamera(query);
}

void WiFiRadioSystem::connectToCamera(tcp::resolver::query query){
	tcp::resolver resolver(io_service_);
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	std::cout << "connecting..." << std::endl;
	boost::asio::async_connect(socket_, endpoint_iterator,
			boost::bind(&WiFiRadioSystem::handle_connect, this,
					boost::asio::placeholders::error, endpoint_iterator));
}

void WiFiRadioSystem::handle_connect(const boost::system::error_code& error,
		tcp::resolver::iterator endpoint_iter)
{
	if (!socket_.is_open())
	{
		std::cout << "Connect timed out\n";
	}

	else if (error)
	{
		std::cout << "Connect error: " << error.message() << "\n";

		socket_.close();
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


void WiFiRadioSystem::run()
{
	// The io_service::run() call will block until all asynchronous operations
	// have finished. While the server is running, there is always at least one
	// asynchronous operation outstanding: the asynchronous accept call waiting
	// for new incoming connections.
	std::cout << "running io_service" << std::endl;
	io_service_.run();
	std::cout << "done" << std::endl;

}

void WiFiRadioSystem::stop()
{
	// Post a call to the stop function so that server::stop() is safe to call
	// from any thread.
	io_service_.post(boost::bind(&WiFiRadioSystem::handle_stop, this));
}

void WiFiRadioSystem::handle_accept(const boost::system::error_code& e)
{
	if (!e)
	{
		connection_manager_.start(new_connection_);
		new_connection_.reset(new Connection(io_service_,
				connection_manager_/*, request_handler_*/));
		acceptor_.async_accept(new_connection_->socket(),
				boost::bind(&WiFiRadioSystem::handle_accept, this,
						boost::asio::placeholders::error));
	}
}

void WiFiRadioSystem::handle_stop()
{
	// The server is stopped by cancelling all outstanding asynchronous
	// operations. Once all operations have finished the io_service::run() call
	// will exit.
	acceptor_.close();
	connection_manager_.stop_all();
}

