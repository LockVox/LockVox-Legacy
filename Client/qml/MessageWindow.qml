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

        CustomQpushButton {


            anchors.bottom: parent.bottom
            anchors.rightMargin: 8
            anchors.bottomMargin: 17
            anchors.leftMargin: 6
            anchors.left:messageText.right
            anchors.right:parent.right

            id: sendMessageButton
            x: 254
            y: 432
            height: 32
            text: "Send"
            Layout.preferredHeight: 33
            Layout.preferredWidth: 31
        }

        TextField {

            anchors.bottom: parent.bottom
            hoverEnabled: false
            font.hintingPreference: Font.PreferDefaultHinting
            anchors.rightMargin: 52
            anchors.bottomMargin: 20
            anchors.leftMargin: 10
            anchors.left: parent.left
            anchors.right:parent.right

            id: messageText
            x: 10
            y: 435
            height: 25
            Layout.preferredHeight: 25
            Layout.preferredWidth: 238
            placeholderText: qsTr("Send a message")
        }

        Text {
            id: channelName
            x: 10
            y: 8
            color: "#676d76"
            text: qsTr("channel")
            font.pixelSize: 12
            Layout.preferredHeight: 14
            Layout.preferredWidth: 140
            font.bold: true

            anchors.left:parent.left
            anchors.top:parent.top
        }

        ColumnLayout {
            anchors.fill: parent


            spacing: 420

            RowLayout {
                Layout.bottomMargin: 0
                Layout.topMargin: 0
                Layout.margins: 3
                Layout.fillHeight: true
                Layout.fillWidth: true
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
    D{i:0;formeditorZoom:1.659999966621399}
}
##^##*/
