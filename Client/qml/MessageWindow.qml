import QtQuick 2.0
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.4

Item {

    objectName: "message_window"

    signal sendMessage(string text)

    width: 300

    Rectangle {
        id: background
        color: "#313539"
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

            color:"white"
            background: Rectangle {
                color:"#1f2325"
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

            Keys.onPressed: {

                if(event.key == Qt.Key_Return){
                            sendMessage(messageText.text)
                            messageText.clear()


                }

            }




        }

        Text {
            id: channelName
            x: 10
            y: 8
            color: "#676d76"
            text: qsTr("channel")
            objectName: "channel_name"
            font.pixelSize: 14
            font.underline: true
            anchors.leftMargin: 8
            anchors.topMargin: 47
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

        Text {
            id: channel_members
            objectName: "channel_members"
            x: 220
            y: 16
            color: "#33a5e5"
            text: pseudo
            anchors.right: parent.right
            anchors.top: parent.top
            font.pixelSize: 14
            minimumPixelSize: 8
            anchors.rightMargin: 26

            anchors.leftMargin: 232
            font.underline: false
            Layout.preferredHeight: 14
            anchors.topMargin: 47
            Layout.preferredWidth: 140
            font.bold: false
        }




    }

    Connections {
        target: sendMessageButton
        onClicked: {sendMessage(messageText.text)
            messageText.clear()



        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.659999966621399}
}
##^##*/
