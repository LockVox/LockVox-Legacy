import QtQuick 2.0

Rectangle {
    width: 200
    height: 100
    border.color: "#cc0909"

    Text {
        objectName: "username"
        id: username
        x: 71
        y: 17
        width: 121
        height: 31
        text : qsTr("Username")

        font.pixelSize: 12
        horizontalAlignment: Text.AlignHCenter
    }

    BorderImage {
        id: userPicture
        x: 0
        y: 0
        width: 71
        height: 72
        source: "qrc:/qtquickplugin/images/template_image.png"
    }

    Text {
        id: status
        x: 71
        y: 35
        width: 121
        height: 31
        text: qsTr("Status")
        font.pixelSize: 12
        horizontalAlignment: Text.AlignHCenter
        objectName: "username"
    }
}


/*##^##
Designer {
    D{i:0;formeditorZoom:1.75}
}
##^##*/
