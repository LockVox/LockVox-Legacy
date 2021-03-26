import QtQuick 2.10
import QtQuick.Templates 2.1 as T

//import Qt.SafeRenderer 1.1
T.Button {
    id: control
    width: 400
    height: 50

    implicitWidth: Math.max(
                       buttonBackground ? buttonBackground.implicitWidth : 0,
                       channelText.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        buttonBackground ? buttonBackground.implicitHeight : 0,
                        channelText.implicitHeight + topPadding + bottomPadding)
    leftPadding: 4
    rightPadding: 4

    text: "My Button"

    background: buttonBackground
    Rectangle {
        id: buttonBackground
        color: "#d4d4d4"
        implicitWidth: 100
        implicitHeight: 40
        opacity: enabled ? 1 : 0.3
        radius: 2
    }

    contentItem: channelText
    Rectangle {
        id: channelPic
        x: 4
        y: 6
        width: 39
        height: 39
        color: "#ffffff"
        radius: 6
        border.width: 1
    }

    Text {
        id: channelText
        x: 58
        y: -9
        width: 342
        height: 54

        opacity: enabled ? 1.0 : 0.3
        color: "gray"
        text: "Channel_name"
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }

    Text {
        id: channelText2
        x: 58
        y: 14
        width: 338
        height: 42
        opacity: enabled ? 1.0 : 0.3
        color: "#808080"
        text: "0 person connected"
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }

    states: [
        State {
            name: "normal"
            when: !control.down
            PropertyChanges {
                target: buttonBackground
                color: "#00d2d2d2"
            }

            PropertyChanges {
                target: channelText2
                visible: false
            }

            PropertyChanges {
                target: channelText
                color: "#33a5e5"
                font.bold: true
            }
        },
        State {
            name: "down"
            when: control.down
            PropertyChanges {
                target: channelText
                color: "#2e3538"
                font.underline: false
                font.italic: false
                font.bold: true
            }

            PropertyChanges {
                target: buttonBackground
                color: "#33a5e5"
            }

            PropertyChanges {
                target: channelText2
                visible: true
                color: "#00ff00"
                font.bold: true
            }
        }
    ]
}



