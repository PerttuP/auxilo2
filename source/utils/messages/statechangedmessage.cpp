#include "statechangedmessage.h"

namespace Utils {

StateChangedMessage::StateChangedMessage(QString label,
                                         QVariant value,
                                         QString ackGroup) :
   m_label(label),
   m_value(value),
   m_ackGroup(ackGroup),
   m_ackId(0)
{
   if (!m_ackGroup.isEmpty()) {
      m_ackId = this->nextAckId();
   }
}

StateChangedMessage::StateChangedMessage(QByteArray data)
{
   QDataStream stream(data);
   stream >> m_label;
   stream >> m_value;
   stream >> m_ackGroup;

   if (!m_ackGroup.isEmpty()) {
      stream >> m_ackId;
   }
}

StateChangedAckMessage StateChangedMessage::createAckMessage(
      StateChangedAckMessage::Result result) const
{
   return StateChangedAckMessage(m_ackId, result);
}

QByteArray StateChangedMessage::data() const
{
   QByteArray message;
   QDataStream stream(&message, QIODevice::WriteOnly);

   stream << m_label;
   stream << m_value;
   stream << m_ackGroup;

   if (!m_ackGroup.isEmpty()) {
      stream << m_ackId;
   }

   return message;
}

QString StateChangedMessage::string() const
{
   QString message("StateChangedMessage:\n");
   message += QString("- Label: %1\n").arg(m_label);
   message += QString("- Value: %1\n")
                      .arg(m_value.toByteArray().toHex().data());

   if (!m_ackGroup.isEmpty()) {
      message +=(QString("- AckGroup: %1\n")).arg(m_ackGroup);
      message += QString("- Ack id: %1\n").arg(m_ackId);
   }

   return message;
}

} // Utils
