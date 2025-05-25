// Copyright 2025 Wincenty Wensker Dawid Łapiński
//
// Main entry point for the GUI client application.
// This file initializes the Qt application and creates the main window.

#include <QApplication>
#include "chatwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    ChatWindow window;
    window.setWindowTitle("Chat Application");
    window.resize(800, 600);
    window.show();
    
    return app.exec();
}
