import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0

Item {
    width: 300

    Rectangle {
        id: background
        color: "#35373a"
        anchors.fill: parent

        ColumnLayout {
            x: 8
            y: 8

            Text {
                id: channelName
                color: "#676d76"
                text: qsTr("channel")
                font.pixelSize: 12
                Layout.preferredHeight: 14
                Layout.preferredWidth: 140
                font.bold: true
            }

            ScrollView {
                id: scrollView
                Layout.preferredHeight: 379
                Layout.preferredWidth: 284
            }

            RowLayout {

                TextField {
                    id: messageText
                    Layout.preferredHeight: 25
                    Layout.preferredWidth: 238
                    placeholderText: qsTr("Send a message")
                }

                CustomQpushButton {
                    id: sendMessageButton
                    text: "Send"
                    Layout.preferredHeight: 33
                    Layout.preferredWidth: 31
                }
            }
        }
    }

}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.8999999761581421}D{i:1}
}
##^##*/
