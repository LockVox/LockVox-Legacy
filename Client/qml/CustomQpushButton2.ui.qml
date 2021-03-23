import QtQuick 2.10
import QtQuick.Templates 2.1 as T

T.Button {
    id: control
    width: 40
    height: 40

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
        radius: 20
        border.width: 1
    }

    contentItem: textItem
    Text {
        id: textItem

        opacity: enabled ? 1.0 : 0.3
        color: "gray"
        text: "<"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 21
    }

    states: [
        State {
            name: "normal"
            when: !control.down
            PropertyChanges {
                target: buttonBackground
                color: "#ffffff"
                border.color: "#1393ac"
            }

            PropertyChanges {
                target: control
                width: 40
                height: 40
            }

            PropertyChanges {
                target: textItem
                color: "#000000"
            }
        },
        State {
            name: "down"
            when: control.down
            PropertyChanges {
                target: textItem
                color: "#1393ac"
            }

            PropertyChanges {
                target: control
                width: 40
                height: 40
            }

            PropertyChanges {
                target: buttonBackground
                color: "#ffffff"
                border.color: "#1393ac"
            }
        }
    ]
}

/*##^##
Designer {
    D{i:0;formeditorZoom:6}
}
##^##*/
