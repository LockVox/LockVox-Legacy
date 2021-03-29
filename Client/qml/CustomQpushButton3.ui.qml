import QtQuick 2.10
import QtQuick.Templates 2.1 as T

T.Button {
    id: control

    onHoveredChanged:hovered ? buttonBackground.color = "#1781bd":buttonBackground.color = "#33a5e5";
    highlighted: true
    implicitWidth: Math.max(
                       buttonBackground ? buttonBackground.implicitWidth : 0,
                       textItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        buttonBackground ? buttonBackground.implicitHeight : 0,
                        textItem.implicitHeight + topPadding + bottomPadding)
    leftPadding: 4
    rightPadding: 4

    text: "My Button"
    flat: true

    background: buttonBackground
    Rectangle {
        id: buttonBackground
        color: "#81039e"
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

    MouseArea {
        id: mouseArea
        x: 0
        y: 0
        visible: false
        anchors.fill: parent
        hoverEnabled: true
        onEntered: buttonBackground.color = "#1781bd"
        onExited: buttonBackground.color = "#33a5e5"
        acceptedButtons: Qt.LeftButton
        onClicked: control.down
    }

    states: [
        State {
            name: "normal"
            when: !control.down
            PropertyChanges {
                target: buttonBackground
                color: "#33a5e5"
                radius: 4
                border.width: 0
            }

            PropertyChanges {
                target: textItem
                color: "#ffffff"
            }

            PropertyChanges {
                target: mouseArea
                visible: false
            }

            PropertyChanges {
                target: control
                highlighted: false
                flat: false
            }
        },
        State {
            name: "down"
            when: control.down
            PropertyChanges {
                target: textItem
                color: "#f5f4f4"
            }

            PropertyChanges {
                target: buttonBackground
                color: "#f46f68"
                border.color: "#f5f4f4"
                border.width: 0
            }

            PropertyChanges {
                target: mouseArea
                visible: false
            }
        }
    ]
}

/*##^##
Designer {
    D{i:0;formeditorZoom:4}
}
##^##*/

