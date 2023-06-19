#include "ChatClient.h"
#include <QtWidgets/QApplication>
#include <plog/Log.h> 
#include <QObject>
#include <plog/Initializers/RollingFileInitializer.h>


int main(int argc, char* argv[])
{

    plog::init(plog::debug, "log.txt", 1000000, 5);
    PLOGD << "CLient Application Start";
    QApplication a(argc, argv);
    ChatClient w;
    w.show();
    return a.exec();
}

