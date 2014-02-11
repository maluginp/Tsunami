#ifndef ADDMEASUREFORM_H
#define ADDMEASUREFORM_H

#include <QWidget>
#include "dbstorage/DbStorages.h"
#include "views/Views.h"

namespace Ui {
class MeasureWindow;
}

namespace tsunami{

class MeasureWindow : public QWidget {
    Q_OBJECT
public:
    enum Action{
        NEW,
        EDIT
    };

    MeasureWindow(int deviceId, QWidget *parent = 0);
    void createMeasure( int analysisId );
    void updateMeasure( int measureId );

    ~MeasureWindow();
private:
    void showMeasure(db::MeasureModel* measure);
    void showSourcesDescription();
    bool prepareMeasureData(db::AnalysisModel* analysis);

    void setHeaderData();
    QStringList getColumns(db::AnalysisModel* analysis);


    Ui::MeasureWindow *ui;
    int deviceId_;

    gui::KeyValueView* headerView_;
    gui::KeyValueView* attributesView_;
    gui::MeasureItemView* measureView_;

    db::MeasureModel* measure_;
    db::MeasureStorage* measureStorage_;

    QVector< gui::KeyValuePair > headerPair_;
private slots:
    void clickedAddAttributeButton();
    void clickedAddButton();
    void clickedExportButton();
    void clickedImportButton();

signals:
    void updatedDataBase();
};

}



#endif // ADDMEASUREFORM_H
