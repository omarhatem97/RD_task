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
    QString file_abs_path, file_name, file_size, file_extension, save_abs_path, save_name, complete_file_loc;
};

#endif // WIDGET_H
