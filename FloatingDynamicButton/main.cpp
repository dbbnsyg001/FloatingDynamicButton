#include "FloatingDynamicButton.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    FloatingDynamicButton w;
    w.SetText("TestText");
    w.SetImage(":/FloatingDynamicButton/Resources/right_arrow.png", "This is a picture", "");
    w.show();
    return a.exec();
}
