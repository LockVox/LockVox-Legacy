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
            font.bold: true
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
            font.bold: true
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
            font.bold: true
        }

        CustomQpushButton {
            id: changeUsernameBtn
            x: 258
            y: 123
            width: 73
            height: 30
            text: "Modify"

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
            width: 258
            height: 21
            color: "#ffffff"
            text: qsTr("Username")
            font.pixelSize: 15
            font.bold: true
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
        }

        CustomQpushButton {
            id: changePasswordBtn
            x: 258
            y: 230
            width: 73
            height: 30
            visible: true
            text: "Modify"
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
            font.bold: true
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
    D{i:0;formeditorZoom:2}
}
##^##*/
