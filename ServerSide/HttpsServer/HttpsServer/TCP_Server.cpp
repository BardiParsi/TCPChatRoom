#include "TCP_Server.h"

TCP_Server::TCP_Server(io_context& io_context, short port) : 
	io_context_(io_context),
	acceptor_v4_(io_context, tcp::endpoint(tcp::v4(), port)),
	acceptor_v6_(io_context, tcp::endpoint(tcp::v6(), port)) 
{
	start_accept_v4(); 
	start_accept_v6();
}

void TCP_Server::start_accept_v4() {
	auto newConnection = TCP_Connection::create(io_context_);
	acceptor_v4_.async_accept(newConnection->socket(), 
		std::bind(&TCP_Server::handle_accept_v4, this, newConnection, asio::placeholders::error));
}

void TCP_Server::start_accept_v6() {
	auto newConnection = TCP_Connection::create(io_context_);
	acceptor_v6_.async_accept(newConnection->socket(),
		std::bind(&TCP_Server::handle_accept_v6, this, newConnection, asio::placeholders::error));
}

void TCP_Server::handle_accept_v4(std::shared_ptr<TCP_Connection> newConnection, const error_code& ec) {
	if (!ec) {
		console.log("Connection from IPv4 accepted.");
		newConnection->start();
	}
	start_accept_v4();
}

void TCP_Server::handle_accept_v6(std::shared_ptr<TCP_Connection> newConnection, const error_code& ec) {
	if (!ec) {
		console.log("Connection from IPv6 accepted.");
		newConnection->start();
	}
	start_accept_v6();
}