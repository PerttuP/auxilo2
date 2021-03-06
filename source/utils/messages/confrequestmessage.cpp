#include "confrequestmessage.h"

namespace Utils {

ConfRequestMessage::ConfRequestMessage(QString responseGroup,
                                       QString featureName,
                                       bool coreFeature) :
   m_responseGroup(responseGroup),
   m_featureName(featureName),
   m_coreFeature(coreFeature)
{
}

ConfRequestMessage::ConfRequestMessage(QByteArray data)
{
   QDataStream stream(data);
   stream >> m_responseGroup;
   stream >> m_featureName;
   stream >> m_coreFeature;
}

QByteArray ConfRequestMessage::data() const
{
   QByteArray message;
   QDataStream stream(&message, QIODevice::WriteOnly);
   stream << m_responseGroup;
   stream << m_featureName;
   stream << m_coreFeature;

   return message;
}

QString ConfRequestMessage::string() const
{
   QString message("ConfRequestMessage:\n");
   message += QString("- Response group: %1\n").arg(m_responseGroup);
   message += QString("- Feature name: %1\n").arg(m_featureName);
   message += QString(" -coreFeature: %1\n").arg(m_coreFeature);

   return message;
}

} // Utils
