import QtQuick 2.0
import QtQuick.Controls.Universal 2.0
import Qt.labs.qmlmodels 1.0
import QtQuick.Controls 2.15
import QtQuick.Dialogs.qml 1.0

Item {
    objectName: "connect_server"
    id: server
    signal connect_server(string ip)
    width: 150
    height: 150

    CustomQpushButton {
        id: connectButton
        x: 24
        y: 91
        width: 91
        height: 32
        text: "Connect"
        Connections {
            target: connectButton
            onClicked: server.connect_server(ipAdress.text)
        }
    }
    
    TextField {
        id: ipAdress
        placeholderText: "127.0.0.1"
        x: 12
        y: 29
        width: 115
        height: 29
        hoverEnabled: false
        //placeholderText: qsTr("IP Adress")
    }
}



