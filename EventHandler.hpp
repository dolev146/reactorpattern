// uses UNIX I/0 handles, which are plain integer values.
typedef int Handle;
// All interaction from Reactor to an event handler qoes
// through function pointers with the following signatures:
typedef Handle (*getHandleFunc)(void *instance);
typedef void (*handleEventFunc)(void *instance);

typedef struct
{
    void *instance;
    getHandleFunc getHandle;
    handleEventFunc handleEvent;
} EventHandler;
