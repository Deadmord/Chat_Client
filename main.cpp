#include "ChatClient.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    Client& client = Client::instance();
    ChatClient w(client);
    w.show();
    return a.exec();
}

