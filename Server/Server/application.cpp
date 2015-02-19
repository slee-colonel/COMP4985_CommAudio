#include "application.h"
#include "ui_application.h"
#include "configure.h"


Application::Application(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Application)
{
    ui->setupUi(this);
}

Application::~Application()
{
    delete ui;
}

//btn click listener
void Application::on_pushButton_clicked()
{

}

void Application::on_actionConfigure_triggered()
{
    QString port;
    int portnum;
    Configure conf;
    conf.setModal(true);
    if (conf.exec() == QDialog::Accepted)
    {
        port = conf.getPort();

        portnum = atoi(port.toUtf8().constData());

        StartServer(portnum);

    }

}
