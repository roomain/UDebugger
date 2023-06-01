#include "DebuggedApp.h"
#include <QtWidgets/QApplication>
#include "QDebuggerServer.h"
#include "IDebugObject.h"
#ifdef WIN32
#include <Windows.h>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDebuggerServer server;
    Debugger::Logger::instance().setCallback(std::bind_front(&QDebuggerServer::log,&server));
    server.setDatabase(Debugger::IDebugObject::database());
    server.listen(QHostAddress("127.0.0.1"), 3001);
    DebuggedApp w;
    w.show();

#ifdef WIN32
    ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif
    return a.exec();
}
