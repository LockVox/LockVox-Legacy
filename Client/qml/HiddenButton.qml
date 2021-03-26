import QtQuick 2.10
import QtQuick.Templates 2.1 as T

Rectangle {
    x: 200
    y: 300
    width: 100
    height: 30
    color: "red"

    MouseArea {
        id: ma
        x: 50
        y: 2
        width: 50
        height: 25
        hoverEnabled: true
        property bool hovered: false
        onEntered: hovered = true
        onExited: hovered = false

        /*Rectangle {
            anchors.fill: parent
            color: "blue"
        }//*/
    }

    T.Button {
        anchors.fill: ma
        visible: hovered || ma.hovered
        width: 50
        height: 25
        text: "clic"
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.75}
}
##^##*/
