#include <QDataStream>

#include "setstateackmessage.h"

namespace Utils {

SetStateAckMessage::SetStateAckMessage(quint32 ackId,
                                       Result result) :
   Message(),
   m_ackId(ackId),
   m_result(result)
{
}

SetStateAckMessage::SetStateAckMessage(QByteArray data) :
   Message()
{
   QDataStream stream(data);
   stream >> m_ackId;

   int tmp;
   stream >> tmp;
   m_result = static_cast<Result>(tmp);
}

QByteArray SetStateAckMessage::data() const
{
   QByteArray message;
   QDataStream stream(&message, QIODevice::WriteOnly);
   stream << m_ackId;
   stream << static_cast<int>(m_result);

   return message;
}

QString SetStateAckMessage::string() const
{
   QString message("SetStateAckMessage:\n");
   message += QString("- AckId: %1\n").arg(m_ackId);
   message += QString("- Result: %1\n").arg(m_result);

   return message;
}

} // Utils
