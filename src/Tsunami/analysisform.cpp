#include "analysisform.h"
#include "ui_analysisform.h"

namespace tsunami{
int AnalysisForm::nTypeLinear_ = 7;
gui::KeyValuePair AnalysisForm::ITEM_TYPE_LINEAR[] = {
    gui::KeyValuePair("number",QVariant(1),gui::KeyValuePair::TYPE_TEXT, tr("Number")),
    gui::KeyValuePair("node",  QVariant(),gui::KeyValuePair::TYPE_TEXT, tr("Node")),
    gui::KeyValuePair("mode",  QVariant("voltage"),gui::KeyValuePair::TYPE_LIST, tr("Type")),
    gui::KeyValuePair("method",QVariant("linear"),gui::KeyValuePair::TYPE_LIST, tr("Method")),
    gui::KeyValuePair("start", QVariant(),gui::KeyValuePair::TYPE_TEXT, tr("Start")),
    gui::KeyValuePair("stop",  QVariant(),gui::KeyValuePair::TYPE_TEXT, tr("Stop")),
    gui::KeyValuePair("step",  QVariant(),gui::KeyValuePair::TYPE_TEXT, tr("Step"))
};
int AnalysisForm::nTypeList_ = 5;
gui::KeyValuePair AnalysisForm::ITEM_TYPE_LIST[] = {
    gui::KeyValuePair("number",QVariant(1),gui::KeyValuePair::TYPE_TEXT, tr("Number")),
    gui::KeyValuePair("node",  QVariant(),gui::KeyValuePair::TYPE_TEXT, tr("Node")),
    gui::KeyValuePair("mode",  QVariant("voltage"),gui::KeyValuePair::TYPE_LIST, tr("Type")),
    gui::KeyValuePair("method",QVariant("list"),gui::KeyValuePair::TYPE_LIST, tr("Method")),
    gui::KeyValuePair("list", QVariant(""),gui::KeyValuePair::TYPE_TEXT, tr("List"))
};
int AnalysisForm::nTypeConst_ = 3;
gui::KeyValuePair AnalysisForm::ITEM_TYPE_CONST[] = {
    gui::KeyValuePair("node",  QVariant(),gui::KeyValuePair::TYPE_TEXT, tr("Node")),
    gui::KeyValuePair("mode",  QVariant("voltage"),gui::KeyValuePair::TYPE_LIST, tr("Type")),
    gui::KeyValuePair("const",QVariant(),gui::KeyValuePair::TYPE_TEXT, tr("Constant"))
};
int AnalysisForm::nTypeOutput_ = 2;
gui::KeyValuePair AnalysisForm::ITEM_TYPE_OUTPUT[] = {
    gui::KeyValuePair("node",  QVariant(),gui::KeyValuePair::TYPE_TEXT, tr("Node")),
    gui::KeyValuePair("mode",  QVariant("voltage"),gui::KeyValuePair::TYPE_LIST, tr("Type"))
};


AnalysisForm::AnalysisForm(int analysisId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalysisForm) {
    ui->setupUi(this);


    storage_             = db::AnalysisStorage::instance();
    model_               = NULL;
    itemsInputView_      = new gui::ListItemView();
    itemsOutputView_     = new gui::ListItemView();
    itemValueInputView_  = new gui::KeyValueView();
    itemValueOutputView_ = new gui::KeyValueView();

    ui->typeInputItemComboBox->addItem( tr("Constant"), "const" );
    ui->typeInputItemComboBox->addItem( tr("Linear"), "linear" );
    ui->typeInputItemComboBox->addItem( tr("List"), "list" );

    changeTypeItemInput(0);
    changeTypeItemOutput(0);


    // Analysis types
    ui->typeAnalysisComboBox->addItem( tr("DC"),  "dc"  );
    ui->typeAnalysisComboBox->addItem( tr("AC"),  "ac"  );
    ui->typeAnalysisComboBox->addItem( tr("TRAN"),"tran");

    changeTypeAnalysis( 0 );
    openAnalysis( analysisId );

    // GUI
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
    connect( ui->outputItemsListView, SIGNAL(clicked(QModelIndex)),
             this,SLOT(selectedItemOutput(QModelIndex)));



}

AnalysisForm::~AnalysisForm() {
    delete ui;
}

Source AnalysisForm::defaultSource(SourceDirection direction) {
    Source source;
    if(direction == SOURCE_DIRECTION_INPUT){
        source.node("");
        source.direction(SOURCE_DIRECTION_INPUT);
        source.method(SOURCE_METHOD_CONST);
        source.addConfiguration( "const", 0.0 );
        source.mode(SOURCE_MODE_VOLTAGE);
    }else if(direction == SOURCE_DIRECTION_OUTPUT){
        source.node("");
        source.direction(SOURCE_DIRECTION_OUTPUT);
        source.method(SOURCE_METHOD_CONST);
        source.mode(SOURCE_MODE_VOLTAGE);
    }
    return source;
}

void AnalysisForm::openAnalysis(int analysisId) {
    model_ = storage_->openAnalysis( analysisId );

    QList<Source> sources;

    // Preparing input
    sources = model_->sources( SOURCE_DIRECTION_INPUT );

    itemsInputView_->clear();
    foreach(Source source, sources){
        itemsInputView_->addItem( source.title(), source.node()  );
    }

    itemsOutputView_->clear();
    sources = model_->sources( SOURCE_DIRECTION_OUTPUT );
    foreach(Source source, sources){
        itemsOutputView_->addItem( source.title(), source.node() );
    }


}


void AnalysisForm::changeTypeItemInput(int index) {
    QString key = ui->typeInputItemComboBox->itemData( index ).toString();

    QString node = itemValueInputView_->getPair( "node" ).value.toString();
    SourceDirection direction = SOURCE_DIRECTION_INPUT;

    Source source;
    if( model_->sourceExists(node,direction) ){
        source = model_->findSource(node,direction);
    }else{
        source = defaultSource(direction);
        source.node(node);
    }

    if(key.compare("linear") == 0){
        if(source.method() != SOURCE_METHOD_LINEAR){
            source.method(SOURCE_METHOD_LINEAR);
            source.addConfiguration("start",0.0);
            source.addConfiguration("stop",5.0);
            source.addConfiguration("step",1.0);
        }
    }else if(key.compare("list") == 0){
        if(source.method() != SOURCE_METHOD_LIST){
            source.method(SOURCE_METHOD_LIST);
            source.addConfiguration("list","");
        }
    }else if( key.compare("const") == 0 ){
        if(source.method() != SOURCE_METHOD_CONST){
            source.method(SOURCE_METHOD_CONST);
            source.addConfiguration( "const", 0.0 );
        }
    }

}

void AnalysisForm::changeTypeItemOutput(int index) {
    Q_ASSERT(false);
}


void AnalysisForm::selectedItemInput(const QModelIndex &index) {
    QString node = index.data(Qt::EditRole).toString();

    QList<Source> sources = model_->sources( SOURCE_DIRECTION_INPUT );
    int nSources = sources.size();
    for(int i=0; i < nSources; ++i){
        if(sources[i].node() == node){
            showSource( sources[i] );
            break;
        }
    }



}

void AnalysisForm::selectedItemOutput(const QModelIndex &index) {
    QString node = index.data(Qt::EditRole).toString();

    QList<Source> sources = model_->sources( SOURCE_DIRECTION_OUTPUT );
    int nSources = sources.size();
    for(int i=0; i < nSources; ++i){
        if(sources[i].node() == node){
            showSource( sources[i] );
            break;
        }
    }

}

void AnalysisForm::showSource(const Source &source) {
    gui::KeyValueView* items;
    if(source.direction() == SOURCE_DIRECTION_INPUT){
        items = itemValueInputView_;
        if(source.method() == SOURCE_METHOD_LINEAR){
            items->setPairs( ITEM_TYPE_LINEAR, nTypeLinear_ );

            items->setValue( "number", source.configuration("number") );
            items->setValue("node",source.node());
            items->setValue("mode",source.modeJson());
            items->setValue("method",source.methodJson());
            items->setValue("start", source.configuration("start"));
            items->setValue("stop",source.configuration("stop"));
            items->setValue("step",source.configuration("step"));

        }else if(source.method() == SOURCE_METHOD_CONST){

            items->setPairs( ITEM_TYPE_CONST, nTypeConst_ );
            items->setValue("node",source.node());
            items->setValue("mode",source.modeJson());
            items->setValue("const", source.configuration("const"));

        }else if(source.method() == SOURCE_METHOD_LIST){
            items->setPairs( ITEM_TYPE_LIST, nTypeList_);
            items->setValue( "number", source.configuration("number") );
            items->setValue("node",source.node());
            items->setValue("mode",source.modeJson());
            items->setValue("method",source.methodJson());
            items->setValue("list", source.configuration("list"));
        }


    }else if(source.direction() == SOURCE_DIRECTION_OUTPUT){
        itemValueOutputView_->setPairs( ITEM_TYPE_OUTPUT, nTypeOutput_ );
    }else{
        Q_ASSERT(false);
    }


}

void AnalysisForm::changeNameAnalysis(const QString &name) {
    model_->name(name);
}

void AnalysisForm::changeTypeAnalysis(int index) {
    QString key = ui->typeAnalysisComboBox->itemData( index ).toString();
    model_->type( key );
}

void AnalysisForm::insertInputItemClick() {

    Source source;
    source.node( itemValueInputView_->getPair( "node" ).value.toString());
    source.method( itemValueInputView_->getPair( "method" ).value.toString() );
    source.mode( itemValueInputView_->getPair( "mode" ).value.toString() );
    source.direction( SOURCE_DIRECTION_INPUT);

    QVariantMap configuration;
    switch( source.method() ){
    case SOURCE_METHOD_CONST:
        configuration.insert("const", itemValueInputView_->getPair( "const" ).value.toDouble());
        break;
    case SOURCE_METHOD_LINEAR:
        configuration.insert("number", itemValueInputView_->getPair( "number" ).value.toInt());
        configuration.insert("start", itemValueInputView_->getPair( "start" ).value.toDouble());
        configuration.insert("stop", itemValueInputView_->getPair( "stop" ).value.toDouble());
        configuration.insert("step", itemValueInputView_->getPair( "step" ).value.toDouble());
        break;
    case SOURCE_METHOD_LIST:
        configuration.insert("number", itemValueInputView_->getPair( "number" ).value.toInt());
        configuration.insert("list", itemValueInputView_->getPair( "list" ).value.toString());
        break;
    }

    source.configuration( configuration );

    model_->addSource( source );




}

void AnalysisForm::removeInputItemRemove() {
    Source source;
    source.node( itemValueInputView_->getPair( "node" ).value.toString());
    source.method( itemValueInputView_->getPair( "method" ).value.toString() );
    source.mode( itemValueInputView_->getPair( "mode" ).value.toString() );
    source.direction( SOURCE_DIRECTION_INPUT);

    model_->removeSource(source);

}

}
