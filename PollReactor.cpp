#include "reactor.hpp"
#include "poll.h"
#include "assert.h"
#include "stdio.h"
#include "stdlib.h"
#include "ReactorEventLoop.hpp"

#define MAX_NO_OF_HANDLERS 10
/* Bind an event handler to the struct used to interface poll(). */

typedef struct
{
    EventHandler Handler;
    struct pollfd fd;

} HandlerRegistration;
static HandlerRegistration registredHandlers[MAX_NO_OF_HANDLERS];

/* Add a copy of the given handler to the first free position in registeredHandlers.*/
static void addToRegistry(EventHandler *handler);

/* Identify the event handler in the
registeredHandlers
and remove
it*/
static void removeFromRegistry(EventHandler *handler);

/* Implementation of the Reactor interface used for registrations.*/
void Register(EventHandler *handler)
{
    assert(NULL != handler);
    addToRegistry(handler);
}

void Unregister(EventHandler *handler)
{
    assert(NULL != handler);
    removeFromRegistry(handler);
}

/* Add a
copy of all registered handlers
to
the
given array.*/
static size_t buildPollArray(struct pollfd *fds);
/* Identify the
event handler
corresponding to the given descriptor
in
the*/
static EventHandler *findHandler(int fd);

static void dispatchSignalledHandles(const struct pollfd *fds,
                                     size_t noOfHandles)
{ /*Loop through all handles.Upon detection of
a handle
signalled by poll
its
corresponding event handler is fetched and invoked.*/
    size_t i = 0;
    for (i = 0; i < noOfHandles; ++i)
    {
        /*   Detect all signalled handles and invoke their corresponding event handlers.*/
        if ((POLLRDNORM | POLLERR) & fds[i].revents)
        {
            EventHandler *signalledHandler = findHandler(fds[i].fd);
            if (NULL != signalledHandler)
            {
                signalledHandler->handleEvent(signalledHandler->instance);
            }
        }
    }
}

/* Implementation of the reactive event loop.*/
void HandleEvents(void)
{
    /* Build the array required to interact with poll().*/
    struct pollfd fds[MAX_NO_OF_HANDLERS] = {0};
    const size_t noOfHandles = buildPollArray(fds);
    /*Inoke the synchronous
            event
                demultiplexer.*/
    // INFTIM = -1
    if (0 < poll(fds, noOfHandles, INFTIM))
    {
        /*
            Identify all signalled handles and invoke the event handler associated with each one.*/
        dispatchSignalledHandles(fds, noOfHandles);
    }
    else
    {
        perror("Poll failure");
    }
}