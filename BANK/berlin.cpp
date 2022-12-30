#include <iostream>
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <chrono>
#include <optional>
#include <iostream>
#include <wx/wx.h>
#include <random>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fstream>

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

using namespace std;
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

int* receivetabint(tcp::socket& socket, int size) {
    int* tab = new int[size];
    boost::asio::mutable_buffer buffer = boost::asio::buffer(tab, size * sizeof(int));
    boost::system::error_code error;
    std::size_t bytes_transferred = 0;
    while (bytes_transferred < size * sizeof(int)) {
        bytes_transferred += boost::asio::read(socket, buffer, boost::asio::transfer_all(), error);
        if (error) {
            // Gérer l'erreur
            return nullptr;
        }
        buffer = buffer + bytes_transferred;
    }
    return tab;
}

string* receivetabstring(tcp::socket& socket, int size) {
    string* tab = new string[size];
    boost::asio::mutable_buffer buffer = boost::asio::buffer(tab, size * sizeof(string));
    boost::system::error_code error;
    std::size_t bytes_transferred = 0;
    while (bytes_transferred < size * sizeof(string)) {
        bytes_transferred += boost::asio::read(socket, buffer, boost::asio::transfer_all(), error);
        if (error) {
            // Gérer l'erreur
            return nullptr;
        }
        buffer = buffer + bytes_transferred;
    }
    return tab;
}

double* receivetabdouble(tcp::socket& socket, int size) {
    double* tab = new double[size];
    boost::asio::mutable_buffer buffer = boost::asio::buffer(tab, size * sizeof(double));
    boost::system::error_code error;
    std::size_t bytes_transferred = 0;
    while (bytes_transferred < size * sizeof(double)) {
        bytes_transferred += boost::asio::read(socket, buffer, boost::asio::transfer_all(), error);
        if (error) {
            // Gérer l'erreur
            return nullptr;
        }
        buffer = buffer + bytes_transferred;
    }
    return tab;
}

// fonctions d'envoie
void send_(boost::asio::ip::tcp::socket& socket, const string& message) {
    const string msg = message + "\n";
    boost::asio::write(socket, boost::asio::buffer(msg));
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

void envoietabDouble(tcp::socket& socket, double* tab, int size) {
    boost::asio::const_buffer buffer = boost::asio::buffer(tab, size * sizeof(double));
    boost::system::error_code error;
    boost::asio::write(socket, buffer, error);
}
//main

int main() {
 
    while (true) {

        boost::asio::io_service io_service;
        //listen for new connection
        tcp::acceptor acceptor_(io_service, tcp::endpoint(tcp::v4(), 1234));
        //socket creation 
        tcp::socket socket_(io_service);
        //waiting for connection
        acceptor_.accept(socket_);
        //read operation

        int nbr_ = receiveInt(socket_);
        int serv_;
        bool ok = true;
        boost::property_tree::ptree pt;

        string mdp_;
        string prenom_;
        string nom_;
        string adresse_;
        string accountName_[100];
        int AccType_[100];
        double balance_[100];
        string transList_[1000];
        int transListAcc_[1000];
        
        // Si le fichier n'existe pas, on affiche une erreur
        std::ifstream file("berlin.json");
        if (!file.good()) {
            ok = false;
        }
        else {
            // On lit le fichier JSON
            try {
                read_json("berlin.json", pt);
            }
            catch (const std::exception& e) {
                wxMessageBox(e.what(), "Erreur", wxOK | wxICON_ERROR);
            }

            // On parcourt les clients
            if (pt.count(to_string(nbr_)) == 0) // on vérifie que le numéro de client existe dans le fichier json
            {
                ok = false;
            }
            else {
                serv_ = 1;
                // On récupère les infos du client et les stocke dans les attributs de la classe
                mdp_ = pt.get<std::string>(to_string(nbr_) + ".password_");
                prenom_ = pt.get<std::string>(to_string(nbr_) + ".firstname_");
                nom_ = pt.get<std::string>(to_string(nbr_) + ".lastName_");
                adresse_ = pt.get<std::string>(to_string(nbr_) + ".address_");

                
                
                int i = 0;
                for (auto& account : pt.get_child(to_string(nbr_) + ".accountName_"))
                {
                    accountName_[i] = account.second.data();
                    AccType_[i] = std::stoi(pt.get<std::string>(to_string(nbr_) + ".accountType_." + account.first));
                    balance_[i] = std::stod(pt.get<std::string>(to_string(nbr_) + ".balance_." + account.first));
                    i++;
                }


                i = 0;

                for (auto& transaction : pt.get_child(to_string(nbr_) + ".transactionList_"))
                {
                    transList_[i] = transaction.second.data();
                    transListAcc_[i] = std::stoi(pt.get<std::string>(to_string(nbr_) + ".transactionListAccount_." + transaction.first));
                    i++;
                }
            }
            
        }





        
        
        cout << nbr_ << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (ok) { // à remplacer par une fonction qui regarde si le nombre est dans le json
            boost::system::error_code error;
            boost::asio::write(socket_, boost::asio::buffer(&serv_, sizeof(serv_)), error);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            send_(socket_, nom_);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            send_(socket_, prenom_);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            send_(socket_, adresse_);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            send_(socket_, mdp_);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            envoietabString(socket_, accountName_, 100);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            envoietabInt(socket_, AccType_, 100);
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            envoietabDouble(socket_, balance_, 100);
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            envoietabString(socket_, transList_, 1000);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            envoietabInt(socket_, transListAcc_, 1000);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        else {
            //On créé une socket
            boost::asio::io_service io_service;
            //socket creation

            tcp::socket socket1(io_service);
            socket1.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 4444));
            boost::system::error_code error;
            boost::asio::write(socket1, boost::asio::buffer(&nbr_, sizeof(nbr_)), error);
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            int serv = receiveInt(socket1);
            cout << serv << endl;
            boost::asio::write(socket_, boost::asio::buffer(&serv, sizeof(serv)), error);

            boost::asio::io_service io_service1;
            //socket creation
            socket_.close();
            tcp::socket socket2(io_service1);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            while (true) {
                try {
                    // Tenter de se connecter au serveur
                    socket2.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 2345), error);

                    // Si la connexion a réussi, sortir de la boucle
                    if (!error) {
                        break;
                    }
                }
                catch (const boost::system::system_error& e) {
                    // Ignorer l'erreur et continuer à essayer de se connecter
                }

                // Attendre 1 seconde avant de réessayer
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            std::this_thread::sleep_for(std::chrono::seconds(10));
            boost::asio::write(socket2, boost::asio::buffer(&serv, sizeof(serv)), error);


        }
    }













}


