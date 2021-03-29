import QtQuick 2.0
import QtQuick.Controls 2.15

Item {
    id: user_parameters
    width: 400
    height: 275
    objectName: "user_parameters"
    signal change_username(string new_pseudo)
    signal change_email(string new_email)
    signal change_description(string new_description)
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
            anchors.top:parent.top
            anchors.bottomMargin: 252
            font.bold: true


            anchors.horizontalCenter:parent.horizontalCenter

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

            anchors.right:parent.right

            Connections {
                target: changeUsernameBtn
                onClicked: user_parameters.state = "changeUsernameState"
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

            anchors.right:parent.right

            Connections {
                target: changeEmailBtn
                onClicked: user_parameters.state = "changeEmailState"
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
            anchors.bottomMargin: 19
            anchors.leftMargin: 258
            anchors.topMargin: 229

            anchors.right:parent.right

            Connections {
                target: changePasswordBtn
                onClicked: user_parameters.state = "changePasswordState"
            }
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

            anchors.bottom: parent.bottom

            anchors.right:parent.right

            Connections {
                        target: applyUsernameBtn
                        onClicked: {

                                user_parameters.change_username(changeUsernameTextField.text)
                                changeUsernameTextField.clear()
                                user_parameters.state = ""

                           }
            }


        }
        
        CustomQpushButton {
            id: applyEmailBtn
            x: 156
            y: 151
            width: 73
            height: 30
            visible: false
            text: "Modify"
            anchors.bottom: parent.bottom

            anchors.right:parent.right

            Connections {
                        target: applyEmailBtn
                        onClicked: {

                            user_parameters.change_email(changeEmailTextField.text)
                            changeEmailTextField.clear()
                            user_parameters.state = ""


                        }
            }





        }
        
        CustomQpushButton {
            id: applyPasswordBtn
            x: 153
            y: 158
            width: 73
            height: 30
            visible: false
            text: "Modify"
            anchors.bottom: parent.bottom

            anchors.right:parent.right
        }

        Text {
            id: big_description_title
             objectName: "big_description"
            x: 88
            y: 64
            width: 248
            height: 21
            color: "#33a5e5"
            text: qsTr("Description")
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            font.pixelSize: 15
            anchors.leftMargin: 87
            anchors.rightMargin: 155
            anchors.topMargin: 79
            anchors.bottomMargin: 203
            font.bold: true
        }

        CustomQpushButton {
            id: changeDescriptionBtn
            x: 258
            y: 79
            width: 73
            height: 30
            text: "Modify"
            anchors.left: big_description_title.right
            anchors.top: parent.top

            Connections {
                target: changeDescriptionBtn
            }

            Connections {
                target: changeDescriptionBtn
                onClicked: user_parameters.state = "changeDescriptionState"
            }
            anchors.leftMargin: 13
            anchors.rightMargin: 69
            anchors.topMargin: 75
            anchors.bottomMargin: 168
        }

        CustomQpushButton {
            id: applyDescriptionBtn
            x: 162
            y: 142
            width: 73
            height: 30
            visible: false
            text: "Modify"
            anchors.right: parent.right
            anchors.top: parent.top
            Connections {
                target: applyDescriptionBtn

                onClicked: {

                    user_parameters.change_description(changeDescriptionTextField.text)
                    changeDescriptionTextField.clear()
                    user_parameters.state = ""


                }
            }
        }
    }

    TextField {
        id: changeEmailTextField
        x: 15
        y: 134
        visible: false
        placeholderText: qsTr("Text Field")
        anchors.bottom:parent.bottom

        anchors.left:parent.left
        anchors.right:parent.right

        color:"white"
        background: Rectangle {
            color:"#313639"
            radius:0
            border.width: 0
        }

        Keys.onPressed: {

            if(event.key == Qt.Key_Return){

                user_parameters.change_email(changeEmailTextField.text)
                changeEmailTextField.clear()
                user_parameters.state = ""


            }

        }
    }

    TextField {
        id: changeUsernameTextField
        x: 9
        y: 140
        visible: false
        placeholderText: qsTr("Text Field")
        anchors.bottom:parent.bottom

        anchors.left:parent.left
        anchors.right:parent.right

        color:"white"
        background: Rectangle {
            color:"#313639"
            radius:0
            border.width: 0
        }

        Keys.onPressed: {

            if(event.key == Qt.Key_Return){

                user_parameters.change_username(changeUsernameTextField.text)
                changeUsernameTextField.clear()
                user_parameters.state = ""


            }
       }
    }

    TextField {
        id: changePasswordTextField
        x: 3
        y: 149
        visible: false
        placeholderText: qsTr("Text Field")
        anchors.bottom:parent.bottom

        anchors.left:parent.left
        anchors.right:parent.right

        color:"white"
        background: Rectangle {
            color:"#313639"
            radius:0
            border.width: 0
        }
    }
    
    CustomQpushButton2 {
        id: cancelBtn
        x: 342
        y: 185
        visible: false
        anchors.right: parent.right
        anchors.bottom:parent.bottom

        Connections {
            target: cancelBtn
            onClicked: user_parameters.state = ""
        }
    }

    TextField {
        id: changeDescriptionTextField
        x: 23
        y: 125
        visible: false
        color: "#ffffff"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        background: Rectangle {
            color: "#313639"
            radius: 0
            border.width: 0
        }
        placeholderText: qsTr("Text Field")
    }

    CustomQpushButton2 {
        id: cancelBtn2
        x: 350
        y: 176
        visible: false
        anchors.right: parent.right
        anchors.top: parent.top
        Connections {
            target: cancelBtn2
        }

        Connections {
            target: cancelBtn2
            onClicked: user_parameters.state = ""
        }
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
                anchors.rightMargin: 69
                anchors.leftMargin: 258
                anchors.topMargin: 178
                anchors.bottomMargin: 71
            }

            PropertyChanges {
                target: password_label
                x: 14
                y: 268
            }

            PropertyChanges {
                target: cancelBtn
                x: 339
                y: 137
                width: 20
                height: 20
                visible: true
                anchors.rightMargin: 41
                anchors.bottomMargin: 118
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
                y: 174
                width: 200
                height: 30
                visible: false
                anchors.rightMargin: -15
                anchors.bottomMargin: 71
                anchors.leftMargin: 15
                placeholderText: qsTr("Type a new username")
            }

            PropertyChanges {
                target: applyUsernameBtn
                x: 258
                y: 131
                height: 30
                visible: true
                text: "Apply"
                anchors.rightMargin: 69
                anchors.bottomMargin: 114
                anchors.leftMargin: 258
            }

            PropertyChanges {
                target: changePasswordBtn
                x: 258
                y: 260
                anchors.rightMargin: 69
                anchors.bottomMargin: 25
                anchors.leftMargin: 258
                anchors.topMargin: 223
            }

            PropertyChanges {
                target: changeUsernameTextField
                x: 15
                y: 133
                width: 200
                height: 28
                visible: true
                anchors.rightMargin: 199
                anchors.bottomMargin: 114
                anchors.leftMargin: 15
                placeholderText: qsTr("new username")
            }

            PropertyChanges {
                target: changePasswordTextField
                x: 0
                y: 143
                anchors.rightMargin: -23
                anchors.bottomMargin: 92
                anchors.leftMargin: 23
            }

            PropertyChanges {
                target: page_title
                y: 8
                anchors.rightMargin: 110
                anchors.leftMargin: 128
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
                target: cancelBtn
                x: 342
                y: 181
                width: 20
                height: 20
                visible: true
                anchors.rightMargin: 38
                anchors.bottomMargin: 74
            }
            
            PropertyChanges {
                target: email_title
                x: 14
                y: 155
                anchors.rightMargin: 306
                anchors.bottomMargin: 97
                anchors.leftMargin: 14
                anchors.topMargin: 164
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
                x: 258
                y: 123
                visible: true
                anchors.rightMargin: 69
                anchors.bottomMargin: 122
            }
            
            PropertyChanges {
                target: changeEmailTextField
                x: 15
                y: 181
                width: 200
                height: 28
                visible: true
                anchors.rightMargin: 202
                anchors.bottomMargin: 66
                anchors.leftMargin: 14
                placeholderText: qsTr("a new email")
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
                y: 175
                height: 30
                visible: true
                text: "Apply"
                anchors.rightMargin: 69
                anchors.bottomMargin: 70
                anchors.leftMargin: 258
            }
            
            PropertyChanges {
                target: changePasswordBtn
                x: 258
                y: 245
                anchors.rightMargin: 69
                anchors.bottomMargin: 25
                anchors.leftMargin: 258
                anchors.topMargin: 223
            }

            PropertyChanges {
                target: changePasswordTextField
                y: 175
                anchors.rightMargin: -19
                anchors.bottomMargin: 60
                anchors.leftMargin: 19
            }

            PropertyChanges {
                target: changeUsernameTextField
                y: 129
                anchors.rightMargin: -19
                anchors.bottomMargin: 106
                anchors.leftMargin: 19
            }
        },
        State {
            name: "changePasswordState"
            PropertyChanges {
                target: password_title
                x: 14
                y: 233
            }

            PropertyChanges {
                target: changeEmailBtn
                x: 258
                y: 237
                visible: true
            }
            
            PropertyChanges {
                target: password_label
                x: 14
                y: 253
                visible: false
            }

            PropertyChanges {
                target: cancelBtn
                x: 340
                y: 242
                width: 20
                height: 20
                visible: true
                anchors.rightMargin: 40
                anchors.bottomMargin: 13
            }

            PropertyChanges {
                target: email_title
                x: 14
                y: 155
                anchors.rightMargin: 306
                anchors.topMargin: 164
                anchors.leftMargin: 14
                anchors.bottomMargin: 97
            }

            PropertyChanges {
                target: email_label
                x: 14
                y: 175
                visible: true
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
                y: 245
                width: 200
                height: 30
                visible: false
                anchors.rightMargin: 190
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
                y: 177
                visible: false
                text: "Apply"
            }

            PropertyChanges {
                target: changePasswordBtn
                x: 258
                y: 245
                visible: false
                anchors.rightMargin: 69
                anchors.topMargin: 223
                anchors.leftMargin: 258
                anchors.bottomMargin: 25
            }
            
            PropertyChanges {
                target: changePasswordTextField
                x: 15
                y: 237
                width: 200
                height: 30
                visible: true
                anchors.rightMargin: 206
                anchors.bottomMargin: 8
                anchors.leftMargin: 15
                placeholderText: qsTr("new password")
            }
            
            PropertyChanges {
                target: applyPasswordBtn
                x: 258
                y: 236
                height: 30
                visible: true
                text: "Apply"
                anchors.rightMargin: 69
                anchors.bottomMargin: 9
                anchors.leftMargin: 258
            }

            PropertyChanges {
                target: page_title
                anchors.rightMargin: 110
                anchors.topMargin: 8
                anchors.leftMargin: 128
            }
        },
        State {
            name: "changeDescriptionState"

            PropertyChanges {
                target: cancelBtn
                x: 342
                y: 82
                visible: false
                anchors.rightMargin: 38
                anchors.bottomMargin: 173
            }

            PropertyChanges {
                target: changeDescriptionBtn
                visible: false
            }

            PropertyChanges {
                target: changeDescriptionTextField
                y: 77
                height: 25
                visible: true
                anchors.topMargin: 80
                placeholderText: qsTr("Type a description")
                anchors.leftMargin: 84
                anchors.rightMargin: 154
                anchors.bottomMargin: 173
            }

            PropertyChanges {
                target: big_description_title
                visible: false
            }

            PropertyChanges {
                target: applyDescriptionBtn
                x: 258
                y: 77
                visible: true
                text: "Apply"
                anchors.topMargin: 77
                anchors.rightMargin: 69
                anchors.bottomMargin: 168
            }

            PropertyChanges {
                target: cancelBtn2
                x: 342
                y: 85
                width: 20
                height: 20
                visible: true
                anchors.topMargin: 82
                anchors.rightMargin: 38
                anchors.bottomMargin: 170
            }
        }
    ]

}



/*##^##
Designer {
    D{i:0;formeditorZoom:1.659999966621399}
}
##^##*/
