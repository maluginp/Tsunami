#include "OpenMeasureDialog.h"
#include "ui_OpenMeasureDialog.h"
#include "dbstorage/measurestorage.h"
#include "models/measuremodel.h"
#include "views/listitemview.h"

namespace tsunami{

OpenMeasureDialog::OpenMeasureDialog(int deviceId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenMeasureDialog),
    measureId_(-1)
{
    ui->setupUi(this);

    db::MeasureStorage* storage = db::MeasureStorage::instance();

    QList<db::MeasureModel*> measures = storage->getMeasuresByDeviceId(deviceId);

    measuresView_ = new gui::ListItemView();

    foreach(db::MeasureModel* measure,measures){
        measuresView_->addItem( measure->name(), measure->id() );
    }

    ui->measuresListView->setModel(measuresView_);

    connect(ui->measuresListView,SIGNAL(clicked(QModelIndex)),
            this,SLOT(clickedMeasureItem(QModelIndex)));
    connect(ui->openButton,SIGNAL(clicked()),this,SLOT(accept()));
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(reject()));
}

OpenMeasureDialog::~OpenMeasureDialog()
{
    delete ui;
}

int OpenMeasureDialog::getMeasureId(int deviceId) {
    OpenMeasureDialog dialog(deviceId);
    if(dialog.exec() == QDialog::Accepted){
        return dialog.measureId_;
    }

    return -1;
}

void OpenMeasureDialog::clickedMeasureItem(const QModelIndex &index) {
    measureId_ = measuresView_->data( index, Qt::UserRole ).toInt();
}

}
