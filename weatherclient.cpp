#include "weatherclient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>

WeatherClient::WeatherClient(QObject *parent) : QObject(parent)
{
    connect(&m_manager, &QNetworkAccessManager::finished,
            this, &WeatherClient::onReplyFinished);
}

void WeatherClient::getWeather(double lat, double lon)
{
    qDebug()<<lat;
    qDebug()<<lon;
    QString urlStr = QString(
                         "https://api.open-meteo.com/v1/forecast?latitude=%1&longitude=%2&hourly=temperature_2m&current_weather=true")
                         .arg(lat, 0, 'f', 6)
                         .arg(lon, 0, 'f', 6);

    QUrl url(urlStr);                 // Creează obiect QUrl
    QNetworkRequest request(url);     // Creează obiect QNetworkRequest

    m_manager.get(request);           // Pasăm referința obiectului
}

void WeatherClient::onReplyFinished(QNetworkReply* reply)
{
    if(reply->error() != QNetworkReply::NoError){
        qWarning() << "Network error:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if(!doc.isObject()){
        qWarning() << "Invalid JSON response";
        reply->deleteLater();
        return;
    }

    QJsonObject obj = doc.object();
    if(obj.contains("current_weather")){
        QJsonObject current = obj["current_weather"].toObject();
        if(current.contains("temperature")){
            m_temperature = QString::number(current["temperature"].toDouble()) + " °C";
            emit temperatureChanged();
        }
        if(current.contains("time")){
            m_time = current["time"].toString();
            emit timeChanged();
        }
    }
    if(obj.contains("hourly"))
    {
        QJsonObject hourly = obj["hourly"].toObject();

        if(hourly.contains("time") && hourly.contains("temperature_2m")) {
            QJsonArray timesArray = hourly["time"].toArray();
            QJsonArray tempsArray = hourly["temperature_2m"].toArray();

            for(int i = 0; i < timesArray.size(); ++i) {
                QString t = timesArray[i].toString();
                double temp = tempsArray[i].toDouble();
                m_times.append(timesArray[i].toString());
                m_temps.append(tempsArray[i].toDouble());


                qDebug() << t << temp;
            }
            emit timesChanged();
            emit tempsChanged();
        }
    }

    reply->deleteLater();
}
