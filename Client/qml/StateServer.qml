import QtQuick 2.0

Item {
    id: item1
    width: 200
    height: 150

    TextEdit {
        id: textEdit
        x: 20
        y: 21
        width: 160
        height: 39
        text: qsTr("None
")
        font.pixelSize: 12
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    states: [
        State {
            name: "connected"

            PropertyChanges {
                target: textEdit
                text: "Connected\n"
            }
        },
        State {
            name: "none"

            PropertyChanges {
                target: textEdit
                text: qsTr("None")
            }
        }
    ]

}
