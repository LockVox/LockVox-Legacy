import QtQuick 2.0
import QtQuick.Controls 2.15

Item {
    id: item1
    width: 400
    height: 275
    objectName: "user_parameters"
    Rectangle {
        id: rectangle
        x: 0
        y: 0
        width: 400
        height: 275
        color: "#1b1f21"
        radius: 4

        anchors.bottom:parent.bottom
        anchors.top:parent.top
        anchors.left:parent.left
        anchors.right:parent.right


        BorderImage {
            id: account_pic
            x: 8
            y: 31
            width: 64
            height: 69
            source: "qrc:/qtquickplugin/images/template_image.png"
        }

        Text {
            id: page_title
            x: 108
            y: 8
            width: 185
            height: 14
            color: "#566369"
            text: qsTr("ACCOUNT INFORMATIONS")
            font.pixelSize: 12
            anchors.leftMargin: 112
            anchors.topMargin: 8
            anchors.rightMargin: 126
            anchors.bottomMargin: 252
            font.bold: true

            anchors.bottom:parent.bottom
            anchors.top:parent.top
            anchors.left:parent.left
            anchors.right:parent.right

        }

        Text {
            id: username_title
            x: 14
            y: 114
            width: 185
            height: 14
            color: "#566369"
            text: qsTr("USERNAME")
            font.pixelSize: 10
            anchors.rightMargin: 155
            anchors.bottomMargin: 142
            anchors.leftMargin: 14
            anchors.topMargin: 114
            font.bold: true
            anchors.bottom:parent.bottom
            anchors.top:parent.top
            anchors.left:parent.left
            anchors.right:parent.right
        }

        Text {
            id: password_title
            x: 14
            y: 218
            width: 185
            height: 14
            color: "#566369"
            text: qsTr("PASSWORD")
            font.pixelSize: 10
            anchors.leftMargin: 14
            anchors.topMargin: 218
            anchors.rightMargin: 320
            anchors.bottomMargin: 43
            font.bold: true
            anchors.bottom:parent.bottom
            anchors.top:parent.top
            anchors.left:parent.left
            anchors.right:parent.right
        }

        CustomQpushButton {
            id: changeUsernameBtn
            x: 258
            y: 123
            width: 73
            height: 30
            text: "Modify"

            anchors.bottom:parent.bottom
            anchors.rightMargin: 69
            anchors.bottomMargin: 123
            anchors.leftMargin: 258
            anchors.topMargin: 123
            anchors.top:parent.top
            anchors.left:parent.left
            anchors.right:parent.right

            Connections {
                target: changeUsernameBtn
                onClicked: item1.state = "changeUsernameState"
            }
        }

        CustomQpushButton {
            id: changeEmailBtn
            x: 258
            y: 176
            width: 73
            height: 30
            text: "Modify"

            anchors.bottom:parent.bottom
            anchors.rightMargin: 69
            anchors.bottomMargin: 70
            anchors.leftMargin: 258
            anchors.topMargin: 177
            anchors.top:parent.top
            anchors.left:parent.left
            anchors.right:parent.right

            Connections {
                target: changeEmailBtn
                onClicked: item1.state = "changeEmailState"
            }
        }

        Text {
            id: big_username_title
            objectName: "big_username"
            x: 89
            y: 55
            width: 248
            height: 21
            color: "#ffffff"
            text: qsTr("Username")
            font.pixelSize: 15
            anchors.rightMargin: 69
            anchors.bottomMargin: 203
            anchors.leftMargin: 87
            anchors.topMargin: 50
            font.bold: true

            anchors.bottom:parent.bottom
            anchors.top:parent.top
            anchors.left:parent.left
            anchors.right:parent.right


        }

        Text {
            id: password_label
            x: 14
            y: 238
            width: 185
            height: 14
            color: "#ffffff"
            text: qsTr("password")
            font.pixelSize: 12
            anchors.rightMargin: 155
            anchors.bottomMargin: 25
            anchors.leftMargin: 14
            anchors.topMargin: 235
            anchors.bottom:parent.bottom
            anchors.top:parent.top
            anchors.left:parent.left
            anchors.right:parent.right
        }

        Text {
            id: username_label
            objectName: "username"
            x: 14
            y: 131
            width: 185
            height: 14
            color: "#ffffff"
            text: qsTr("username")
            font.pixelSize: 12
            anchors.leftMargin: 14
            anchors.topMargin: 130
            anchors.rightMargin: 171
            anchors.bottomMargin: 130


            anchors.bottom:parent.bottom
            anchors.top:parent.top
            anchors.left:parent.left
            anchors.right:parent.right
        }

        CustomQpushButton {
            id: changePasswordBtn
            x: 258
            y: 230
            width: 73
            height: 30
            visible: true
            text: "Modify"
            anchors.bottom:parent.bottom
            anchors.rightMargin: 69
            anchors.bottomMargin: 16
            anchors.leftMargin: 258
            anchors.topMargin: 231
            anchors.top:parent.top
            anchors.left:parent.left
            anchors.right:parent.right
        }

        Text {
            id: email_title
            x: 14
            y: 164
            width: 185
            height: 14
            color: "#566369"
            text: qsTr("EMAIL")
            font.pixelSize: 10
            anchors.leftMargin: 14
            anchors.topMargin: 167
            anchors.rightMargin: 306
            anchors.bottomMargin: 94
            font.bold: true
            anchors.bottom:parent.bottom
            anchors.top:parent.top
            anchors.left:parent.left
            anchors.right:parent.right
        }

        Text {
            id: email_label
            objectName: "email"
            x: 14
            y: 184
            width: 185
            height: 14
            color: "#ffffff"
            text: qsTr("email@email.com")
            font.pixelSize: 12
            anchors.leftMargin: 14
            anchors.topMargin: 184
            anchors.rightMargin: 148
            anchors.bottomMargin: 77
            anchors.bottom:parent.bottom
            anchors.top:parent.top
            anchors.left:parent.left
            anchors.right:parent.right
        }

        CustomQpushButton {
            id: applyUsernameBtn
            x: 156
            y: 176
            width: 73
            height: 30
            visible: false
            text: "Modify"
        }

        CustomQpushButton {
            id: applyEmailBtn
            x: 156
            y: 151
            width: 73
            height: 30
            visible: false
            text: "Modify"
        }
    }

    TextField {
        id: changeEmailTextField
        x: 15
        y: 134
        visible: false
        placeholderText: qsTr("Text Field")
    }
    states: [
        State {
            name: "changeUsernameState"

            PropertyChanges {
                target: password_title
                x: 14
                y: 248
            }

            PropertyChanges {
                target: changeEmailBtn
                x: 258
                y: 206
            }

            PropertyChanges {
                target: password_label
                x: 14
                y: 268
            }

            PropertyChanges {
                target: customQpushButton2
                x: 258
                y: 198
                visible: true
            }

            PropertyChanges {
                target: email_title
                x: 14
                y: 194
            }

            PropertyChanges {
                target: email_label
                x: 14
                y: 214
            }

            PropertyChanges {
                target: item1
                height: 316
            }

            PropertyChanges {
                target: rectangle
                x: 0
                y: 0
                width: 400
                height: 317
            }

            PropertyChanges {
                target: username_label
                visible: false
            }

            PropertyChanges {
                target: changeUsernameBtn
                visible: false
            }

            PropertyChanges {
                target: changeEmailTextField
                x: 15
                y: 139
                width: 200
                height: 30
                visible: true
                placeholderText: qsTr("Type a new username")
            }

            PropertyChanges {
                target: applyUsernameBtn
                x: 258
                y: 138
                visible: true
                text: "Apply"
            }

            PropertyChanges {
                target: changePasswordBtn
                x: 258
                y: 260
            }
        },
        State {
            name: "changeEmailState"
            PropertyChanges {
                target: password_title
                x: 14
                y: 233
            }

            PropertyChanges {
                target: changeEmailBtn
                x: 258
                y: 237
                visible: false
            }

            PropertyChanges {
                target: password_label
                x: 14
                y: 253
            }

            PropertyChanges {
                target: customQpushButton2
                x: 258
                y: 123
                visible: true
            }

            PropertyChanges {
                target: email_title
                x: 14
                y: 155
            }

            PropertyChanges {
                target: email_label
                x: 14
                y: 175
                visible: false
            }

            PropertyChanges {
                target: item1
                height: 316
            }

            PropertyChanges {
                target: rectangle
                x: 0
                y: 0
                width: 400
                height: 317
            }

            PropertyChanges {
                target: username_label
                visible: true
            }

            PropertyChanges {
                target: changeUsernameBtn
                visible: true
            }

            PropertyChanges {
                target: changeEmailTextField
                x: 15
                y: 180
                width: 200
                height: 30
                visible: true
                placeholderText: qsTr("Type a new email")
            }

            PropertyChanges {
                target: applyUsernameBtn
                x: 258
                y: 138
                visible: false
                text: "Apply"
            }

            PropertyChanges {
                target: applyEmailBtn
                x: 258
                y: 182
                visible: true
                text: "Apply"
            }

            PropertyChanges {
                target: changePasswordBtn
                x: 258
                y: 245
            }
        }
    ]

}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}
}
##^##*/
