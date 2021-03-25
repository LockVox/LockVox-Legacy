import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0

Item {

    objectName: "message_window"

    signal sendMessage(string text)

    width: 300

    Rectangle {
        id: background
        color: "#35373a"
        anchors.fill: parent

        ColumnLayout {
            anchors.fill: parent


            spacing: 420

            Text {
                id: channelName
                color: "#676d76"
                text: qsTr("channel")
                font.pixelSize: 12
                Layout.preferredHeight: 14
                Layout.preferredWidth: 140
                font.bold: true
            }

            RowLayout {
                Layout.bottomMargin: 0
                Layout.topMargin: 0
                Layout.margins: 3
                Layout.fillHeight: true
                Layout.fillWidth: true

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

    Connections {
        target: sendMessageButton
        onClicked: sendMessage(messageText.text)
    }

}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.100000023841858}D{i:2}
}
##^##*/
