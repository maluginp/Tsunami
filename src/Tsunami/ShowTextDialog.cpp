#include "ShowTextDialog.h"
#include "ui_ShowTextDialog.h"
#include <QClipboard>
#include <QFile>
#include <QFileDialog>
namespace tsunami{
ShowTextDialog::ShowTextDialog(const QString &title, const QString &message, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowTextDialog)
{
    ui->setupUi(this);

    ui->messageText->setText(message);
    setWindowTitle( title );

    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(reject()));
    connect(ui->saveFile,SIGNAL(clicked()),this,SLOT(clickedSaveFile()));
    connect(ui->copyToClipboardButton,SIGNAL(clicked()),this,SLOT(clippedToClipboard()));
}


ShowTextDialog::~ShowTextDialog() {
    delete ui;
}

void ShowTextDialog::clippedToClipboard() {
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText( ui->messageText->toPlainText() );
}

void ShowTextDialog::clickedSaveFile() {
    QString fileName =  QFileDialog::getSaveFileName();
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly)){
        return;
    }

    file.write( ui->messageText->toPlainText().toAscii() );

    file.close();
}
}
