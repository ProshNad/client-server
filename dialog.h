#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include <QTcpServer>
#include <QTextEdit>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
private:
     QTcpServer* m_ptcpServer;
       QTextEdit*  m_ptxt;
       quint16     m_nNextBlockSize;

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_pushButton_clicked();
    void slotNewConnection();
    void slotReadClient();

private:
    Ui::Dialog *ui;
   void sendToClient(QTcpSocket* pSocket, const QString& str);
};

#endif // DIALOG_H
