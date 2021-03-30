import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Extras 1.4

Item {

    id: channel
    x: 0
    y: 0
    width: 640
    height: 50


    clip:true
    MouseArea
    {

        hoverEnabled: true
        //containsMouse: true
        id: mouse
        width: rectangle.width
        height: rectangle.height
        x:rectangle.x
        y:rectangle.y

        visible: true
        anchors.fill: parent
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0

        onEntered: username.color="#f46f68"

        onExited: username.color="#33a5e5"

        onClicked: {
            listClient.currentIndexChanged(index)
            listClient.currentIndex = index
        }
    }

    Text {
        objectName: "username"
        id: username
        x: 56
        y: 18
        width: 59
        height: 15
        color: "#33a5e5"
        text: pseudo
        font.pixelSize: 12
        horizontalAlignment: Text.AlignHCenter
        font.bold: true
    }

    StatusIndicator {
        id: statusIndicator
        x: 121
        y: 15
        width: 18
        height: 20
        color: "#52f34c"
        active: isOnline
    }

    Rectangle {
        id: rectangle
        x: 8
        y: 12
        width: 33
        height: 30
        color: "#ffffff"
        radius: 7
    }


    Image{
        objectName: "img"
        id: img
        x: 4
        y: 4
        width: 42
        height: 42
        source:"image://ImageProvider/"+pseudo
    }



}

/*##^##
Designer {
    D{i:0;formeditorZoom:3}
}
##^##*/
