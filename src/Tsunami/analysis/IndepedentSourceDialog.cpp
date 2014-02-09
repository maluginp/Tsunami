#include "IndepedentSourceDialog.h"
#include "ui_IndepedentSourceDialog.h"

IndepedentSourceDialog::IndepedentSourceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IndepedentSourceDialog)
{
    ui->setupUi(this);


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

void IndepedentSourceDialog::sourceVoltage() {
    ui->sourcePulseRadioButton->setText(tr("PULSE(V1 V2 ....)"));
    ui->sourceExpRadioButton->setText(tr("EXP(V1 V2 ....)"));
    ui->sourceSinRadioButton->setText(tr("SIN(V1 V2 ....)"));

}

void IndepedentSourceDialog::showConst() {
    ui->acGroup->setEnabled(false);
    ui->dcGroup->setEnabled(true);


    // Hide labels
    hiddenAllParameters();

}

void IndepedentSourceDialog::showPulse() {
    ui->dcGroup->setEnabled(false);
    ui->acGroup->setEnabled(true);


    hiddenAllParameters(false);

    if(type_ == "current"){
        parameterLabel(1)->setText( tr("I1, A") );
        parameterLabel(2)->setText( tr("I1, A") );
    }else{
        parameterLabel(1)->setText( tr("V1, V") );
        parameterLabel(2)->setText( tr("V2, V") );
    }
    parameterLabel(3)->setText( tr("Time delay") );
    parameterLabel(4)->setText( tr("Time rise") );
    parameterLabel(5)->setText( tr("Time fall") );
    parameterLabel(6)->setText( tr("Time on") );
    parameterLabel(7)->setText( tr("Period") );
    parameterLabel(8)->setText( tr("Cycles") );


}

void IndepedentSourceDialog::showExp() {
    ui->dcGroup->setEnabled(false);
    ui->acGroup->setEnabled(true);


    hiddenAllParameters(false);
    hiddenParameter(8);
    hiddenParameter(7);

    if(type_ == "current"){
        parameterLabel(1)->setText( tr("I1, A") );
        parameterLabel(2)->setText( tr("I1, A") );
    }else{
        parameterLabel(1)->setText( tr("V1, V") );
        parameterLabel(2)->setText( tr("V2, V") );
    }
    parameterLabel(3)->setText( tr("Rise delay") );
    parameterLabel(4)->setText( tr("Rise tau") );
    parameterLabel(5)->setText( tr("Fall delay") );
    parameterLabel(6)->setText( tr("Fall tau") );

}

void IndepedentSourceDialog::showSin() {
    ui->dcGroup->setEnabled(false);
    ui->acGroup->setEnabled(true);


    hiddenAllParameters(false);
    hiddenParameter(8);
    hiddenParameter(7);
    hiddenParameter(6);

    if(type_ == "current"){
        parameterLabel(1)->setText( tr("I1, A") );
        parameterLabel(2)->setText( tr("I1, A") );
    }else{
        parameterLabel(1)->setText( tr("V1, V") );
        parameterLabel(2)->setText( tr("V2, V") );
    }
    parameterLabel(3)->setText( tr("Frequency, Hz") );
    parameterLabel(4)->setText( tr("Time delay") );
    parameterLabel(5)->setText( tr("Theta") );

}

void IndepedentSourceDialog::hiddenAllParameters(bool hide) {
    for(int i=1; i <= 8; ++i){
        hiddenParameter(i,hide);
    }
}

void IndepedentSourceDialog::hiddenParameter( int index, bool hide){
    parameterLabel(index)->setHidden(hide);
    parameterLabel(index)->setHidden(hide);
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

QString IndepedentSourceDialog::sourceType() {
    if(ui->sourceConstRadioButton->isChecked()){
        return "const";
    }else if(ui->sourcePulseRadioButton->isChecked()){
        return "pulse";
    }else if(ui->sourceExpRadioButton->isChecked()){
        return "exp";
    }else if(ui->sourceSinRadioButton->isChecked()){
        return "sin";
    }

    Q_ASSERT(false);
    return "";
}

void IndepedentSourceDialog::toggledSourceType(bool toggled) {
    if(!toggled){
        return;
    }

    QString type = sourceType();
    if(type=="const"){
        showConst();
    }else if(type == "pulse"){
        showPulse();
    }else if(type == "exp"){
        showExp();
    }else if(type == "sin"){
        showSin();
    }

}


