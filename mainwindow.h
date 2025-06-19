#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QMap>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Public method to check if a username exists
    bool isUsernameTaken(const QString &username) const;

public slots:
    void addRegisteredUser(const QString &username, const QString &hashedPassword);

private slots:
    void on_RegisterButton_2_clicked();
    void on_LoginButton_clicked();
    void on_GuestButton_4_clicked();
    void on_clearButton_3_clicked();

private:
    Ui::MainWindow *ui;
    void openGameBoard();
    QMap<QString, QString> registeredUsers; // Username/Email -> Hashed Password
    QString hashPassword(const QString &password); // Member function to hash password
    void loadRegisteredUsers();
    void saveRegisteredUsers();
};
#endif // MAINWINDOW_H
