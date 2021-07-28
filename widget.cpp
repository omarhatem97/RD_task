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
    ui->nameLabel->setText("");
    ui->saveLabel->setText("");
    ui->passwordLabel->setText("");
    ui->statusLabel->setText("");
    ui->pathLabel->setText("");
    ui->optionLabel->setText("");

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
    qDebug() << "here:" <<file_path << endl;
    complete_file_loc = file_path;
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
        option = "e";

}

void Widget::on_decryptRadioButton_toggled(bool checked)
{
    if(checked)
        option = "d";
}

void Widget::on_pushButton_clicked()
{
    //check for empty lineEdit boxes
    if(ui->filePathLineEdit->text().isEmpty()){
        ui->pathLabel->setText("Please Enter the file path!");
        if(ui->lineEditPassword->text().isEmpty()){
            ui->passwordLabel->setText("Please Enter the password!");
        }
        if(ui->saveLineEdit->text().isEmpty()){
            ui->saveLabel->setText("Please Enter a path!");
        }
        if(ui->nameLineEdit->text().isEmpty()){
            ui->nameLabel->setText("Please Enter the name!");
        }
        if(option != "e" || option != "d"){
            ui->optionLabel->setText("Please select an option!");
        }
        return;
    }
    else{
        QString pass = ui->lineEditPassword->text();
        QString name = ui->nameLineEdit->text();
        QString q = "\""; //quotations
        if(encryption){

            QString command = "cd "+ file_abs_path + " && " + "openssl aes-256-cbc -a -salt -in " +q+file_name+ q + " -out " + save_abs_path+name +"."+ file_extension + " -k " +pass;
            qDebug() << command;
//            system("openssl aes-256-cbc -a -salt -in secrets.txt -out secrets.txt.enc");
            system(command.toStdString().c_str());
        }
        else{
            QString command = "cd "+ file_abs_path + " && " + "openssl aes-256-cbc -d -a -in " + file_abs_path+q+file_name+ q + " -out " + save_abs_path+name +"."+ file_extension + " -k " +pass;
            qDebug() << command << endl;
            system(command.toStdString().c_str());
        }
    }

    if(encryption)
        ui->statusLabel->setText("Encrypted succefully!");
    else
        ui->statusLabel->setText("Decrypted succefully!");

}


void Widget::on_browseSaveButton_clicked()
{
//    QString file_path = QFileDialog::getOpenFileName(this, "open a file", "C://");
    QString file_path = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "C://",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
//    qDebug() << file_path << endl;

    //get filename and set it
    QFileInfo fileInfo(file_path);

//    file_name = fileInfo.fileName();
//    file_size = QString::number(fileInfo.size());
//    qDebug() << "size is: " <<file_size << endl;

    QDir d = fileInfo.absoluteDir();
    save_abs_path = d.absolutePath();
    qDebug() << "file path: " <<file_abs_path << endl;

//    file_extension = fileInfo.completeSuffix();
    QFileInfo fileInfo1(file_path);
    qDebug() << "extension is: " <<fileInfo.completeSuffix() << endl;

    ui->saveLineEdit->setText(file_path);
}
