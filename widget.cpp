#include "widget.h"
#include "ui_widget.h"
#include <qdebug.h>
#include <QLineEdit>
#include <QFileDialog>
#include <QFileInfo>
#include <QColor>
#include <QColorDialog>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //init labels
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);
    ui->nameLabel->setText("");
    ui->saveLabel->setText("");
    ui->passwordLabel->setText("");
    ui->statusLabel->setText("");
    ui->pathLabel->setText("");
    ui->optionLabel->setText("");
    ui->detailsLabel->setText("");

    //init data members
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

    //get filename
    QFileInfo fileInfo(file_path);

    file_name = fileInfo.fileName();

    // get file size
    double res = double(fileInfo.size()) / 1000000;

    file_size = QString::number(res, 'f', 6);

    //get abs dir
    QDir d = fileInfo.absoluteDir();
    file_abs_path=d.absolutePath();

    //get extension

    file_extension = fileInfo.suffix();
    QFileInfo fileInfo1(file_path);
    qDebug() << "extension is: " <<fileInfo.suffix() << endl;

    //set labels
    ui->filePathLineEdit->setText(file_path);
    ui->detailsLabel->setText("size: " + file_size + " MB" + "   extension: ." + file_extension);
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
        ui->pathLabel->setText("<font color='red'>Please Enter the file path!</font>");
        if(ui->lineEditPassword->text().isEmpty()){
            ui->passwordLabel->setText("<font color='red'>Please Enter the password!</font>");

        }
        if(ui->saveLineEdit->text().isEmpty()){
            ui->saveLabel->setText("<font color='red'>Please Enter a path!</font>");
        }
        if(ui->nameLineEdit->text().isEmpty()){
            ui->nameLabel->setText("<font color='red'>Please Enter the name!</font>");
        }
        if(option != "e" || option != "d"){
            ui->optionLabel->setText("<font color='red'>Please select an option!</font>");
        }
        return;
    }
    else{
        QString pass = ui->lineEditPassword->text();
        QString name = ui->nameLineEdit->text();
        QString q = "\""; //quotations
        if(option == "e"){
            //encrypt
            QString command = "cd "+ file_abs_path + " && " + "openssl aes-256-cbc -a -salt -in " +q+file_name+ q + " -out " + save_abs_path+name +"."+ file_extension + " -k " +pass;
            qDebug() << command;

            system(command.toStdString().c_str());
        }
        else{
            //decrypt
            QString command = "cd "+ file_abs_path + " && " + "openssl aes-256-cbc -d -a -in " + file_abs_path+q+file_name+ q + " -out " + save_abs_path+name +"."+ file_extension + " -k " +pass;
            qDebug() << command << endl;
            system(command.toStdString().c_str());
        }
    }

    if(option == "e")
        ui->statusLabel->setText("<font color='green'>Encrypted succefully!</font>");
    else
        ui->statusLabel->setText("<font color='green'>Decrypted succefully!</font>");

}


void Widget::on_browseSaveButton_clicked()
{

    QString file_path = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "C://",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);


    //get filename and set it
    QFileInfo fileInfo(file_path);


    QDir d = fileInfo.absoluteDir();
    save_abs_path = d.absolutePath();
    qDebug() << "file path: " <<file_abs_path << endl;


    ui->saveLineEdit->setText(file_path);
}
