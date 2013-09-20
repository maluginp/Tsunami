#ifndef ANALYSISITEMS_H
#define ANALYSISITEMS_H
#include <QString>

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
        CURRENT
    };

    const ModeType& mode() const { return mode_; }
    const QString& node() const { return node_; }

    void setMode( const ModeType& mode );
    void setNode( const QString& node );

    QString name() const;

    void parseJson( const QString& json ) = 0;
    AnalysisItemType getItemType() const = 0;
private:
    ModeType mode_;
    QString node_;
};

class AnalysisItemConst : public IAnalysisItem {
public:

    AnalysisItemType getType() const{
        return ANALYSIS_ITEM_CONST;
    }

    void parseJson( const QString& json );

    const double& constant() const { return constant_; }
    void setConstant( const double& constant );
private:
    double constant_;
};

class AnalysisItemSweep : public IAnalysisItem {
    AnalysisItemType getType() const {
        return ANALYSIS_ITEM_SWEEP;
    }
    void parseJson(const QString &json);
private:
    int number_;
    QString method_;
    double start_;
    double stop_;
    double step_;
};

#endif // ANALYSISITEMS_H
