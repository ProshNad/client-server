#include "dialog.h"
#include "ui_dialog.h"
#include "QVBoxLayout"
#include "QMessageBox"
#include "QTcpSocket"
#include "QDataStream"
#include "QTime"
#include "QDebug"
#include "QLabel"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    m_ptcpServer = new QTcpServer();

        if (!m_ptcpServer->listen(QHostAddress::Any, 2323)) {
            QMessageBox::critical(0,
                                  "Server Error",
                                  "Unable to start the server:"
                                  + m_ptcpServer->errorString()
                                 );
            m_ptcpServer->close();
            return;
        }
        connect(m_ptcpServer, SIGNAL(newConnection()),
                this,         SLOT(slotNewConnection())
               );

        m_nNextBlockSize=0;

}
void Dialog::slotNewConnection()
{
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(deleteLater())
           );
    connect(pClientSocket, SIGNAL(readyRead()),
            this,          SLOT(slotReadClient())
           );

    sendToClient(pClientSocket, "Соединение установлено!");
}
void Dialog::slotReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_4_2);
    ///


    ////

    for (;;) {


        if (!m_nNextBlockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QTime   time;
        QString str;
        QVector<QVector <int> > mas;
        int i;
        int j;
        in >> time >> str>>mas>>i>>j;
        qDebug()<<mas;

        int sum1=0;
        int sum11=0;
        int i1=0;
        int sum2=0;
        int sum22=0;
        int j2=0;

        for(int ii=0;ii<j;ii++){
            sum1=0;
            for(int jj=0;jj<i;jj++){

                sum1=sum1+mas[ii][jj];

            }
            if(sum1>sum11){
                i1=ii;
                sum11=sum1;
            }
        }
        qDebug()<<"max i:"<<i1<<" and sum is:"<<sum11;//строки

        for(int ii=0;ii<i;ii++){
            sum2=0;
            for(int jj=0;jj<j;jj++){

                sum2=sum2+mas[jj][ii];

            }
            if(sum2>sum22){
                j2=ii;
                sum22=sum2;
            }
        }
        qDebug()<<"max j:"<<j2<<" and sum is:"<<sum22;//столбцы


        QString strMessage =
            time.toString() + " " + "Сообщение от клиента - " + str;
        ui->textEdit->append(strMessage);


        m_nNextBlockSize = 0;


        sendToClient(pClientSocket,
                     "Ответ сервера:строка "+QString::number(i1+1)+
                     ", с суммой:"+QString::number(sum11)+
                     "; столбец "+QString::number(j2+1)+
                     ", с суммой:"+QString::number(sum22)
                    );


    }
}

void Dialog::sendToClient(QTcpSocket* pSocket, const QString& str)
{

    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << QTime::currentTime() << str;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket->write(arrBlock);

}
Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
   this->~Dialog();
}
