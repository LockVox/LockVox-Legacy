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
        x: 2
        y: 0
        visible: true
        anchors.fill: parent
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0

        onEntered: {

            rectangle.visible=true

        }
        onExited: {

            rectangle.visible=false

        }

        onClicked: {

            listChannel.currentIndexChanged(index)
            channel_activity.visible=true
            listChannel.currentIndex = index
        }



    }

    Rectangle {
        id: rectangle
        x: 2
        y: 0
        width: 638
        height: 50
        visible: false
        color: listChannel.isCurrentItem ? "red" : "#313539"
    }

    Text {
        objectName: "title"
        id: title
        x: 8
        y: 10
        width: 188
        height: 32
        color: "#6f797f"
        text: model.name
        font.pixelSize: 12
        horizontalAlignment: Text.AlignHCenter
    }

    Text {
        id: channel_activity
        x: 8
        y: 30
        width: 188
        height: 32
        visible: false
        color: "#11ff3a"
        text:"0 people connected"
        font.pixelSize: 12
        horizontalAlignment: Text.AlignHCenter
        objectName: "title"
    }




    states: [
        State {
            name: "selectedState"

            PropertyChanges {
                target: rectangle
                visible: true
                color: "#11ff3a"
            }
        }
    ]
}



/*##^##
Designer {
    D{i:0;formeditorZoom:1.75}
}
##^##*/
