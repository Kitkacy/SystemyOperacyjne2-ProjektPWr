// Copyright 2025 Wincenty Wensker Dawid Łapiński
//
// Implementation of the ChatWindow class which provides the UI and functionality
// for connecting to the chat server and sending/receiving messages.

#include "chatwindow.h"

#include <QMessageBox>
#include <QScrollBar>
#include <QHostAddress>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

ChatWindow::ChatWindow(QWidget *parent)
    : QMainWindow(parent), m_socket(nullptr)
{
    setupUi();
}

ChatWindow::~ChatWindow()
{
    if (m_socket) {
        m_socket->close();
        delete m_socket;
    }
}

void ChatWindow::setupUi()
{
    m_centralWidget = new QWidget(this);
    m_mainLayout = new QVBoxLayout(m_centralWidget);
    
    // Login widget
    m_loginWidget = new QWidget(this);
    m_loginLayout = new QVBoxLayout(m_loginWidget);
    
    m_nameLabel = new QLabel("Enter your name:", m_loginWidget);
    m_loginLayout->addWidget(m_nameLabel);
    
    m_nameInput = new QLineEdit(m_loginWidget);
    m_loginLayout->addWidget(m_nameInput);
    
    m_connectButton = new QPushButton("Connect", m_loginWidget);
    m_loginLayout->addWidget(m_connectButton);
    connect(m_connectButton, &QPushButton::clicked, this, &ChatWindow::connectToServer);
    
    // Chat widget
    m_chatWidget = new QWidget(this);
    m_chatLayout = new QVBoxLayout(m_chatWidget);
    
    m_chatDisplay = new QTextEdit(m_chatWidget);
    m_chatDisplay->setReadOnly(true);
    m_chatLayout->addWidget(m_chatDisplay);
    
    m_inputLayout = new QHBoxLayout();
    
    m_messageInput = new QLineEdit(m_chatWidget);
    m_inputLayout->addWidget(m_messageInput);
    
    m_sendButton = new QPushButton("Send", m_chatWidget);
    m_inputLayout->addWidget(m_sendButton);
    connect(m_sendButton, &QPushButton::clicked, this, &ChatWindow::sendMessage);
    connect(m_messageInput, &QLineEdit::returnPressed, this, &ChatWindow::sendMessage);
    
    m_chatLayout->addLayout(m_inputLayout);
    
    // Initially show only the login widget
    m_mainLayout->addWidget(m_loginWidget);
    m_chatWidget->setVisible(false);
    
    setCentralWidget(m_centralWidget);
}

void ChatWindow::connectToServer()
{
    m_username = m_nameInput->text().trimmed();
    if (m_username.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please enter a name.");
        return;
    }
    
    if (m_socket)
    {
        m_socket->close();
        delete m_socket;
        m_socket = nullptr;
    }
    
    m_socket = new QTcpSocket(this);
    
    connect(m_socket, &QTcpSocket::readyRead, this, &ChatWindow::readMessage);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error),
            this, &ChatWindow::handleError);
    
    // Connect to the server
    m_socket->connectToHost(QHostAddress(SERVER_IP), PORT);
    
    if (m_socket->waitForConnected(5000))
    {
        // Send the username
        m_socket->write(m_username.toUtf8());
        
        // Switch to chat view
        m_loginWidget->setVisible(false);
        m_mainLayout->addWidget(m_chatWidget);
        m_chatWidget->setVisible(true);
        
        m_chatDisplay->append("<b>Connected to chat server</b>");
        m_messageInput->setFocus();
    }
    else
    {
        QMessageBox::critical(this, "Connection Error", 
                             "Could not connect to server. Please try again.");
        delete m_socket;
        m_socket = nullptr;
    }
}

void ChatWindow::sendMessage()
{
    QString message = m_messageInput->text().trimmed();
    if (message.isEmpty() || !m_socket)
        return;
    
    m_socket->write(message.toUtf8());
    m_messageInput->clear();
    m_messageInput->setFocus();
}

void ChatWindow::readMessage()
{
    QByteArray data = m_socket->readAll();
    if (!data.isEmpty()) {
        displayMessage(QString::fromUtf8(data));
    }
}

void ChatWindow::handleError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            displayMessage("<b>Disconnected from server</b>");
            m_chatWidget->setVisible(false);
            m_loginWidget->setVisible(true);
            break;
        default:
            QMessageBox::critical(this, "Connection Error", 
                                 "Error: " + m_socket->errorString());
    }
}

void ChatWindow::displayMessage(const QString& message)
{
    m_chatDisplay->append(message);
    
    // Scroll to the bottom
    QScrollBar *scrollbar = m_chatDisplay->verticalScrollBar();
    scrollbar->setValue(scrollbar->maximum());
}
