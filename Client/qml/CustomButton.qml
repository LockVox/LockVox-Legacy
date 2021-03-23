import QtQuick 2.0

Item {
    id: item1
    width: 100
    height: 38
    property alias text1: text1

    Rectangle {
        id: background
        color: "#45a5dc"
        radius: 6
        border.color: "#acc2c2"
        border.width: 0
        anchors.fill: parent

        Text {
            id: text1
            color: "#fdfdfd"
            text: qsTr("Text")
            anchors.fill: parent
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    Connections {
        target: item1
        onClicked: item1.state = "clickedState"
    }
    states: [
        State {
            name: "hoverState"

            PropertyChanges {
                target: background
                color: "#f68585"
                border.width: 0
            }

            PropertyChanges {
                target: text1
                color: "#f3f5f6"
            }
        },
        State {
            name: "clickedState"
            PropertyChanges {
                target: background
                color: "#f68585"
                border.width: 0
            }

            PropertyChanges {
                target: text1
                color: "#f3f5f6"
                font.pixelSize: 9
            }
        }
    ]

}

/*##^##
Designer {
    D{i:0;height:38;width:100}
}
##^##*/
