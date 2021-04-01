import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Extras 1.4

Item {

    id: channel
    x: 0
    y: 0
    width: parent.width-5
    height: 35

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
            //channel_activity.visible=true
        }
        onExited: {
            rectangle.visible=false
            //channel_activity.visible=false
        }

        onClicked: {
            listChannel.currentIndexChanged(index)
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
        radius:5
    }

    Text {
        objectName: "title"
        id: title
        x: 8
        y: 2
        width: 188
        height: 32
        color: "#6f797f"
        text: model.name
        font.pixelSize: 13
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        lineHeightMode: Text.ProportionalHeight
        wrapMode: Text.Wrap
        font.bold: true
    }


    Text {
        id: channel_activity
        x: 62
        y: 25
        width: 81
        height: 25
        visible: false
        color: "#11ff3a"
        text:nbUsers+"/"+maxUsers+" persons connected"
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
    D{i:0;formeditorZoom:3}
}
##^##*/
