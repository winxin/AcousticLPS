#ifndef DATAMODEL_H
#define DATAMODEL_H
#include "qbeacon.h"

#include <QMap>
#include <QObject>
#include <model/model.pb.h>
class QEnvironement : public QObject
{
    Q_OBJECT
public:
    static QEnvironement* instance(QObject *parent = Q_NULLPTR);
    inline QString name() const { return QString::fromStdString(_environement->name()); }
    inline QString latitude() const { return QString::fromStdString(_environement->latitude()); }
    inline QString longitude() const { return QString::fromStdString(_environement->longitud()); }
    inline int beaconsCount() const { return _environement->beacons_size(); }

    QBeacon addBeacon();
    QBeacon beacon(int index);
private:
    explicit QEnvironement(QObject *parent = 0);
    ~QEnvironement();
    static QEnvironement*   _instance;
    Environement*           _environement{new Environement};
    QMap<QUuid, QBeacon>    _wrappedBeacons;
};

#endif // DATAMODEL_H