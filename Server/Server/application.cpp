/*---------------------------------------------------------------------------------------
--	SOURCE FILE:		application.cpp -   Source file for the server UI class
--
--	PROGRAM:			Server.exe
--
--	FUNCTIONS:			Application
--						~Application
--						on_pushButton_clicked()
--						on_actionConfigure_triggered()
--						appendToLog
--						updatePlaylist
--
--	DATE:				Febuary 19 2015
--
--	DESIGNERS:			Filip Gutica & Auto-generated
--
--	PROGRAMMERS:		Filip Gutica & Auto-generated
--
--	NOTES:
--	Creates and instantiates the server gui. Has functions for loading songs into the playlist
--  and interactions with the UI
---------------------------------------------------------------------------------------*/
#include "application.h"
#include "ui_application.h"
#include "configure.h"



/*------------------------------------------------------------------------------
--	FUNCTION: Application()
--
--	PURPOSE:		Constructor, initializes the Ui object containing al ui elemnets
--
--	DESIGNERS:		Auto-generated
--
--	PROGRAMMER:		Auto-generated
/*-----------------------------------------------------------------------------*/
Application::Application(QWidget *parent) : QMainWindow(parent),ui(new Ui::Application)
{
    ui->setupUi(this);

    ui->playList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    loadPlaylist();
    updatePlaylist();
}

/*------------------------------------------------------------------------------
--	FUNCTION: ~Application()
--
--	PURPOSE:		Destructor, cleans up the ui. Deletes the ui object
--
--	DESIGNERS:		Auto-generated
--
--	PROGRAMMER:		Auto-generated
/*-----------------------------------------------------------------------------*/
Application::~Application()
{
    delete ui;
}

/*------------------------------------------------------------------------------
--	FUNCTION: on_pushButton_clicked()
--
--	PURPOSE:		Starts the file dialog that adds songs to the playlist when the
--                  add song button is pressed.
--
--	PARAMETERS:
--		void
--
--	DESIGNERS:		Filip Gutica
--
--	PROGRAMMER:		Filip Gutica
/*-----------------------------------------------------------------------------*/
void Application::on_pushButton_clicked()
{
    QString fName;

    //Open the file dialog
    if ((fName = QFileDialog::getOpenFileName(this, tr("Open File"), "C://",
                tr("All files (*.*);;mp3 File (*.mp3);;wav File (*.wav)"))) != "")
    {

        //Add the file path to the list
        fileList.push_back(fName);

        //Update the playlist
        updatePlaylist();
    }

    //Iterate through the list print out contents **debugging purposes**
    for (int i = 0; i < fileList.size(); i++)
    {
        qDebug() << fileList.at(i) << endl;
    }

}

/*------------------------------------------------------------------------------
--	FUNCTION: on_actionConfigure_triggered()
--
--	PURPOSE:		Starts the configure/start server dialog when the start server
--                  menu item is pressed. The dialog requests the
--                  port number for the server to listen on then starts the server.
--
--	PARAMETERS:
--		void
--
--	DESIGNERS:		Filip Gutica
--
--	PROGRAMMER:		Filip Gutica
/*-----------------------------------------------------------------------------*/
void Application::on_actionConfigure_triggered()
{
    QString port;   // Port number from the edittext
    int portnum;    // integer to put the port number
    Configure conf;

    conf.setModal(true);

    if (conf.exec() == QDialog::Accepted)           // Execute the dialog. and if user hit ok...
    {
        port = conf.getPort();                      // Get the port number

        portnum = atoi(port.toUtf8().constData());  // Convert QString to int

        StartServer(portnum, (LPVOID)this, fileList);         // Start the server pass instance of the ui as a void pointer

    }

}

/*------------------------------------------------------------------------------
--	FUNCTION: appendToLog()
--
--	PURPOSE:		Generates a signal: valueChanged(QString) which signals the
--                  ui thread to append the passed string to the serverLog QTextBrowser
--
--	PARAMETERS:
--		QString     QString to append to the log
--
--	DESIGNERS:		Filip Gutica
--
--	PROGRAMMER:		Filip Gutica
/*-----------------------------------------------------------------------------*/
void Application::appendToLog(QString str)
{
    emit valueChanged(str);
}

/*------------------------------------------------------------------------------
--	FUNCTION: loadPlaylist()
--
--	PURPOSE:		Scans the Music directory one folder out from wherever the .exe is
--                  Adds any mp3 or wav files to the file list.
--
--	PARAMETERS:
--		void
--
--	DESIGNERS:		Filip Gutica
--
--	PROGRAMMER:		Filip Gutica
/*-----------------------------------------------------------------------------*/
void Application::loadPlaylist()
{
    QDirIterator dirIter("../../../Music", QDirIterator::Subdirectories);
    QString curFile;

    while (dirIter.hasNext())
    {
        dirIter.next();
        if (QFileInfo(dirIter.filePath()).isFile())
        {
            if (QFileInfo(dirIter.filePath()).suffix() == "mp3"
                    || QFileInfo(dirIter.filePath()).suffix() == "wav")
            {
                curFile = dirIter.filePath();
                qDebug() << curFile;
                fileList.push_back(curFile);
            }
        }
    }
}

/*------------------------------------------------------------------------------
--	FUNCTION: updatePlaylist()
--
--	PURPOSE:		loads our std::list fileList into the QTextBrowser widget
--                  representing our playlist.
--
--	PARAMETERS:
--		void
--
--	DESIGNERS:		Filip Gutica
--
--	PROGRAMMER:		Filip Gutica
/*-----------------------------------------------------------------------------*/
void Application::updatePlaylist()
{
    //Add to the list view
    ui->playList->setModel(new QStringListModel(QList<QString>::fromVector(fileList)));
}


void Application::on_playList_doubleClicked(const QModelIndex &index)
{
    QString song = index.data().toString();

    AudioPlayer audPlayer;

    audPlayer.setAudio(song);

    audPlayer.exec();

    qDebug() << index.data().toString();
}
