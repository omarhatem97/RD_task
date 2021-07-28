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

private:
    Ui::Widget *ui;
    bool encryption;
    QString file_abs_path, file_name, file_size, file_extension;
};

#endif // WIDGET_H
