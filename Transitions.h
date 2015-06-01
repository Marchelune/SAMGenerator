#ifndef TRANSITION_H
#define TRANSITION_H

#include <QState>
#include <QStateMachine>
#include <QSignalTransition>
#include <QKeyEventTransition>
#include <QMouseEventTransition>


/// returns the position of the mouse cursor in widget coordinates.
inline QPoint cursorPos(QWidget* w) {return w->mapFromGlobal(QCursor::pos());}

/// returns the position of the mouse cursor in widget coordinates.
inline QPoint cursorPos(QWidget& w) {return w.mapFromGlobal(QCursor::pos());}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/// adds a QSignalTransition.
inline QSignalTransition* addTrans(QState* from, QState* to,
                                   QObject* sender, const char* signal) {
    return from->addTransition(sender, signal, to);
}

/// adds a QSignalTransition and connects a slot to this translation.
inline QSignalTransition* addTrans(QState* from, QState* to,
                                   QObject* sender, const char* signal,
                                   QObject* receiver, const char* slot) {
    QSignalTransition *trans = from->addTransition(sender, signal, to);
    QObject::connect(trans, SIGNAL(triggered()), receiver, slot);
    return trans;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/// adds a QEventTransition.
inline QEventTransition* addEventTrans(QState* from, QState* to,
                                       QObject* sender, QEvent::Type type) {
    QEventTransition* trans = new QEventTransition(sender, type);
    trans->setTargetState(to);
    from->addTransition(trans);
    return trans;
}

/// adds a QEventTransition and connects a slot to this translation.
inline QEventTransition* addEventTrans(QState* from, QState* to,
                                       QObject* sender, QEvent::Type type,
                                       QObject* receiver, const char* slot) {
    QEventTransition* trans = addEventTrans(from, to, sender, type);
    QObject::connect(trans, SIGNAL(triggered()), receiver, slot);
    return trans;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/// adds a QKeyEventTransition.
inline QKeyEventTransition* addKeyTrans(QState* from, QState* to,
                                        QObject* sender, QEvent::Type type, int key) {
    QKeyEventTransition* trans = new QKeyEventTransition(sender, type, key);
    trans->setTargetState(to);
    from->addTransition(trans);
    return trans;
}

/// adds a QKeyEventTransition and connects a slot to this translation.
inline QKeyEventTransition* addKeyTrans(QState* from, QState* to,
                                        QObject* sender, QEvent::Type type, int key,
                                        QObject* receiver, const char* slot) {
    QKeyEventTransition* trans = addKeyTrans(from, to, sender, type, key);
    QObject::connect(trans, SIGNAL(triggered()), receiver, slot);
    return trans;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/// adds a QMouseEventTransition.
inline QMouseEventTransition* addMouseTrans(QState* from, QState* to,
                                 QObject* sender, QEvent::Type type, Qt::MouseButton btn) {
    QMouseEventTransition* trans = new QMouseEventTransition(sender, type, btn);
    trans->setTargetState(to);
    from->addTransition(trans);
    return trans;
}

/// adds a QMouseEventTransition and connects a slot to this translation.
inline QMouseEventTransition* addMouseTrans(QState* from, QState* to,
                                 QObject* sender, QEvent::Type type, Qt::MouseButton btn,
                                 QObject* receiver, const char* slot) {
    QMouseEventTransition* trans = addMouseTrans(from, to, sender, type, btn);
    QObject::connect(trans, SIGNAL(triggered()), receiver, slot);
    return trans;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class MouseEventTransition : public QMouseEventTransition {
    QPoint& _pos;
public:
    MouseEventTransition(QObject* sender, QEvent::Type type, Qt::MouseButton button, QPoint& pos)
        : QMouseEventTransition(sender, type, button), _pos(pos) {}

    virtual void onTransition(QEvent *e) {
        QEvent* realEvent = static_cast<QStateMachine::WrappedEvent*>(e)->event();
        switch (realEvent->type()) {
        case QEvent::MouseMove:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseButtonDblClick:
            _pos = (static_cast<QMouseEvent*>(realEvent))->pos();
        default: break;
        }
    }
};

/** adds a MouseEventTransition that updates the mouse position.
* 'pos' is a QPoint which value is updated when the transition is fired.
* 'pos' must not be destroyed as long as this transition exists.
*/
inline MouseEventTransition* addMouseTrans(QState* from, QState* to,
                                           QObject* sender, QEvent::Type type, Qt::MouseButton btn,
                                           QPoint& pos) {
    MouseEventTransition* trans = new MouseEventTransition(sender, type, btn, pos);
    trans->setTargetState(to);
    from->addTransition(trans);
    return trans;
}

/** adds a MouseEventTransition that updates the mouse position and connects a slot.
* 'pos' is a QPoint which value is updated when the transition is fired.
* 'pos' must not be destroyed as long as this transition exists.
*/
inline MouseEventTransition* addMouseTrans(QState* from, QState* to,
                                           QObject* sender, QEvent::Type type, Qt::MouseButton btn,
                                           QObject* receiver, const char* slot,
                                           QPoint& pos) {
    MouseEventTransition* trans = addMouseTrans(from, to, sender, type, btn, pos);
    QObject::connect(trans, SIGNAL(triggered()), receiver, slot);
    return trans;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/// Custom event (fires a CustomTransition).
struct CustomEvent : public QEvent {
    /** constructor.
    * ctype is the "custom" type. It should be >= 0 and < 6000.
    * The corresponding event type is ctype+QEvent::User+1
    */
    CustomEvent(int ctype) : QEvent( QEvent::Type(customToEventType(ctype)) ) {}

    int ctype() const {return eventToCustomType(type());}

    static int eventToCustomType(int eventType) {return eventType-QEvent::User-1;}
    static int customToEventType(int customType) {return customType+QEvent::User+1;}
};


/// Custom transition (fired by a CustomEvent).
class CustomTransition : public QAbstractTransition {
public:
    /** constructor.
    * ctype is the "custom" type. It should be >= 0 and < 6000.
    * @see CustomEvent.
    */
    CustomTransition(int ctype) : _ctype(ctype) {}
    int ctype() const {return _ctype;}

protected:
    const int _ctype;

    virtual bool eventTest(QEvent *e) {
        return _ctype == CustomEvent::eventToCustomType(e->type());
    }
    virtual void onTransition(QEvent *) {}
};


/** adds a transition that can be fired by a CustomEvent.
* The transition is fired by posting a CustomEvent with the same ctype on the
* appropriate QStateMachine:
*        state_machine->postEvent(new CustomEvent(ctype));
* Note that ctype should be >= 0 and < 6000.
*/
inline CustomTransition* addCustomTrans(QState* from, QState* to, int ctype) {
    CustomTransition* trans = new CustomTransition(ctype);
    trans->setTargetState(to);
    from->addTransition(trans);
    return trans;
}

/** adds a transition that can be fired by a CustomEvent and connects a slot to this transition.
* @see addCustomTrans(QState* from, QState* to, int ctype).
*/
inline CustomTransition* addCustomTrans(QState* from, QState* to, int ctype,
                                        QObject* receiver, const char* slot) {
    CustomTransition* trans = addCustomTrans(from, to, ctype);
    QObject::connect(trans, SIGNAL(triggered()), receiver, slot);
    return trans;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename Data>
class DataEvent : public QEvent {
public:
    Data _data;
    DataEvent(const Data& data) : QEvent(QEvent::User), _data(data) {}
};

template <typename Data>
class DataTransition : public QAbstractTransition {
public:
    Data _data;
    DataTransition(const Data& data) : _data(data) {}
protected:
    virtual bool eventTest(QEvent *e) {
        // checks QEvent's type() ID
        if (e->type() != QEvent::User) return false;
        // checks the actual C++ Data type
        DataEvent<Data> *de = dynamic_cast<DataEvent<Data>*>(e);
        return (de!=NULL && _data==de->_data);
    }
    virtual void onTransition(QEvent *) {}
};

template <typename Data>
inline DataTransition<Data>* addDataTrans(QState* from, QState* to, const Data& data) {
    DataTransition<Data>* trans = new DataTransition<Data>(data);
    trans->setTargetState(to);
    from->addTransition(trans);
    return trans;
}

template <typename Data>
inline DataTransition<Data>* addDataTrans(QState* from, QState* to, const Data& data,
                                          QObject* receiver, const char* slot) {
    DataTransition<Data>* trans = addDataTrans(from, to, data);
    QObject::connect(trans, SIGNAL(triggered()), receiver, slot);
    return trans;
}

#endif // TRANSITION_H
