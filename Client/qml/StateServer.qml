import QtQuick 2.0

Item {
    id: item1
    width: 200
    height: 150

    TextEdit {
        objectName: "state"
        id: textEdit
        x: 20
        y: 21
        width: 160
        height: 39
        text: qsTr("You're no connected - Please connect to a server before trying to log in or register");
        font.pixelSize: 12
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

}
