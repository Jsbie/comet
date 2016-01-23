#ifndef NEW_INPUT_EVENT_H
#define NEW_INPUT_EVENT_H

class InputData;

struct NewInputEventBase {
    virtual ~NewInputEventBase() {}
    virtual void sendInputData(InputData** inputData) = 0;
};

template<class Event, class Callback>
struct NewInputEvent : public NewInputEventBase {
    Event* event;
    Callback callback;
    NewInputEvent(Event* _event, Callback _callback) : event(_event), callback(_callback) {}

    virtual void sendInputData(InputData** inputData) {
        (event->*callback)(inputData);
    }
};

#endif // NEW_INPUT_EVENT_H
