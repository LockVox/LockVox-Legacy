import QtQuick 2.10
import QtQuick.Templates 2.1 as T

T.Button {
    id: control
    width: 200
    height: 50

    implicitWidth: Math.max(
                       buttonBackground ? buttonBackground.implicitWidth : 0,
                       textItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        buttonBackground ? buttonBackground.implicitHeight : 0,
                        textItem.implicitHeight + topPadding + bottomPadding)
    leftPadding: 4
    rightPadding: 4
    onHoveredChanged: hovered ? buttonBackground.color
                                = "#f46f68" : buttonBackground.color = "#313539"

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

    contentItem: textItem
    Text {
        id: textItem
        text: control.text

        opacity: enabled ? 1.0 : 0.3
        color: "gray"

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    states: [
        State {
            name: "normal"
            when: !control.down
            PropertyChanges {
                target: buttonBackground
                color: "#313539"
                radius: 0
                border.width: 0
            }

            PropertyChanges {
                target: textItem
                x: 0
                y: 0
                width: 200
                height: 50
                color: "#ffffff"
                font.bold: false
            }
        },
        State {
            name: "down"
            when: control.down
            PropertyChanges {
                target: textItem
                x: 0
                y: 0
                width: 200
                height: 50
                color: "#ffffff"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 7
            }

            PropertyChanges {
                target: buttonBackground
                color: "#33a5e5"
                radius: 0
                border.width: 0
            }
        }
    ]
}

/*##^##
Designer {
    D{i:0;formeditorZoom:3}
}
##^##*/

