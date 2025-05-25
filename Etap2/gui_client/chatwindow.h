// Copyright 2025 Wincenty Wensker Dawid Łapiński
//
// Header file for the ChatWindow class which is the main window of the chat application.
// Defines the UI elements and functionality for the chat client.

#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

private slots:
    void connectToServer();
    void sendMessage();
    void readMessage();
    void handleError(QAbstractSocket::SocketError error);
    void displayMessage(const QString& message);
    
private:
    // UI elements
    QWidget* m_centralWidget;
    QVBoxLayout* m_mainLayout;
    QWidget* m_loginWidget;
    QVBoxLayout* m_loginLayout;
    QLabel* m_nameLabel;
    QLineEdit* m_nameInput;
    QPushButton* m_connectButton;
    QWidget* m_chatWidget;
    QVBoxLayout* m_chatLayout;
    QTextEdit* m_chatDisplay;
    QHBoxLayout* m_inputLayout;
    QLineEdit* m_messageInput;
    QPushButton* m_sendButton;
    
    // Network components
    QTcpSocket* m_socket;
    QString m_username;
    
    void setupUi();
};

#endif // CHATWINDOW_H
