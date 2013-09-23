#ifndef ANALYSISITEMS_H
#define ANALYSISITEMS_H
#include <QString>
#include <QVariant>

enum AnalysisItemType{
    ANALYSIS_ITEM_NONE,
    ANALYSIS_ITEM_CONST,
    ANALYSIS_ITEM_SWEEP,
    ANALYSIS_ITEM_FUNC,
    ANALYSIS_ITEM_OUTPUT
};

class IAnalysisItem{
public:
    enum ModeType{
        VOLTAGE,
        CURRENT,
        UNKNOWN
    };

    const ModeType& mode() const { return mode_; }
    QVariant modeToVariant() const;
    const QString& node() const { return node_; }


    void setMode( const QString& mode);
    void setMode( const ModeType& mode );
    void setNode( const QString& node );

    QString name() const;
    virtual QString title() const = 0;


    virtual QVariantMap json() = 0;

    virtual void parseJson( const QString& json ) = 0;
    virtual AnalysisItemType getItemType() const {
        return ANALYSIS_ITEM_NONE;
    }
protected:
    ModeType mode_;
    QString node_;
};

class AnalysisItemConst : public IAnalysisItem {
public:
    AnalysisItemConst() {}
    AnalysisItemConst(const QString& node,const QString& mode,
                      const double& constant) {
        setNode( node );
        if(mode == "voltage"){
            mode_ = VOLTAGE;
        }else if(mode == "current"){
            mode_ = CURRENT;
        }
        setConstant( constant );
    }

    QString title() const{
        return QString("%1 CONST=%2").arg(name()).arg(constant());
    }

    AnalysisItemType getItemType() const{
        return ANALYSIS_ITEM_CONST;
    }

    void parseJson( const QString& json );
    QVariantMap json();
    const double& constant() const { return constant_; }
    void setConstant( const double& constant );
private:
    double constant_;
};

class AnalysisItemSweep : public IAnalysisItem {
public:
    AnalysisItemSweep(){}
    AnalysisItemSweep(const QString& node,const QString& mode, const int& number,
                      const QString& method, const double& start, const double& stop,
                      const double& step) :
       number_(number),method_(method),start_(start),stop_(stop),step_(step)
    {
        setNode( node );
        if(mode == "voltage"){
            mode_ = VOLTAGE;
        }else if(mode == "current"){
            mode_ = CURRENT;
        }

    }
    QString title() const{
        return QString("%1 %2").arg(name()).arg(method());
    }
    AnalysisItemType getItemType() const {
        return ANALYSIS_ITEM_SWEEP;
    }
    const int& number() const { return number_; }
    const QString& method() const { return method_; }
    const double& start() const { return start_; }
    const double& stop() const { return stop_; }
    const double& step() const { return step_; }

    void setNumber( const int& number );
    void setMethod( const QString& method );
    void setStart( const double& start );
    void setStop( const double& stop);
    void setStep( const double& step);

    QVariantMap json();
    void parseJson(const QString &json);
private:
    int number_;
    QString method_;
    double start_;
    double stop_;
    double step_;
};

class AnalysisItemOutput : public IAnalysisItem {
public:
    AnalysisItemOutput() {}
    AnalysisItemOutput( const QString& node,const QString& mode){
        setNode( node );
        if(mode == "voltage"){
            mode_ = VOLTAGE;
        }else if(mode == "current"){
            mode_ = CURRENT;
        }
    }
    QString title() const{
        return QString("%1 OUTPUT").arg(name());
    }

    AnalysisItemType getItemType() const {
        return ANALYSIS_ITEM_OUTPUT;
    }
    const double& value() const { return value_; }
    void setValue( const double& value );
    void parseJson(const QString &json);
    QVariantMap json();
private:
    double value_;

};

#endif // ANALYSISITEMS_H
