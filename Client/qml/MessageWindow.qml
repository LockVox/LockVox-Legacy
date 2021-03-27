import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.4

Item {

    objectName: "message_window"

    signal sendMessage(string text)

    width: 300

    Rectangle {
        id: background
        color: "#35373a"
        anchors.fill: parent
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0

        CustomQpushButton {


            anchors.bottom: parent.bottom
            anchors.rightMargin: 14
            anchors.bottomMargin: 25
            anchors.leftMargin: 6
            anchors.left:messageText.right
            anchors.right:parent.right

            id: sendMessageButton
            x: 254
            y: 428
            height: 27
            text: "Send"
            Layout.preferredHeight: 33
            Layout.preferredWidth: 31
        }

        TextField {


            placeholderTextColor:"#6f797f"
            //Text.color: "white"

            background: Rectangle {
                color:"#41494b"
                radius:5
                border.width: 0
            }
            anchors.bottom: parent.bottom
            hoverEnabled: false
            font.hintingPreference: Font.PreferDefaultHinting
            anchors.rightMargin: 52
            anchors.bottomMargin: 20
            anchors.leftMargin: 21
            anchors.left: parent.left
            anchors.right:parent.right

            id: messageText
            x: 10
            y: 428
            height: 32
            Layout.preferredHeight: 25
            Layout.preferredWidth: 238
            placeholderText: qsTr("Send a message")

            inputMethodHints: Qt.ImhNoPredictiveText

            function _onEnterPressed(event)
            {
                if ((event.modifiers & Qt.ControlModifier))
                {
                    console.log("YAHSDHHD")
                }
                else
                {
                    event.accepted = false;
                }
            }

            Keys.onReturnPressed: { _onEnterPressed(event) }
            Keys.onEnterPressed: { _onEnterPressed(event) }




        }

        Text {
            id: channelName
            x: 10
            y: 8
            color: "#676d76"
            text: qsTr("channel")
            font.pixelSize: 12
            anchors.leftMargin: 10
            anchors.topMargin: 8
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
        onClicked: {sendMessage(messageText.text)
            messageText.clear()

        target:messageText
            Key.onEnterPressed

        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.100000023841858}
}
##^##*/
