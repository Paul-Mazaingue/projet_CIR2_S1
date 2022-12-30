#include <iostream>
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <chrono>
#include <optional>


using std::string;
using std::to_string;
using std::vector;
using std::queue;
using std::cin;
using std::cout;
using std::endl;
using std::ostream;
using std::random_device;
using std::default_random_engine;
using std::uniform_int_distribution;
using namespace std::chrono_literals;
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <boost/asio.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/serialization.hpp>


using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::endl;
using std::string;
using std::unique;
using std::unique_ptr;
using std::vector;
using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

// fonctions de reçus
string read_(tcp::socket& socket) {
    boost::asio::streambuf buf;
    boost::asio::read_until(socket, buf, "\n");
    string data = boost::asio::buffer_cast<const char*>(buf.data());
    return data;
}

double receiveDouble(tcp::socket& socket) {
    double n;
    boost::asio::mutable_buffer buffer = boost::asio::buffer(&n, sizeof(n));
    socket.async_read_some(buffer, [](const boost::system::error_code& error, std::size_t bytes_transferred) {});
    double result = *static_cast<double*>(buffer.data());
    return result;
}

int receiveInt(tcp::socket& socket) {
    int n;
    boost::asio::mutable_buffer buffer = boost::asio::buffer(&n, sizeof(n));
    socket.async_read_some(buffer, [](const boost::system::error_code& error, std::size_t bytes_transferred) {});
    int result = *static_cast<int*>(buffer.data());
    return result;
}


string* receivetabstring(tcp::socket& socket, int size) {
    string* tab = new string[size];
    boost::asio::mutable_buffer buffer = boost::asio::buffer(tab, size * sizeof(string));
    socket.async_read_some(buffer, [](const boost::system::error_code& error, std::size_t bytes_transferred) {});
    return tab;
}

double* receivetabdouble(boost::asio::ip::tcp::socket& socket, int size) {
    double* tab = new double[size];
    boost::asio::mutable_buffer buffer = boost::asio::buffer(tab, size * sizeof(double));
    socket.async_read_some(buffer, [](const boost::system::error_code& error, std::size_t bytes_transferred) {});
    return tab;
}

// fonctions d'envoie
void send_(boost::asio::ip::tcp::socket& socket, const string& message) {
    const string msg = message + "\n";
    boost::asio::write(socket, boost::asio::buffer(message));
}
void envoietabInt(tcp::socket& socket, int* tab, int size) {
    boost::asio::const_buffer buffer = boost::asio::buffer(tab, size * sizeof(int));
    boost::system::error_code error;
    boost::asio::write(socket, buffer, error);
}

void envoietabString(tcp::socket& socket, string* tab, int size) {
    boost::asio::const_buffer buffer = boost::asio::buffer(tab, size * sizeof(string));
    boost::system::error_code error;
    boost::asio::write(socket, buffer, error);
}
//main

int main() {
    double G1 = 123456;
    int G2 = 234567;
    double G3 = 345678;
    while (true) {



        boost::asio::io_service io_service;
        //listen for new connection
        tcp::acceptor acceptor_(io_service, tcp::endpoint(tcp::v4(), 4444));
        //socket creation 
        tcp::socket socket_(io_service);
        //waiting for connection
        acceptor_.accept(socket_);
        //read operation
        int message = receiveInt(socket_);
        cout << message << endl;

        if (message == G2) { // Si enregistré dans le fichier 2
            int nbr = 2;

            boost::system::error_code error;
            boost::asio::write(socket_, boost::asio::buffer(&nbr, sizeof(nbr)), error);
            int i = 0;
        }
        else {
            cout << message << endl;

        }
    }
}



