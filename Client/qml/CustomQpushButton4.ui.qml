import QtQuick 2.10
import QtQuick.Templates 2.1 as T

T.Button {
    id: control

    implicitWidth: Math.max(buttonBackground ? buttonBackground.implicitWidth : 0,
                            textItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(buttonBackground ? buttonBackground.implicitHeight : 0,
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
                color: "#d2d2d2"
            }
        },
        State {
            name: "down"
            when: control.down
            PropertyChanges {
                target: textItem
                color: "black"
            }
        }
    ]
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
