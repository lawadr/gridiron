/*
Copyright (c) 2012-2013 Lawrence Adranghi

See LICENSE in root directory.
*/

#include <QtGui/qdialog.h>
#include <QtGui/qdialogbuttonbox.h>

class QLineEdit;
class QPushButton;

class NewProjectDialog : public QDialog {
    Q_OBJECT

public:
    NewProjectDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0);

    QString getName() const;
    QString getLocation() const;

private:
    QLineEdit* nameEdit_;
    QLineEdit* locationEdit_;
    QPushButton* okButton_;

    private slots:
        void nameChange();
        void locationChange();
        void browse();
};
