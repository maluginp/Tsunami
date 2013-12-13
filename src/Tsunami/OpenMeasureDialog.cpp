#include "OpenMeasureDialog.h"
#include "ui_OpenMeasureDialog.h"
#include "models/MeasureModel.h"
#include "views/ListItemView.h"

namespace tsunami{

OpenMeasureDialog::OpenMeasureDialog(int deviceId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenMeasureDialog),
    measureId_(-1), deviceId_(deviceId)
{
    ui->setupUi(this);

    storage_ = db::MeasureStorage::instance();


    measuresView_ = new gui::ListItemView();


    ui->measuresListView->setModel(measuresView_);

    ui->measureTypeComboBox->addItem("ALL", "");
    ui->measureTypeComboBox->addItem( "DC",   "dc"   );
    ui->measureTypeComboBox->addItem( "AC",   "ac"   );
    ui->measureTypeComboBox->addItem( "TRAN", "tran" );

    changeMeasureAnalysis(0);


    connect(ui->measuresListView,SIGNAL(clicked(QModelIndex)),
            this,SLOT(clickedMeasureItem(QModelIndex)));
    connect(ui->openButton,SIGNAL(clicked()),this,SLOT(accept()));
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(reject()));
    connect(ui->measureNameLineEdit,SIGNAL(textChanged(QString)),this,SLOT(changeMeasureName(QString)));
    connect(ui->measureTypeComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(changeMeasureAnalysis(int)));

}

OpenMeasureDialog::~OpenMeasureDialog()
{
    delete ui;
}

int OpenMeasureDialog::getMeasureId(int deviceId, QWidget* parent =0 ) {
    OpenMeasureDialog dialog(deviceId,parent);
    if(dialog.exec() == QDialog::Accepted){
        return dialog.measureId_;
    }

    return -1;
}

void OpenMeasureDialog::clickedMeasureItem(const QModelIndex &index) {
    measureId_ = measuresView_->data( index, Qt::UserRole ).toInt();
}

void OpenMeasureDialog::changeMeasureName(const QString &name) {
    ui->measuresListView->setCurrentIndex( measuresView_->findByKey( name )  );
}

void OpenMeasureDialog::changeMeasureAnalysis(int index) {
    QString typeName = ui->measureTypeComboBox->itemData(index).toString();

    QList<db::MeasureModel*> measures;
    measuresView_->clear();
    if(typeName.isEmpty()){
        measures = storage_->getMeasuresByDeviceId(deviceId_);
    }else{
        QVariantMap criteria;

        criteria.insert("device",deviceId_);
        criteria.insert("analysis",typeName);

        measures = storage_->findMeasures(criteria);
    }
    foreach(db::MeasureModel* measure,measures){
        QString type = measure->typeJson();
        measuresView_->addItem( QString("%1 (%2)").arg(measure->name()).
                                arg(type.toUpper()), measure->id() );
    }

}

}
