#include <QLayout>
#include <QLabel>
#include <QException>
#include <QDesktopWidget>

#include "globals.h"
#include "mainwindow.h"
#include "messagesender.h"
#include "messageviewer.h"
#include "messagegroup.h"

#include "signalmessage.h" // For testing

namespace MessageTool {

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent),
     m_serverAddress(NULL),
     m_serverPort(NULL),
     m_connect(NULL),
     m_tabWidget(NULL),
     m_connected(false),
     m_groupModel(NULL)
{
   qRegisterMetaType<Globals::MessageType>("Globals::MessageType");

   try {
      m_groupModel = new GroupModel("../parameters/coreconfig.xml", this);
   } catch (QException& e) {
      qCritical("Failed to load group names.");
   }

   m_messageModel = new MessageModel(this);

   connect(m_groupModel,
           SIGNAL(messageReceived(Globals::MessageType,QByteArray,QString)),
           m_messageModel,
           SLOT(onNewMessage(Globals::MessageType,QByteArray,QString)));

   QWidget* messageViewer = new MessageViewer(*m_groupModel,
                                              *m_messageModel,
                                              this);

   QWidget* messageSender = new MessageSender(*m_groupModel,
                                              this);

   m_tabWidget = new QTabWidget(this);
   m_tabWidget->addTab(messageViewer, "View");
   m_tabWidget->addTab(messageSender, "Send");
   m_tabWidget->setEnabled(false);

   QLabel* serverAddressLabel = new QLabel("Address:", this);
   QLabel* serverPortLabel = new QLabel("Port:", this);

   m_serverAddress = new QLineEdit("127.0.0.1", this);
   m_serverPort = new QLineEdit("13803", this);

   m_connect = new QPushButton("Connect");
   connect(m_connect, SIGNAL(clicked()),
           this, SLOT(connectClicked()));

   QHBoxLayout* connectionLayout = new QHBoxLayout;
   connectionLayout->addWidget(serverAddressLabel);
   connectionLayout->addWidget(m_serverAddress);
   connectionLayout->addWidget(serverPortLabel);
   connectionLayout->addWidget(m_serverPort);
   connectionLayout->addWidget(m_connect);

   QVBoxLayout* mainLayout = new QVBoxLayout;
   mainLayout->addLayout(connectionLayout);
   mainLayout->addWidget(m_tabWidget);

   QWidget* centeralWidget = new QWidget(this);
   centeralWidget->setLayout(mainLayout);
   this->setCentralWidget(centeralWidget);

   QDesktopWidget desktopWidget;
   quint32 width = desktopWidget.width() * 0.7;
   quint32 height = desktopWidget.height() * 0.7;
   this->resize(width, height);
}

MainWindow::~MainWindow()
{
}

void MainWindow::connectClicked()
{
   bool ok = false;
   qint16 port = m_serverPort->text().toUInt(&ok);
   if (!ok) {
      qCritical("Invalid port %s", m_serverPort->text().toLatin1().data());
      return;
   }

   Utils::Connection::setHost(m_serverAddress->text(), port);
   m_connected = true;

   m_serverAddress->setEnabled(false);
   m_serverPort->setEnabled(false);
   m_connect->setEnabled(false);
   m_tabWidget->setEnabled(true);
}

} // MessageTool
