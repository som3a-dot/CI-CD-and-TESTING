#ifndef SYMBOLWINDOW_H
#define SYMBOLWINDOW_H

#include <QMainWindow>

namespace Ui {
class SymbolWindow;
}

class SymbolWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SymbolWindow(QWidget *parent = nullptr);
    ~SymbolWindow();
    QString getSelectedSymbol() const;

signals:
    void symbolSelected(const QString &symbol);

private slots:
    void on_xButton_clicked();
    void on_oButton_2_clicked();

private:
    Ui::SymbolWindow *ui;
    QString selectedSymbol;
};

#endif // SYMBOLWINDOW_H
