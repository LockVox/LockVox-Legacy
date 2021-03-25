import QtQuick 2.0
import QtQuick.Controls 2.15

Item {
    width: 400
    height: 293
    Rectangle {
        id: rectangle
        x: 10
        y: 11
        width: 379
        height: 274
        color: "#2e3538"
        radius: 7

        Text {
            id: titlePage
            x: 94
            y: 10
            width: 191
            height: 49
            color: "#33a5e5"
            text: qsTr("Change Username")
            font.pixelSize: 15
            horizontalAlignment: Text.AlignHCenter
            minimumPixelSize: 15
            font.bold: true
        }

        Text {
            id: explainationLabel
            x: 67
            y: 44
            width: 246
            height: 42
            color: "#838d8d"
            text: "Type a new username for your account "
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            font.bold: false
        }

        TextField {
            id: changeUsernameTextField
            x: 90
            y: 105
            width: 200
            height: 30
            text: ""
            placeholderText: qsTr("Change username")
        }

        CustomQpushButton2 {
            id: returnHomeButton
            x: 351
            y: 8
        }

        CustomQpushButton {
            id: changeUsernameButton
            x: 201
            y: 180
            text: "Change"
        }

        CustomQpushButton {
            id: cancelButton
            x: 75
            y: 180
            text: "Cancel"
        }
    }

}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.659999966621399}
}
##^##*/
