#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
namespace tsunami{
SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog) {
    ui->setupUi(this);

    storage_ = db::SettingStorage::instance();
    openSettings();

    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(reject()));
    connect(ui->applyButton,SIGNAL(clicked()),this,SLOT(clickedApplyButton()));
    connect(ui->saveButton,SIGNAL(clicked()),this,SLOT(clickedSaveButton()));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::saveSettings() {
    bool ok;
    storage_->saveValue( "spice/ngspice/path", ui->spicePathLineEdit->text() );
//    storage_->saveValue( "optimize/method", ui->optimizeMethodComboBox->currentT )
    storage_->saveValue("optimize/max_iteration", ui->optimizeMaxIterationsLineEdit->text().toInt());
    storage_->saveValue("optimize/tolerance/function",ui->optimizeToleranceFunctionLineEdit->text().toDouble());


}

void SettingsDialog::openSettings() {

    ui->spiceSimulatorComboBox->addItem( "NGSpice", QVariant("ngspice") );
    ui->spiceSimulatorComboBox->setEnabled(false);

    ui->spicePathLineEdit->setText( storage_->value( "spice/ngspice/path" ).toString() );
    ui->optimizeMethodComboBox->addItem( tr("Hooke-Jeeves"), QVariant("hookejeeves") );
    ui->optimizeMethodComboBox->setEnabled(false);
    ui->optimizeMaxIterationsLineEdit->setText( storage_->value("optimize/max_iteration").toString() );
    ui->optimizeToleranceFunctionLineEdit->setText( storage_->value("optimize/tolerance/function").toString() );


}

void SettingsDialog::clickedApplyButton() {
    saveSettings();
}

void SettingsDialog::clickedSaveButton() {
    saveSettings();
    accept();

}
}
