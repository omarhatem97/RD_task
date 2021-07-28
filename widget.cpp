#include "widget.h"
#include "ui_widget.h"
#include <qdebug.h>
#include <QLineEdit>
#include <QFileDialog>
#include <QFileInfo>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);
    encryption = false;
    file_abs_path = ""; file_name = "";  file_size = "";  file_extension ="";

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_browseButton_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this, "open a file", "C://");
    qDebug() << file_path << endl;

    //get filename and set it
    QFileInfo fileInfo(file_path);

    file_name = fileInfo.fileName();
    file_size = QString::number(fileInfo.size());
    qDebug() << "size is: " <<file_size << endl;

    QDir d = fileInfo.absoluteDir();
    file_abs_path=d.absolutePath();
    qDebug() << "file path: " <<file_abs_path << endl;

    file_extension = fileInfo.completeSuffix();
    QFileInfo fileInfo1(file_path);
    qDebug() << "extension is: " <<fileInfo.completeSuffix() << endl;

    ui->filePathLineEdit->setText(file_path);
}

void Widget::on_encryptRadioButton_toggled(bool checked)
{
    if(checked)
        encryption = true;
}

void Widget::on_decryptRadioButton_toggled(bool checked)
{
    if(checked)
        encryption = false;
}

void Widget::on_pushButton_clicked()
{
    //check for empty lineEdit boxes
    if(ui->filePathLineEdit->text().isEmpty()){
        ui->pathLabel->setText("Please Enter the file path!");
        if(ui->lineEditPassword->text().isEmpty()){
            ui->passwordLabel->setText("Please Enter the password!");
        }
        return;
    }
    else{
        if(encryption){
            QString pass = ui->lineEditPassword->text();
            QString command = "cd "+ file_abs_path + " && " + "openssl aes-256-cbc -a -salt -in " + file_name + " -out " + file_name + ".enc" + " -k " +pass;
            qDebug() << command;
//            system("openssl aes-256-cbc -a -salt -in secrets.txt -out secrets.txt.enc");
            system(command.toStdString().c_str());
        }
        else{
            system("openssl enc -d -aes-256-cbc -in encrypted.data -out un_encrypted.data");
        }
    }

    if(encryption) ui->statusLabel->setText("Encrypted succefully!");
    ui->statusLabel->setText("Decrypted succefully!");

}
