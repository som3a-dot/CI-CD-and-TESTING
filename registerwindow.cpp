#include "registerwindow.h"
#include "ui_registerwindow.h"
#include <QMessageBox>
#include <QPixmap>
#include <QCryptographicHash>
#include <QRegularExpression>

registerwindow::registerwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registerwindow)
{
    ui->setupUi(this);

    // Connect the PASSWORD text changed signal to update strength
    connect(ui->lineEdit_2, &QLineEdit::textChanged, this, &registerwindow::onPasswordTextChanged);
}

registerwindow::~registerwindow()
{
    delete ui;
}

void registerwindow::on_registerButton_clicked()
{
    QString username = ui->lineEdit->text().trimmed();
    QString password = ui->lineEdit_2->text().trimmed();
    QString confirmPassword = ui->lineEdit_3->text().trimmed();

    if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields are required.");
        return;
    }

    if (password != confirmPassword) {
        QMessageBox::warning(this, "Error", "Passwords do not match.");
        return;
    }

    QString strength = checkPasswordStrength(password);
    if (strength == "Weak") {
        QMessageBox::warning(this, "Warning", "Password is too weak. Use a stronger password.");
        return;
    } else if (strength == "Medium") {
        if (QMessageBox::question(this, "Confirmation", "Password is medium strength. Proceed anyway?",
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
            return;
        }
    }

    // Check if username already exists
    MainWindow *mainWindow = qobject_cast<MainWindow*>(parent());
    if (mainWindow && mainWindow->isUsernameTaken(username)) {
        QMessageBox::warning(this, "Error", "USERNAME ALREADY EXISTS!");
        return;
    }

    QString hashedPassword = hashPassword(password);

    // Use the new slot to add the user to registeredUsers
    if (mainWindow) {
        mainWindow->addRegisteredUser(username, hashedPassword);
    }

    QMessageBox::information(this, "Success", "REGISTERED SUCCESSFULLY!");
    this->hide();
}

void registerwindow::on_cancelButton_2_clicked()
{
    // Hide the registration window when CANCEL is clicked
    this->hide();
}

// Slot to update password strength label
void registerwindow::onPasswordTextChanged(const QString &text)
{
    QString strength = checkPasswordStrength(text);
    ui->passwordStrengthLabel->setText("PASSWORD STRENGTH: " + strength);

    // Optional: Set color based on strength
    if (strength == "Weak") {
        ui->passwordStrengthLabel->setStyleSheet("color: red;");
    } else if (strength == "Medium") {
        ui->passwordStrengthLabel->setStyleSheet("color: orange;");
    } else if (strength == "Strong") {
        ui->passwordStrengthLabel->setStyleSheet("color: green;");
    }
}

// Function to hash password using SHA-256
QString registerwindow::hashPassword(const QString &password)
{
    return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}

// Function to check password strength
QString registerwindow::checkPasswordStrength(const QString &password)
{
    int length = password.length();
    bool hasUpper = password.contains(QRegularExpression("[A-Z]")); // Use QRegularExpression
    bool hasLower = password.contains(QRegularExpression("[a-z]")); // Use QRegularExpression
    bool hasDigit = password.contains(QRegularExpression("[0-9]")); // Use QRegularExpression

    if (length < 6 || (!hasUpper && !hasLower && !hasDigit)) return "Weak";
    if (length < 8 || (!hasUpper || !hasLower || !hasDigit)) return "Medium";
    return "Strong";
}
