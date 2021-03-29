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
        height: 18
        color: "#33a5e5"
        text : qsTr("Username")
        anchors.bottom:parent.bottom
        anchors.top:parent.top
        anchors.left:parent.left
        anchors.right:parent.right

        font.pixelSize: 12
        horizontalAlignment: Text.AlignHCenter
        font.bold: true
        anchors.rightMargin: -10
        anchors.bottomMargin: 63
        anchors.leftMargin: 71
        anchors.topMargin: 8
    }

    BorderImage {
        id: userPicture
        x: 0
        y: 0
        width: 71
        height: 72
        source: "qrc:/qtquickplugin/images/template_image.png"
    }

    Image {
        id: lock_vox_logo_miniature1
        x: 8
        y: 0
        width: 72
        height: 72
        source: "image://imageprovider"+username.text
        fillMode: Image.PreserveAspectFit
    }


}


/*##^##
Designer {
    D{i:0;formeditorZoom:3}
}
##^##*/
