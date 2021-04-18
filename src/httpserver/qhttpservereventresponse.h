#ifndef QHTTPSERVEREVENTRESPONSE_H
#define QHTTPSERVEREVENTRESPONSE_H

#include <QtHttpServer/qhttpserverresponse.h>

QT_BEGIN_NAMESPACE

class QHttpEventSource : public QObject
{
public:
    void setResponder(QHttpServerResponder &&responder)
    {
        m_responder.reset(new QHttpServerResponder { std::move(responder) });
    }

    void sendNamedEvent(QByteArray name, QByteArray data)
    {
        QByteArray body { "event: " };
        body += name;
        body += "\r\ndata: ";
        body += data;
        body += "\r\n\r\n";
        m_responder->writeBody(body);
    }

    void sendData(QByteArray data)
    {
        QByteArray body { "data: " };
        body += data;
        body += "\r\n\r\n";
        m_responder->writeBody(body);
    }

private:
    QScopedPointer<QHttpServerResponder> m_responder;
};

class QHttpServerEventResponsePrivate;
class Q_HTTPSERVER_EXPORT QHttpServerEventResponse : public QHttpServerResponse
{
    Q_DECLARE_PRIVATE(QHttpServerEventResponse)
public:
    using QHttpServerResponse::QHttpServerResponse;

    QHttpServerEventResponse(QHttpEventSource *eventSource);

    virtual void write(QHttpServerResponder &&responder) const override;

protected:
    QHttpServerEventResponse(QHttpServerEventResponsePrivate *d);
};

QT_END_NAMESPACE

#endif // QHTTPSERVEREVENTRESPONSE_H
