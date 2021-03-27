import QtQuick 2.10
import QtQuick.Templates 2.1 as T
import QtQuick.Layouts 1.0

T.Button {
    id: control
    width: 250
    height: 150

    implicitWidth: Math.max(
                       buttonBackground ? buttonBackground.implicitWidth : 0,
                       textItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        buttonBackground ? buttonBackground.implicitHeight : 0,
                        textItem.implicitHeight + topPadding + bottomPadding)
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
        anchors.fill: parent
    }

    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: 8
        anchors.leftMargin: 8
        anchors.bottomMargin: 9
        anchors.rightMargin: 8

        RowLayout {}

        Text {
            id: text1
            text: content
            font.pixelSize: 12
            Layout.preferredHeight: 114
            Layout.preferredWidth: 234
        }

        Text {
            id: dateMsg
            text: date
            font.pixelSize: 12
            Layout.preferredHeight: 14
            Layout.preferredWidth: 105
        }

        Text {
            id: username
            text: from
            font.pixelSize: 12
            Layout.preferredHeight: 14
            Layout.preferredWidth: 106
        }
    }

    states: [
        State {
            name: "normal"
            when: !control.down
            PropertyChanges {
                target: buttonBackground
                visible: false
                color: "#d2d2d2"
            }

            PropertyChanges {
                target: username
                color: "#00ff00"
            }

            PropertyChanges {
                target: date
                color: "#6b7376"
            }

            PropertyChanges {
                target: text1
                color: "#6b7376"
            }
        },
        State {
            name: "down"
            when: control.down

            PropertyChanges {
                target: username
                color: "#00ff00"
            }

            PropertyChanges {
                target: date
                color: "#6b7376"
            }

            PropertyChanges {
                target: text1
                color: "#6b7376"
            }

            PropertyChanges {
                target: buttonBackground
                visible: true
                color: "#374040"
                radius: 6
            }
        }
    ]
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.3300000429153442}
}
##^##*/

