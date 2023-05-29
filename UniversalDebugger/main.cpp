#include "DebuggerUI.h"
#include <QtWidgets/QApplication>
#include <qlocale.h>

#ifdef _WIN32
#include <Windows.h>
#endif

int main(int argc, char *argv[])
{
#ifdef _WIN32
    ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif
    QLocale::setDefault(QLocale("C"));
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/DebuggerUI/resources/bug.png"));
    DebuggerUI win;
    win.setWindowTitle("Universal debugger");
    win.show();
    return app.exec();
}
