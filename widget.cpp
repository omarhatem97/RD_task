#include "widget.h"
#include "ui_widget.h"
#include <qdebug.h>
#include <QLineEdit>
#include <QFileDialog>
#include <QFileInfo>
#include <QColor>
#include <QColorDialog>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <QByteArray>
#include <cstring>

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

//    get_cipher_from_file("");

}

Widget::~Widget()
{
    delete ui;
}




int Widget::encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    /* Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits */
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
      handleErrors();

    /* Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
      handleErrors();
    ciphertext_len = len;

    /* Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int Widget::decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    /* Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
      handleErrors();

    /* Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
      handleErrors();
    plaintext_len = len;

    /* Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

void Widget::handleErrors()
{
    ERR_print_errors_fp(stderr);
    abort();
}

void Widget::aes_encrypt(unsigned char* plaintext)
{
    /* Set up the key and iv. Do I need to say to not hard code these in a
       * real application? :-)
       */

      /* A 256 bit key */
      unsigned char *key = (unsigned char *)"01234567890123456789012345678901";

      /* A 128 bit IV */
      unsigned char *iv = (unsigned char *)"0123456789012345";

      /* Message to be encrypted */
//      plaintext =
//                  (unsigned char *)"test for openssltest ";

      /* Buffer for ciphertext. Ensure the buffer is long enough for the
       * ciphertext which may be longer than the plaintext, dependant on the
       * algorithm and mode
       */
      unsigned char ciphertext[100000];

      /* Buffer for the decrypted text */
      unsigned char decryptedtext[100000];

      int decryptedtext_len, ciphertext_len;

      /* Encrypt the plaintext */
        ciphertext_len = encrypt (plaintext, strlen ((char *)plaintext), key, iv,
                                  ciphertext);

        /* Do something useful with the ciphertext here */
        printf("Ciphertext is:\n");
//        std::cout << ciphertext << std::endl;
        printf("%s\n", ciphertext);
//        BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);
        qDebug() << "cipher text: " << ciphertext << endl;

        //save ciphertext to file
        write_to_file(ciphertext, ciphertext_len);

        //-----------------Base64 Encode--------------------------------
        printf("\n");
        char encodedData[100];
        EVP_EncodeBlock((unsigned char *)encodedData, ciphertext, 16);
        printf("Base64 Encode Data: ");
        printf(encodedData);
        printf("\n");
        //---------------------------------------------------------------

//        /* Decrypt the ciphertext */
//        decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv,
//          decryptedtext);

////        std::cout << "decrypted: " << decryptedtext << std::endl;
//        /* Add a NULL terminator. We are expecting printable text */
//        decryptedtext[decryptedtext_len] = '\0';

//        /* Show the decrypted text */
//        printf("Decrypted text is:\n");
//        printf("%s\n", decryptedtext);
        for (int i = 0; i < 100000; ++i) {
            cipherTextArray[i] = ciphertext[i];
        }

}

void Widget::aes_decrypt(QString filePath, int ciphertext_len)
{
    /* A 256 bit key */
    unsigned char *key = (unsigned char *)"01234567890123456789012345678901";

    /* A 128 bit IV */
    unsigned char *iv = (unsigned char *)"0123456789012345";

    /* Buffer for the decrypted text */
    unsigned char decryptedtext[100000];

    /* Buffer for ciphertext. Ensure the buffer is long enough for the
     * ciphertext which may be longer than the plaintext, dependant on the
     * algorithm and mode
     */
    unsigned char ciphertext[100000];



    QByteArray arr= read_from_file("");
    unsigned char c[100000];
    std::memcpy(c,arr.constData(),arr.size());

    /* Decrypt the ciphertext */
    int decryptedtext_len = decrypt(c, ciphertext_len, key, iv,
      decryptedtext);

//        std::cout << "decrypted: " << decryptedtext << std::endl;
    /* Add a NULL terminator. We are expecting printable text */
    decryptedtext[decryptedtext_len] = '\0';


    ////save cipher text to file

    write_to_file(ciphertext, decryptedtext_len);

    ///

    /* Show the decrypted text */
    printf("Decrypted text is:\n");
    printf("%s\n", decryptedtext);
}

QByteArray Widget::read_from_file(QString filePath)
{
//        qDebug() << "file path: " << filePath << endl;
//        QString  fileContent;
//        //Save the file to disk

//        QFile file(filePath);
//        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
//            return "";

//        QTextStream in(&file);
//        QString line = in.readLine();

//        while(!line.isNull())
//        {
//             fileContent.append(line + "\n");
//             line = in.readLine();
//        }

//        file.close();
////        QByteArray ba = fileContent.toLocal8Bit();
////        char *res = ba.data();
//        qDebug() << "read from file: " << fileContent.toStdString().c_str()<<endl;
//        return fileContent.toStdString().c_str();

    QFile file("D:\\test.txt");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "error opening!" << endl;
        return NULL;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_9);

    QByteArray iContents = file.readAll();

    unsigned char* arr = new unsigned char[100000];

    std::memcpy(arr,iContents.constData(),iContents.size());

//    out << ciphertext;
//        for (int i = 0; i < 100000; ++i) {
//            arr[i] = (unsigned char)iContents[i];
//            qDebug() << arr[i] << ' ' << iContents[i] << endl;
//        }

//        file.flush();
    file.close();
    return iContents;
}

void Widget::write_decrypted_to_file(QString filePath)
{

}

void Widget::get_cipher_from_file(QString path)
{
    //just for debugging , it gets the cipher text from file
    const char* cipher = read_from_file("D:\\testEnc.txt");
    unsigned char* cipherText = (unsigned char*) cipher;
    while(cipherText){
        qDebug() << *(cipherText++) << endl;
    }
}

void Widget::write_to_file(unsigned char cipher[100000], int ciphertext_len)
{
    //Save the file to disk

//        if( saveFilePath.isEmpty())
//            return;

//        QString name = ui->nameLineEdit->text();
//        QFile file(save_abs_path+name +"."+ file_extension);

//        //Open the file
//        if( !file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
//            return;

////        QByteArray ba(cipherTextArray, 100000);
////        ba.resize(100000);


//        QTextStream out(&file);

//        const char * cipherText = (const char *) cipher;
//        QString str = QString::fromUtf8(cipherText, ciphertext_len);

//        for (int i=0; i<ciphertext_len; i++) {
//            out <<(unsigned char) cipher[i];

//        }
//        out << "\n";

//        file.close();
    QFile file("D:\\final.txt");
    if(!file.open(QIODevice::WriteOnly)){
        qDebug() << "error opening!" << endl;
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_9);
    for (int i = 0; i < 100000; ++i) {
        out << cipher[i];
    }
//    out << ciphertext;

    file.flush();
    file.close();
}

void Widget::on_browseButton_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this, "open a file", "C://");
    filePath = file_path;
//    qDebug() << "here:" <<file_path << endl;

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
//    qDebug() << "extension is: " <<fileInfo.suffix() << endl;

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
//            QString command = "cd "+ file_abs_path + " && " + "openssl aes-256-cbc -a -salt -in " +q+file_name+ q + " -out " + save_abs_path+name +"."+ file_extension + " -k " +pass;
////            qDebug() << command;

//            system(command.toStdString().c_str());
            const char* test = read_from_file(filePath);
            unsigned char* plaintext = (unsigned char*) test;
            aes_encrypt(plaintext);
            qDebug() << test << endl;
        }
        else{
            //decrypt
//            QString command = "cd "+ file_abs_path + " && " + "openssl aes-256-cbc -d -a -in " + file_abs_path+q+file_name+ q + " -out " + save_abs_path+name +"."+ file_extension + " -k " +pass;
////            qDebug() << command << endl;
//            system(command.toStdString().c_str());
            aes_decrypt(filePath , 0);
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

    saveFilePath = file_path;
    //get filename and set it
    QFileInfo fileInfo(file_path);


    QDir d = fileInfo.absoluteDir();
    save_abs_path = d.absolutePath();
//    qDebug() << "file path: " <<file_abs_path << endl;


    ui->saveLineEdit->setText(file_path);
}
