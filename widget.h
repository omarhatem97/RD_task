#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
                unsigned char *iv, unsigned char *ciphertext);

    int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
                unsigned char *iv, unsigned char *plaintext);

    void handleErrors(void);

    void aes_encrypt(unsigned char* plainText);

    void aes_decrypt(QString filePath, int ciphertext_len);

    QByteArray read_from_file(QString filePath);

    void write_to_file(unsigned char[100000], int);
    void write_decrypted_to_file(QString filePath);
    void get_cipher_from_file(QString path);

private slots:
    void on_browseButton_clicked();

    void on_encryptRadioButton_toggled(bool checked);

    void on_decryptRadioButton_toggled(bool checked);

    void on_pushButton_clicked();

    void on_browseSaveButton_clicked();

private:
    Ui::Widget *ui;
    bool encryption;
    QString option;
    QString file_abs_path, filePath, saveFilePath, file_name, file_size, file_extension, save_abs_path, save_name;
    unsigned char cipherTextArray[100000];
};

#endif // WIDGET_H
