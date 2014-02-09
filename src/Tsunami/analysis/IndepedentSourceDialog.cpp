#include "IndepedentSourceDialog.h"
#include "ui_IndepedentSourceDialog.h"
namespace tsunami{
IndepedentSourceDialog::IndepedentSourceDialog(Source *source,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IndepedentSourceDialog),
    source_(source)
{
    ui->setupUi(this);

    openSource();

    connect(ui->sourceConstRadioButton,SIGNAL(toggled(bool)),
            this,SLOT(toggledSourceType(bool)));
    connect(ui->sourcePulseRadioButton,SIGNAL(toggled(bool)),
            this,SLOT(toggledSourceType(bool)));
    connect(ui->sourceSinRadioButton,SIGNAL(toggled(bool)),
            this,SLOT(toggledSourceType(bool)));
    connect(ui->sourceExpRadioButton,SIGNAL(toggled(bool)),
            this,SLOT(toggledSourceType(bool)));

    connect(ui->saveButton,SIGNAL(clicked()),
            this,SLOT(clickedSaveButton()));
}


IndepedentSourceDialog::~IndepedentSourceDialog()
{
    delete ui;
}

void IndepedentSourceDialog::openSource() {
    if(source_->mode() == SOURCE_MODE_CURRENT){
        sourceCurrent();
    }else{
        sourceVoltage();
    }


    switch(source_->type()){
    case SOURCE_TYPE_CONST: showConst();break;
    case SOURCE_TYPE_PULSE: showPulse(); break;
    case SOURCE_TYPE_EXP: showExp(); break;
    case SOURCE_TYPE_SIN: showSin(); break;
    }
}

void IndepedentSourceDialog::sourceVoltage() {
    ui->sourcePulseRadioButton->setText(tr("PULSE(V1 V2 ....)"));
    ui->sourceExpRadioButton->setText(tr("EXP(V1 V2 ....)"));
    ui->sourceSinRadioButton->setText(tr("SIN(V1 V2 ....)"));

    setWindowTitle(tr("Voltage source"));
}

void IndepedentSourceDialog::sourceCurrent() {
    ui->sourcePulseRadioButton->setText(tr("PULSE(I1 I2 ....)"));
    ui->sourceExpRadioButton->setText(tr("EXP(I1 I2 ....)"));
    ui->sourceSinRadioButton->setText(tr("SIN(I1 I2 ....)"));

    setWindowTitle(tr("Current source"));
}

void IndepedentSourceDialog::showConst() {
    if(!ui->sourceConstRadioButton->isChecked()){
        ui->sourceConstRadioButton->setChecked(true);
    }
    ui->acGroup->setEnabled(false);
    ui->dcGroup->setEnabled(true);


    // Hide labels
    hiddenAllParameters();


    if(source_->hasConfiguration("dc")){
        ui->dcValueLineEdit->setText( source_->configuration("dc").toString()  );
    }
}

void IndepedentSourceDialog::showPulse() {
    if(!ui->sourcePulseRadioButton->isChecked()){
        ui->sourcePulseRadioButton->setChecked(true);
    }
    ui->dcGroup->setEnabled(false);
    ui->acGroup->setEnabled(true);


    hiddenAllParameters(false);

    showAcSource();
    parameterLabel(3)->setText( tr("Time delay") );
    parameterLineEdit(3)->setText( source_->configuration("tdelay").toString() );

    parameterLabel(4)->setText( tr("Time rise") );
    parameterLineEdit(4)->setText( source_->configuration("trise").toString() );

    parameterLabel(5)->setText( tr("Time fall") );
    parameterLineEdit(5)->setText( source_->configuration("tfall").toString() );

    parameterLabel(6)->setText( tr("Time on") );
    parameterLineEdit(6)->setText( source_->configuration("ton").toString() );

    parameterLabel(7)->setText( tr("Period") );
    parameterLineEdit(7)->setText( source_->configuration("period").toString() );

    parameterLabel(8)->setText( tr("Cycles") );
    parameterLineEdit(8)->setText( source_->configuration("cycles").toString() );



}

void IndepedentSourceDialog::showExp() {
    if(!ui->sourceExpRadioButton->isChecked()){
        ui->sourceExpRadioButton->setChecked(true);
    }
    ui->dcGroup->setEnabled(false);
    ui->acGroup->setEnabled(true);


    hiddenAllParameters(false);
    hiddenParameter(8);
    hiddenParameter(7);

    showAcSource();

    parameterLabel(3)->setText( tr("Rise delay") );
    parameterLineEdit(3)->setText( source_->configuration("rdelay").toString() );

    parameterLabel(4)->setText( tr("Rise tau") );
    parameterLineEdit(4)->setText( source_->configuration("rtau").toString() );

    parameterLabel(5)->setText( tr("Fall delay") );
    parameterLineEdit(5)->setText( source_->configuration("fdelay").toString() );

    parameterLabel(6)->setText( tr("Fall tau") );
    parameterLineEdit(6)->setText( source_->configuration("ftau").toString() );

}

void IndepedentSourceDialog::showSin() {
    ui->dcGroup->setEnabled(false);
    ui->acGroup->setEnabled(true);
    if(!ui->sourceSinRadioButton->isChecked()){
        ui->sourceSinRadioButton->setChecked(true);
    }

    hiddenAllParameters(false);
    hiddenParameter(8);
    hiddenParameter(7);
    hiddenParameter(6);

    showAcSource();

    parameterLabel(3)->setText( tr("Frequency, Hz") );
    parameterLineEdit(3)->setText( source_->configuration("freq").toString() );
    parameterLabel(4)->setText( tr("Time delay") );
    parameterLineEdit(4)->setText( source_->configuration("delay").toString() );
    parameterLabel(5)->setText( tr("Theta") );
    parameterLineEdit(5)->setText( source_->configuration("theta").toString() );

}

void IndepedentSourceDialog::showAcSource() {
    ui->acAmplitudeLineEdit->setText( source_->configuration("acmag").toString() );
    ui->acPhaseLineEdit->setText( source_->configuration("acphase").toString());

    if(source_->mode() == SOURCE_MODE_CURRENT){
        parameterLabel(1)->setText( tr("I1, A") );
        parameterLineEdit(1)->setText( source_->configuration("i1").toString() );
        parameterLabel(2)->setText( tr("I1, A") );
        parameterLineEdit(2)->setText( source_->configuration("i2").toString() );
    }else{
        parameterLabel(1)->setText( tr("V1, V") );
        parameterLineEdit(1)->setText( source_->configuration("v1").toString() );
        parameterLabel(2)->setText( tr("V2, V") );
        parameterLineEdit(2)->setText( source_->configuration("v2").toString() );
    }
}

void IndepedentSourceDialog::hiddenAllParameters(bool hide) {
    for(int i=1; i <= 8; ++i){
        hiddenParameter(i,hide);
    }
}

void IndepedentSourceDialog::hiddenParameter( int index, bool hide){
    parameterLabel(index)->setHidden(hide);
    parameterLineEdit(index)->setHidden(hide);
}

QLabel *IndepedentSourceDialog::parameterLabel(int index) {

    switch(index){
    case 8:
        return ui->parameter8Label;
    case 7:
        return ui->parameter7Label;
    case 6:
        return ui->parameter6Label;
    case 5:
        return ui->parameter5Label;
    case 4:
        return ui->parameter4Label;
    case 3:
        return ui->parameter3Label;
    case 2:
        return ui->parameter2Label;
    case 1:
        return ui->parameter1Label;
    default:
        break;
    }
    Q_ASSERT(false);
    return NULL;
}

QLineEdit *IndepedentSourceDialog::parameterLineEdit(int index) {
    switch(index){
    case 8:
        return ui->parameter8LineEdit;
    case 7:
        return ui->parameter7LineEdit;
    case 6:
        return ui->parameter6LineEdit;
    case 5:
        return ui->parameter5LineEdit;
    case 4:
        return ui->parameter4LineEdit;
    case 3:
        return ui->parameter3LineEdit;
    case 2:
        return ui->parameter2LineEdit;
    case 1:
        return ui->parameter1LineEdit;
    default:
        break;
    }
    Q_ASSERT(false);
    return NULL;
}

SourceType IndepedentSourceDialog::sourceType() {
    if(ui->sourceConstRadioButton->isChecked()){
        return SOURCE_TYPE_CONST;
    }else if(ui->sourcePulseRadioButton->isChecked()){
        return SOURCE_TYPE_PULSE;
    }else if(ui->sourceExpRadioButton->isChecked()){
        return SOURCE_TYPE_EXP;
    }else if(ui->sourceSinRadioButton->isChecked()){
        return SOURCE_TYPE_SIN;
    }

    Q_ASSERT(false);
    return SOURCE_TYPE_GND;
}

void IndepedentSourceDialog::clickedSaveButton() {
    source_->cleanConfiguration();

    if(source_->type() == SOURCE_TYPE_CONST){
        source_->addConfiguration( "dc", ui->dcValueLineEdit->text() );
    }else{
        source_->addConfiguration("acmag", ui->acAmplitudeLineEdit->text());
        source_->addConfiguration("acphase",ui->acPhaseLineEdit->text());
        if(source_->mode() == SOURCE_MODE_CURRENT){
            source_->addConfiguration("i1",parameterLineEdit(1)->text());
            source_->addConfiguration("i2",parameterLineEdit(2)->text());
        }else{
            source_->addConfiguration("v1",parameterLineEdit(1)->text());
            source_->addConfiguration("v2",parameterLineEdit(2)->text());
        }

        if(source_->type() == SOURCE_TYPE_PULSE){
            source_->addConfiguration("tdelay",parameterLineEdit(3)->text());
            source_->addConfiguration("trise",parameterLineEdit(4)->text());
            source_->addConfiguration("tfall",parameterLineEdit(5)->text());
            source_->addConfiguration("ton",parameterLineEdit(6)->text());
            source_->addConfiguration("period",parameterLineEdit(7)->text());
            source_->addConfiguration("cycles",parameterLineEdit(8)->text());
        }else if(source_->type() == SOURCE_TYPE_EXP){
            source_->addConfiguration("rdelay",parameterLineEdit(3)->text());
            source_->addConfiguration("rtau",parameterLineEdit(4)->text());
            source_->addConfiguration("fdelay",parameterLineEdit(5)->text());
            source_->addConfiguration("ftau",parameterLineEdit(6)->text());
        }else if(source_->type() == SOURCE_TYPE_SIN){
            source_->addConfiguration("freq",parameterLineEdit(3)->text());
            source_->addConfiguration("delay",parameterLineEdit(4)->text());
            source_->addConfiguration("theta",parameterLineEdit(5)->text());
        }

    }
    accept();
}

void IndepedentSourceDialog::toggledSourceType(bool toggled) {
    if(!toggled){
        return;
    }

    SourceType type = sourceType();

    switch(type){
    case SOURCE_TYPE_CONST: showConst(); break;
    case SOURCE_TYPE_PULSE: showPulse(); break;
    case SOURCE_TYPE_EXP: showExp(); break;
    case SOURCE_TYPE_SIN: showSin(); break;
    }

    source_->type(type);

}


}
