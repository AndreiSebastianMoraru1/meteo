#ifndef WEATHERCLIENT_H
#define WEATHERCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>

class WeatherClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString temperature READ temperature NOTIFY temperatureChanged)
    Q_PROPERTY(QString time READ time NOTIFY timeChanged)
    Q_PROPERTY(QVariantList  times READ times NOTIFY timesChanged)
    Q_PROPERTY(QVariantList  temps READ temps NOTIFY tempsChanged)

public:
    explicit WeatherClient(QObject *parent = nullptr);
    Q_INVOKABLE void getWeather(double lat, double lon);

    QString temperature() const { return m_temperature; }
    QString time() const { return m_time; }
    QVariantList  times() const { return m_times; }
    QVariantList  temps() const { return m_temps; }

signals:
    void temperatureChanged();
    void timeChanged();
    void tempsChanged();
    void timesChanged();

private slots:
    void onReplyFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager m_manager;
    QString m_temperature;
    QString m_time;
    QVariantList  m_temps;
    QVariantList  m_times;
};

#endif // WEATHERCLIENT_H
