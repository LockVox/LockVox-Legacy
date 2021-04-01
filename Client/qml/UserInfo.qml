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

        font.pixelSize: 15
        horizontalAlignment: Text.AlignHCenter
        font.family: "Tahoma"
        font.strikeout: false
        minimumPointSize: 14
        minimumPixelSize: 14
        font.bold: true
        anchors.rightMargin: -6
        anchors.bottomMargin: 8
        anchors.leftMargin: 68
        anchors.topMargin: 62

        onTextChanged: {
            userPicture.source = ""
            userPicture.source = "image://ImageProvider/" + username.text
        }
    }

    Image {
        id: userPicture
        x: 0
        y: 0
        width: 71
        height: 100
        fillMode: Image.PreserveAspectFit
        source: "image://ImageProvider/"+username.text
    }

    Text {
        id: description
        objectName: "description"
        x: 71
        y: 13
        width: 121
        height: 18
        visible: false
        color: "#687075"
        text: qsTr("Description")
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        font.pixelSize: 11
        horizontalAlignment: Text.AlignLeft
        wrapMode: Text.Wrap
        font.italic: false
        anchors.topMargin: 120
        anchors.leftMargin: 89
        anchors.rightMargin: 15

        anchors.bottomMargin: -62
        font.bold: false
    }


}


/*##^##
Designer {
    D{i:0;formeditorZoom:3}
}
##^##*/
