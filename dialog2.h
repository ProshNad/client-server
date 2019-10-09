#ifndef DIALOG2_H
#define DIALOG2_H

#include <QDialog>
#include <QTcpSocket>
#include <QTextEdit>
#include <QLineEdit>


namespace Ui {
class Dialog2;
}

class Dialog2 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog2(QWidget *parent = 0);
    ~Dialog2();

private slots:
    void on_pushButton_clicked();
    void slotReadyRead   (                            );
        void slotError       (QAbstractSocket::SocketError);
        void slotSendToServer(                            );
        void slotConnected   (                            );

        void on_spinBox_editingFinished();

        void on_spinBox_2_editingFinished();

private:
    Ui::Dialog2 *ui;
    QTcpSocket* m_pTcpSocket;
       QTextEdit*  m_ptxtInfo;
       QLineEdit*  m_ptxtInput;
       quint16     m_nNextBlockSize;

};

#endif // DIALOG2_H
