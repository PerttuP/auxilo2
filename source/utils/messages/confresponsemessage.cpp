#include <QDataStream>

#include "confresponsemessage.h"

namespace Utils {

ParameterSet::ParameterSet()
{
}

ParameterSet::ParameterSet(QString featureName) :
   m_featureName(featureName)
{
}

bool ParameterSet::appendParameter(QString name,
                                   QString value)
{
   name = name.toLower();
   if (!m_parameters.contains(name)) {
      m_parameters.insert(name, value);
      return true;
   }

   return false;
}

void ParameterSet::appendParameters(QHash<QString, QString> parameters)
{
   foreach (QString parameter, parameters.keys()) {
      appendParameter(parameter, parameters.value(parameter));
   }
}

ParameterSet ParameterSet::getSection(QString startsWith,
                                      bool returnShortPath) const
{
   ParameterSet section(featureName());

   foreach (QString parameter, m_parameters.keys()) {
      if (parameter.startsWith(startsWith)) {
         QString value = m_parameters.value(parameter);
         QString name = parameter;
         if (returnShortPath) {
            name = name.mid(startsWith.length());
         }

         section.appendParameter(name, value);
      }
   }

   return section;
}

bool ParameterSet::contains(QString name) const
{
   return m_parameters.contains(name.toLower());
}

QStringList ParameterSet::parameterList(QString startsWith,
                                        bool returnShortPath) const
{
   if (startsWith == QString()) {
      return m_parameters.keys();
   }

   QStringList parameters;
   foreach (QString parameter, m_parameters.keys()) {
      if (parameter.startsWith(startsWith)) {
         if (returnShortPath) {
            parameters.append(parameter.mid(startsWith.length()));
         } else {
            parameters.append(parameter);
         }
      }
   }

   return parameters;
}

ConfResponseMessage::ConfResponseMessage(const ParameterSet& parameterSet) :
   m_parameterSet(parameterSet),
   m_result(NO_ERROR)
{
}

ConfResponseMessage::ConfResponseMessage(const QByteArray& payload) :
   m_parameterSet("")
{
   QDataStream stream(payload);

   int result;
   stream >> result;
   m_result = static_cast<Result>(result);

   QString featureName;
   stream >> featureName;
   m_parameterSet.setFeatureName(featureName);

   quint32 numberOfParameters;
   stream >> numberOfParameters;

   for (quint32 i = 0; i < numberOfParameters; ++i) {
      QString name;
      stream >> name;

      QString value;
      stream >> value;

      m_parameterSet.appendParameter(name, value);
   }
}

QByteArray ConfResponseMessage::data() const
{
   QByteArray message;
   QDataStream stream(&message, QIODevice::WriteOnly);

   const QHash<QString, QString>& parameters = m_parameterSet.parameters();

   stream << m_result;
   stream << m_parameterSet.featureName();
   stream << parameters.size();

   foreach (QString parameter, parameters.keys()) {
      stream << parameter;
      stream << parameters.value(parameter);
   }

   return message;
}

QString ConfResponseMessage::string() const
{
   QString message("ConfResponseMessage:\n");
   message += QString("Result: %1\n").arg(m_result);

   const QHash<QString, QString>& parameters = m_parameterSet.parameters();
   foreach (QString parameter, parameters.keys()) {
      message += QString("- %1: %2\n").arg(parameter)
                                      .arg(parameters.value(parameter));
   }

   return message;
}

} // Utils
