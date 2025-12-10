import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 400
    height: 500
    title: qsTr("Weather Example")

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 10

        TextField {
            id: latInput
            placeholderText: "Latitude"
            validator: DoubleValidator { bottom: -90; top: 90; decimals: 6 }
            width: 200
        }

        TextField {
            id: lonInput
            placeholderText: "Longitude"
            validator: DoubleValidator { bottom: -180; top: 180; decimals: 6 }
            width: 200
        }

        Button {
            text: "Get Weather"
            onClicked: {
                if(latInput.acceptableInput && lonInput.acceptableInput){
                    weatherClient.getWeather(parseFloat(latInput.text), parseFloat(lonInput.text))
                } else {
                    console.log("Invalid input")
                }
            }
        }

        Text { text: "Current Temperature: " + weatherClient.temperature; font.pixelSize: 18 }
        Text { text: "Current Time: " + weatherClient.time; font.pixelSize: 14 }

        ListModel { id: weatherModel }

        ListView {
            width: parent.width
            height: 500
            model: weatherModel
            delegate: Row {
                spacing: 20
                Text { text: model.time }
                Text { text: model.temp + " °C" }
            }

            Component.onCompleted: {
                // Populate model automatically when data changes
                weatherClient.timesChanged.connect(function() {
                    weatherModel.clear()
                    for(var i = 0; i < weatherClient.times.length; i++){
                        weatherModel.append({
                            "time": weatherClient.times[i],
                            "temp": weatherClient.temps[i]
                        })
                    }
                })
            }
        }
    }
}
