#ifndef DATAMODEL_H
#define DATAMODEL_H
#include "qbeacon.h"

#include <util/singleton.h>
#include <model/model.pb.h>
#include <QMap>
#include <QObject>
#include <QSet>
class QEnvironement
{
public:
    QEnvironement();
    ~QEnvironement();
    inline QString name() const { return QString::fromStdString(_environement->name()); }
    inline Real length() const { return _environement->length(); }
    inline Real width() const { return _environement->width(); }
    inline Real height() const { return _environement->height(); }
    inline Size beaconsCount() const { return _environement->beacons_size(); }
    inline const QSet<QBeacon>& beacons() const { return _beacons; }
    bool    removeBeacon(const QBeacon& beaconAt);
    QBeacon addBeacon();
    QBeacon beaconAt(int index);

    void setLength(Real value);
    void setWidth(Real value);
    void setHeight(Real value);
    void clear();
    void init();
    bool loadEnvironementFromFile(const QString& filename = QString());
    bool saveEnvironementInFile(const QString& filename = QString()) const;

private:
    static constexpr Real DefaultWidth{10.0};
    static constexpr Real DefaultHeight{3.0};
    static constexpr Real DefaultLength{10.0};
    Environement*           _environement{new Environement};
    QSet<QBeacon>           _beacons;
    QMap<QUuid, QBeacon>    _wrappedBeacons;
};
#define Environement Singleton<QEnvironement>::instance()
#endif // DATAMODEL_H
