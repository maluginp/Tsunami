#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "dbstorage/SettingStorage.h"

namespace Ui {
class SettingsDialog;
}

namespace tsunami{

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private:
    void saveSettings();
    void openSettings();

    Ui::SettingsDialog *ui;
    db::SettingStorage* storage_;

private slots:
    void clickedApplyButton();
    void clickedSaveButton();
};
}
#endif // SETTINGSDIALOG_H
