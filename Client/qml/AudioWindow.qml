import QtQuick 2.0
import QtQuick.Controls 2.15

Item {
    width: 179
    height: 124
    Rectangle {
        id: background
        x: 0
        y: 0
        width: 179
        height: 124
        color: "#1b1f21"

        Text {
            id: channelName
            x: 6
            y: 2
            width: 68
            height: 14
            color: "#566369"
            text: qsTr("#Channel")
            font.pixelSize: 12
        }

        ScrollView {
            id: scrollView
            x: 6
            y: 22
            width: 200
            height: 102

            Rectangle {
                id: rectangle
                x: 0
                y: -5
                width: 169
                height: 1
                color: "#2e3538"
            }
        }
    }

}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.75}
}
##^##*/
