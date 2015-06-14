#ifndef SCRIPTAPIIMPLEMENTATION_HH
#define SCRIPTAPIIMPLEMENTATION_HH

#include <QObject>
#include "scriptapi.hh"
#include "interfaces/scriptlibrary.hh"
#include "interfaces/scriptupdateobserver.hh"
#include "interfaces/scriptupdatesubject.hh"
#include "messagegroup.h"
#include "stateresponsemessage.h"
#include "setstateackmessage.h"
#include <mutex>
#include <condition_variable>


namespace SignalHandler 
{

class ScriptApiImplementation : 
        public QObject, public ScriptAPI, public ScriptUpdateObserver
{
public:
    
    /*!
     * \brief Constructor.
     * \param lib Script library.
     * \param subject Script library update subject.
     * \param namingSuffix Suffix to be added at the end of message groups.
     * \pre lib != nullptr, subject != nullptr, namingSuffix is unique.
     */
    ScriptApiImplementation(const ScriptLibrary* lib,
                            ScriptUpdateSubject* subject,
                            const QString& namingSuffix);
    
    //! Destructor.
    virtual ~ScriptApiImplementation();
    
    //! Move- and copy-constructors and assignment operators are forbidden.
    ScriptApiImplementation(const ScriptApiImplementation&) = delete;
    ScriptApiImplementation(ScriptApiImplementation&&) = delete;
    ScriptApiImplementation& operator=(const ScriptApiImplementation&) = delete;
    ScriptApiImplementation& operator=(ScriptApiImplementation&&) = delete;
    
    // ScriptApi interface implementations. See scriptapi.hh for documentation.
    
    virtual QDateTime dateTimeNow() const;
    
    virtual Utils::StateResponseMessage::State 
    getStateOf(const QString& stateName);
    
    virtual ScriptAPI::StateMap getStates(const QStringList& states);
    
    virtual int setState(const QString& stateName, const QVariant& value);
    
    virtual int sendSignal(const QString& signalName, const QStringList& args);
    
    // Implements the ScriptUpdateObserver interface.
    virtual void notifyOnScriptUpdate(const ScriptLibrary* new_lib);
    
    
private Q_SLOTS:
    
    void onStateReqReturned(const QByteArray& data);
    void onStateChangeAck(const QByteArray& data);
  
    
private:
    
    const ScriptLibrary* lib_;
    ScriptUpdateSubject* subject_;
    Utils::MessageGroup* reqGroup_;
    Utils::MessageGroup* ackGroup_;
    QString reqGroupName_;
    QString ackGroupName_;
    Utils::StateResponseMessage pendingReq_;
    Utils::SetStateAckMessage pendingAck_;
    
    std::mutex updateMx_;
    std::mutex waitMx_;
    std::condition_variable cv_;
};

}

#endif // SCRIPTAPIIMPLEMENTATION_HH
