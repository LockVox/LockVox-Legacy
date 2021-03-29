import QtQuick 2.0

Item {
    id: item1
    width: 200
    height: 150

    Text {
        objectName: "state"
        id: textEdit
        x: 20
        y: 21
        width: 160
        height: 110
        text: qsTr("Please specify the @IP of the serve you want to connect")
        font.pixelSize: 12
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.Wrap
    }
}
