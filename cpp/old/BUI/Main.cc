#include "Window.hh"
#include <old/BCore/Bemo.hh>
#include <QApplication>
#include <old/BCore/util/Log.hh>


int main(int argc, char **argv) {

    bemo::initialize();

    QApplication app (argc, argv);
    Window window;
    window.show();

    int rc = app.exec();

    bemo::terminate();

    return rc;
}