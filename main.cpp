#include <QtWidgets/QApplication>
#include <plog/Log.h> 
#include <plog/Initializers/RollingFileInitializer.h>
#include "ChatClient.h"
#include "Message.h"

int main(int argc, char* argv[])
{

    plog::init(plog::debug, "log.txt", 1000000, 5);
    PLOGI << "Logging starts";

    QApplication a(argc, argv);
    //ChatClient w;
    //w.startClient();
    Message w;
    w.show();
    return a.exec();
}

