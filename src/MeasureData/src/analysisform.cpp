#include "analysisform.h"
#include "ui_analysisform.h"

KeyValuePair AnalysisForm::ITEM_TYPE_SWEEP[] = {
    KeyValuePair("number",QVariant(1),KeyValuePair::TYPE_TEXT, tr("Number")),
    KeyValuePair("node",  QVariant(),KeyValuePair::TYPE_TEXT, tr("Node")),
    KeyValuePair("mode",  QVariant("voltage"),KeyValuePair::TYPE_LIST, tr("Type")),
    KeyValuePair("method",QVariant("linear"),KeyValuePair::TYPE_LIST, tr("Method")),
    KeyValuePair("start", QVariant(),KeyValuePair::TYPE_TEXT, tr("Start")),
    KeyValuePair("stop",  QVariant(),KeyValuePair::TYPE_TEXT, tr("Stop")),
    KeyValuePair("step",  QVariant(),KeyValuePair::TYPE_TEXT, tr("Step"))
};

KeyValuePair AnalysisForm::ITEM_TYPE_CONST[] = {
    KeyValuePair("node",  QVariant(),KeyValuePair::TYPE_TEXT, tr("Node")),
    KeyValuePair("mode",  QVariant("voltage"),KeyValuePair::TYPE_LIST, tr("Type")),
    KeyValuePair("const",QVariant(),KeyValuePair::TYPE_TEXT, tr("Constant"))
};
KeyValuePair AnalysisForm::ITEM_TYPE_OUTPUT[] = {
    KeyValuePair("mode",  QVariant("voltage"),KeyValuePair::TYPE_LIST, tr("Type")),
    KeyValuePair("node",  QVariant(),KeyValuePair::TYPE_TEXT, tr("Node"))
};



AnalysisForm::AnalysisForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalysisForm) {
    ui->setupUi(this);

    storage_ = AnalysisStorage::instance();
    itemsInputView_ = new ListItemView();
    itemsOutputView_ = new ListItemView();
    itemValueInputView_ = new KeyValueView();
    itemValueOutputView_ = new KeyValueView();

    ui->typeInputItemComboBox->addItem( tr("Sweep"), "sweep" );
    ui->typeInputItemComboBox->addItem( tr("Constant"), "const" );
    ui->typeOutputItemComboBox->addItem( tr("Output"), "output" );
    ui->typeOutputItemComboBox->addItem( tr("Function"), "func" );
    changeTypeItemInput(0);

    // Analysis types
    ui->typeAnalysisComboBox->addItem( tr("DC"),  "dc"  );
    ui->typeAnalysisComboBox->addItem( tr("AC"),  "ac"  );
    ui->typeAnalysisComboBox->addItem( tr("TRAN"),"tran");

    openAnalysis( 1 );

    ui->inputSplitter->widget(1)->setMaximumWidth( 250 );
    ui->outputSplitter->widget(1)->setMaximumWidth( 250 );



    ui->itemInputTableView->setModel( itemValueInputView_ );
    ui->itemOutputTableView->setModel( itemValueOutputView_ );
    ui->inputItemsListView->setModel( itemsInputView_ );
    ui->outputItemsListView->setModel( itemsOutputView_ );


    // CONNECTS
    connect(ui->cancelButton,SIGNAL(clicked()),
            this,SLOT(restoreAnalysis()));
    connect(ui->saveButton,SIGNAL(clicked()),
            this,SLOT(saveAnalysis()));
    connect(ui->nameAnalysisText,SIGNAL(textChanged(QString)),
            this,SLOT(changeNameAnalysis(QString)));
    connect(ui->typeAnalysisComboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(changeTypeAnalysis(int)));

    // INPUTS TAB
    connect( ui->typeInputItemComboBox,SIGNAL(currentIndexChanged(int)),
                                  this, SLOT(changeTypeItemInput(int)) );
    connect( ui->insertItemInputButton,SIGNAL(clicked()),
             this,SLOT(insertInputItemClick()));
    connect( ui->removeItemInputButton,SIGNAL(clicked()),
             this,SLOT(removeInputItemRemove()));

    connect( ui->inputItemsListView, SIGNAL(clicked(QModelIndex)),
             this,SLOT(selectedItemInput(QModelIndex)));


    // OUTPUTS TAB
    connect( ui->typeOutputItemComboBox,SIGNAL(currentIndexChanged(int)),
                                   this,SLOT(changeTypeItemOutput(int)));
    connect( ui->insertItemOutputButton,SIGNAL(clicked()),
             this,SLOT(insertOutputItemClick()));
    connect( ui->removeItemOutputButton,SIGNAL(clicked()),
             this,SLOT(removeOutputItemClick()));



}



AnalysisForm::~AnalysisForm()
{
    delete ui;
}

void AnalysisForm::prepareItemsInput() {
    IAnalysisItem* item = NULL;
    int noItem = 0;
    QList< QPair<QString,QVariant> > items;

    foreach(item, model_.inputs()){
        items << QPair<QString,QVariant>(item->name(), noItem);
        noItem++;
    }

    itemsInputView_->setItems( items );
}

void AnalysisForm::prepareItemsOutput() {
    IAnalysisItem* item = NULL;
    QList< QPair<QString,QVariant> > items;
    int noItem=0;

    foreach(item, model_.outputs()){
        items << QPair<QString,QVariant>(item->name(), noItem);
        noItem++;
    }

    itemsOutputView_->setItems( items );
}

void AnalysisForm::openAnalysis(const int &analysisId) {
    openAnalysisImpl(storage_->openAnalysis( analysisId ));
}

void AnalysisForm::openAnalysisImpl(const AnalysisModel &model) {

    model_ = model;
    storedModel_ = model_;
    ui->nameAnalysisText->setText( model.name() );

    prepareItemsInput();
    prepareItemsOutput();

}

void AnalysisForm::changeTypeItemInput(const int &index) {
    if(ui->typeInputItemComboBox->currentIndex() != index){
        ui->typeInputItemComboBox->setCurrentIndex( index );
    }

    QString key = ui->typeInputItemComboBox->itemData( index ).toString();

    QVariantMap mapModes;
    mapModes.insert( "Voltage", "voltage" );
    mapModes.insert( "Current", "current" );
    QVariantMap mapSweepMethods;
    mapSweepMethods.insert("Linear","linear");
    mapSweepMethods.insert("List","list");


    // Sweep and const
    if( key.compare("sweep") == 0 ){
        itemValueInputView_->setPairs( ITEM_TYPE_SWEEP, 7 );
        itemValueInputView_->setPairData("method",mapSweepMethods);
    } else if( key.compare("const") == 0 ){
        itemValueInputView_->setPairs( ITEM_TYPE_CONST, 3 );
    } else{
        return;
    }

    itemValueInputView_->setPairData( "mode", mapModes );
    itemValueInputView_->fillDelegates( ui->itemInputTableView );
}

void AnalysisForm::changeTypeItemOutput(const int &index) {
    QString key = ui->typeOutputItemComboBox->itemData( index ).toString();
    QVariantMap mapModes;
    mapModes.insert( "Voltage", "voltage" );
    mapModes.insert( "Current", "current" );

    if( key.compare("output") == 0 ){
        itemValueOutputView_->setPairs( ITEM_TYPE_OUTPUT, 2 );
    } else {
        return;
    }
    itemValueOutputView_->setPairData( "mode", mapModes );
    itemValueOutputView_->fillDelegates( ui->itemOutputTableView );
}

void AnalysisForm::insertInputItemClick() {
    QString key = ui->typeInputItemComboBox->itemData(
                ui->typeInputItemComboBox->currentIndex() ).toString();

    if( key.compare("sweep") == 0 ){
        AnalysisItemSweep* item = new AnalysisItemSweep();
        item->setNumber( itemValueInputView_->getPair("number").value.toInt()  );
        item->setNode(   itemValueInputView_->getPair("node").value.toString() );
        item->setMode(   itemValueInputView_->getPair("mode").value.toString() );
        item->setMethod( itemValueInputView_->getPair("method").value.toString());
        item->setStart(  itemValueInputView_->getPair("start").value.toDouble());
        item->setStop(   itemValueInputView_->getPair("stop").value.toDouble());
        item->setStep(   itemValueInputView_->getPair("step").value.toDouble());
        model_.addInput( item );
        prepareItemsInput();
    } else if( key.compare("const") == 0 ){
        AnalysisItemConst* item = new AnalysisItemConst();
        item->setConstant( itemValueInputView_->getPair("const").value.toDouble() );
        item->setNode(   itemValueInputView_->getPair("node").value.toString() );
        item->setMode(   itemValueInputView_->getPair("mode").value.toString() );
        model_.addInput( item );
        prepareItemsInput();
    }
}

void AnalysisForm::removeInputItemRemove() {

}

void AnalysisForm::selectedItemInput(const QModelIndex &index) {
//    int noItem = index.data( Qt::UserRole ).toInt();
    int noItem = index.row();
    qDebug() << "Number item: " << noItem;

    if(model_.inputs().at(noItem)->getItemType() == ANALYSIS_ITEM_SWEEP){
        changeTypeItemInput( 0 );
        AnalysisItemSweep* item = static_cast<AnalysisItemSweep*>(model_.inputs().at(noItem));
        itemValueInputView_->setValue( "number", item->number() );
        itemValueInputView_->setValue( "node", item->node() );
        itemValueInputView_->setValue("mode",  item->modeToVariant());
        itemValueInputView_->setValue( "method", item->method());
        itemValueInputView_->setValue( "start", item->start() );
        itemValueInputView_->setValue("stop", item->stop());
        itemValueInputView_->setValue("step", item->step());
    }else if( model_.inputs().at(noItem)->getItemType() == ANALYSIS_ITEM_CONST ){
        changeTypeItemInput( 1 );
        AnalysisItemConst* item = static_cast<AnalysisItemConst*>(model_.inputs().at(noItem));
        itemValueInputView_->setValue( "node", item->node() );
        itemValueInputView_->setValue("mode",  item->modeToVariant());
        itemValueInputView_->setValue("const",item->constant());
    }
}

void AnalysisForm::changeNameAnalysis(const QString &name) {
    model_.setName( name );
}

void AnalysisForm::changeTypeAnalysis(const int &index) {
    QString key = ui->typeAnalysisComboBox->itemData( index ).toString();
    model_.setType( key );
}

void AnalysisForm::insertOutputItemClick() {
    QString key = ui->typeOutputItemComboBox->itemData(
                ui->typeOutputItemComboBox->currentIndex() ).toString();

    if( key.compare("sweep") == 0 ){
        AnalysisItemOutput* item = new AnalysisItemOutput();
        item->setNode(   itemValueInputView_->getPair("node").value.toString() );
        item->setMode(   itemValueInputView_->getPair("mode").value.toString() );
        model_.addOutput( item );
        prepareItemsOutput();
    } else {
    }

}

void AnalysisForm::removeOutputItemClick() {

}

void AnalysisForm::saveAnalysis() {
    if(storage_->saveAnalysis( model_ )){
        qDebug() << "Analysis saved";
    }else{
        qDebug() << "Analysis did not save";
    }
}

void AnalysisForm::restoreAnalysis() {
    openAnalysisImpl( storedModel_ );
}
