#include "dialog2.h"
#include "ui_dialog2.h"
#include "QVBoxLayout"
#include "QMessageBox"
#include "QTcpSocket"
#include "QDataStream"
#include "QTime"
#include "QString"
#include "QDebug"

Dialog2::Dialog2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog2)
{
    ui->setupUi(this);
    m_pTcpSocket = new QTcpSocket(this);

        m_pTcpSocket->connectToHost("localhost", 2323);
        connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
        connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
        connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                this,         SLOT(slotError(QAbstractSocket::SocketError))
               );



        connect(ui->pushButton_2, SIGNAL(clicked()), SLOT(slotSendToServer()));


        m_nNextBlockSize = 0;

}

void Dialog2::slotReadyRead()
{
    QDataStream in(m_pTcpSocket);
    in.setVersion(QDataStream::Qt_4_2);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (m_pTcpSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QTime   time;
        QString str;
        in >> time >> str;

        ui->textEdit->append(time.toString() + " " + str);
        m_nNextBlockSize = 0;
    }
}

void Dialog2::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "Хост не найден." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "Хост закрыт." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "Соединение прервано." :
                     QString(m_pTcpSocket->errorString())
                    );
    ui->textEdit->append(strError);
}
void Dialog2::slotSendToServer()
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    QVector<QVector <int> > mas(ui->tableWidget->rowCount(),QVector <int> (ui->tableWidget->columnCount()));

    for(int i=0;i<ui->tableWidget->rowCount();i++){
        for(int j=0;j<ui->tableWidget->columnCount();j++){

            mas[i][j]=ui->tableWidget->item(i,j)->text().toInt();

        }
    }


QString r=QString("матрица размером:")+QString::number(ui->tableWidget->columnCount())
                  +QString("X")+QString::number(ui->tableWidget->rowCount());

    out << quint16(0) << QTime::currentTime() <<r<<mas<<ui->tableWidget->columnCount()<<ui->tableWidget->rowCount();//m_ptxtInput->text();

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    m_pTcpSocket->write(arrBlock);
  //  m_ptxtInput->setText("");
}

void Dialog2::slotConnected()
{
    ui->textEdit->append("Связь с сервером установлена.");
}

Dialog2::~Dialog2()
{
    m_pTcpSocket->close();
    delete m_pTcpSocket;
    delete ui;
}

void Dialog2::on_pushButton_clicked()
{
    this->~Dialog2();

}

void Dialog2::on_spinBox_editingFinished()
{
    ui->tableWidget->setColumnCount(ui->spinBox->value());
}

void Dialog2::on_spinBox_2_editingFinished()
{
   ui->tableWidget->setRowCount(ui->spinBox_2->value());
}
