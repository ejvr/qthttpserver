#include "qhttpservereventresponse.h"
#include <private/qhttpserverresponse_p.h>

QT_BEGIN_NAMESPACE

class QHttpServerEventResponsePrivate : public QHttpServerResponsePrivate
{
public:
    QHttpServerEventResponsePrivate(QHttpEventSource *eventSource) : m_eventSource { eventSource }
    {
    }

    mutable QScopedPointer<QHttpEventSource> m_eventSource;
};

QHttpServerEventResponse::QHttpServerEventResponse(QHttpEventSource *eventSource)
    : QHttpServerEventResponse { new QHttpServerEventResponsePrivate { eventSource } }
{
}

void QHttpServerEventResponse::write(QHttpServerResponder &&responder) const
{
    Q_D(const QHttpServerEventResponse);

    if (d->m_eventSource == nullptr)
        return QHttpServerResponse::write(std::move(responder));
    QTcpSocket *socket = responder.socket();
    responder.writeStatusLine(StatusCode::Ok);
    responder.writeHeader("Content-Type", "text/event-stream");
    responder.writeHeader("Cache-Control", "no-cache");
    responder.writeHeader("X-Accel-Buffering", "no");

    /// @todo Some implementations also set 'Connection' header to 'keep-alive', but only if http
    /// version != 2

    d->m_eventSource->setResponder(std::move(responder));
    QObject::connect(socket, &QObject::destroyed, d->m_eventSource.take(), &QObject::deleteLater);
}

QHttpServerEventResponse::QHttpServerEventResponse(QHttpServerEventResponsePrivate *d)
    : QHttpServerResponse(d)
{
}

QT_END_NAMESPACE
