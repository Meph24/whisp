#ifndef CONNECTIONINITIALPROCESSOR_HPP
#define CONNECTIONINITIALPROCESSOR_HPP

#include <list>
#include <memory>
#include <thread>

#include "ClientConnection.hpp"
#include "WallClock.hpp"

using std::list;
using std::thread;
using std::unique_ptr;

struct SimulationServer;
struct ConnectionListener;

struct ConnectionInitialProcessor
{
    SimulationServer& server;

    ConnectionListener& listener;
    WallClock& wc;

    ConnectionInitialProcessor(
        SimulationServer& server,
        ConnectionListener& listener, 
        WallClock& wc
        );
    ~ConnectionInitialProcessor();

    bool running = false;
    thread main_process;
    void mainProcess();
    void start();
    void stop();

    struct SingleConnectionProcessor
    {
        unique_ptr<ClientConnection> connection;
        ConnectionInitialProcessor* cip;

        thread t;

        SingleConnectionProcessor(unique_ptr<ClientConnection>&&,
            ConnectionInitialProcessor& cip
            );
        ~SingleConnectionProcessor();

        SingleConnectionProcessor(SingleConnectionProcessor&&);
        SingleConnectionProcessor& operator=(SingleConnectionProcessor&&);

        SingleConnectionProcessor(const SingleConnectionProcessor&) = delete;
        SingleConnectionProcessor& operator=(const SingleConnectionProcessor&) = delete;

        void process();
        bool finished() const;
    };

    void asyncProcessNewConnection();

    bool hasProcessedConnections();
    unique_ptr<ClientConnection> nextConnection();

private:
    mutex process_lock;
    list<SingleConnectionProcessor> processes;

    deque<unique_ptr<ClientConnection> > connections;
    void processIncomingConnections();
};

#endif // CONNECTIONINITIALPROCESSOR_HPP
