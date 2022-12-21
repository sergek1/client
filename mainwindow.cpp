#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    nextBlockSize = 0;
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::slotReadyRead() {
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_4);
    if (in.status()==QDataStream::Ok) {
//                /*QVector<*/QString storage;
//                in >> storage;
//                nextBlockSize = 0;
//                QString common_string;
////                QString key_string = storage.at(0);
////                    std::cout<<"key_string"<<storage[0].toStdString();
//                std::cout<<"после отправки size = "<<storage.size()<<std::endl;
        for(;;){
            if(nextBlockSize == 0) {
                if(socket->bytesAvailable()<2){
                    break;
                }
                in >> nextBlockSize;
            }
            if(socket->bytesAvailable() < nextBlockSize){
                break;
            }
            //            QVector<QString> storage{};
            //            storage.reserve(10000);
            QString storage;
            in >> storage;
            nextBlockSize = 0;
//            QString common_string;
//            QString key_string = storage.at(0);
            //            std::cout<<key_string,<<storage[0]
            std::cout<<"после отправки size = "<<storage.size()<<std::endl;
//            int key =  stoi(key_string.toStdString());
            //            for (int i = 1; i < storage.size(); ++i) {
            //                common_string += storage.at(i);
            //            }
            //            for (int i = 0; i < common_string.size(); ++i) {
            //                int temp = (common_string.toStdString()[i] + key);
            //                common_string[i] = (char)temp;
            //            }
//            for (int i = 0; i < storage.size(); ++i) {
//                int temp = (storage.toStdString()[i] + 1/*key*/);
//                storage[i] = (char)temp;
//            }
            ui->textBrowser->append(storage);
            SendToServer(storage);

        }
    } else {
        ui->textBrowser->append("read error in client");
    }
}

void MainWindow::SendToServer(QString str) {
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << quint64(0)<<str;
    out.device()->seek(0);
    out << quint64(Data.size() - sizeof(quint64));
//    out<<str;
    socket->write(Data);

//    Data.clear();
//    QDataStream out(&Data, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_6_4);
//    out <<quint64(0)<<str;
//    out.device()->seek(0);
//    out << quint64(Data.size() - sizeof(quint64));
//    out<<str;
//    std::cout<<""<<std::endl;
//    Sockets[which_client]->write(Data);

}

void MainWindow::on_connect_clicked() {
    server_ip = ui->server_ip->text();
    socket->connectToHost(server_ip, 2323);
}
