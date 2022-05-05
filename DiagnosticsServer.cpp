#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "EventHandler.hpp"
#include "reactor.hpp"

typedef struct DiagnosticsServer
{
    Handle listeningSocket;
    EventHandler eventHandler;

    /* Other attributes here.
     */
} diagnostic_server_t, *DiagnosticsServerPtr;


// Implementation of the EventHandler interface.
static Handle getServerSocket(void *instance)
{
    const DiagnosticsServerPtr server = (DiagnosticsServerPtr)instance;
    return server->listeningSocket;
}

static void handleConnectRequest(void *instance)
{
    DiagnosticsServerPtr server = (DiagnosticsServerPtr)instance;
    /* The server gets notified as a new connection
request arrives. Add code for accepting the
new connection and creating a client here.*/
}

DiagnosticsServerPtr createServer(unsigned int tcpPort)
{
    DiagnosticsServerPtr newServer = (DiagnosticsServerPtr)malloc(sizeof(*newServer));
    if (NULL != newServer)
    {
        /* Code for creating the server socket here.
        The real code should look for a failure,, etc/ */

        newServer->listeningSocket = createServerSocket(tcpPort);
        // Successfully created - register the listening socket.
        newServer->eventHandler.instance = newServer;
        newServer->eventHandler.getHandle = getServerSocket;
        newServer->eventHandler.handleEvent = handleConnectRequest;
        Register(&newServer->eventHandler);
    }
    return newServer;
}

void destroyServer(DiagnosticsServerPtr server)
{

    /*Before deleting the server we have to unregister at the Reactor*/
    Unregister(&server->eventHandler);
    free(server);
}
