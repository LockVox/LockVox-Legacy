import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0



Item {
    objectName: "login"
    id: login

    signal login_request(string email, string password)
    width: 200
    height: 300


    Column {
        id: column
        x: 0
        y: 6
        height: 85
        spacing: 8

        anchors.left: parent.left
        anchors.right:parent.right
        anchors.verticalCenter:parent.verticalCenter
        anchors.verticalCenterOffset:-90

        anchors.rightMargin: 8
        anchors.leftMargin: 8
        anchors.topMargin: 6
        anchors.bottomMargin:300



        TextField {
            id: emailField
            width: 182
            height: 40
            placeholderText: qsTr("Email")

            anchors.top:parent.top
            anchors.left:parent.left
            anchors.right:parent.right
            color:"white"
            background: Rectangle {
                color:"#313639"
                radius:0
                border.width: 0
            }


        }

        TextField {
            id: passwordField
            width: 182
            height: 40
            placeholderText: qsTr("Password")
            echoMode:TextInput.Password
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

                    login.login_request(emailField.text,passwordField.text)
                     passwordField.clear()

                }

            }


            //TextField.passwordCharacter: true


        }

    }

    CustomQpushButton {
        id: customQpushButton
        x: 50
        y: 243
        width: 100
        height: 34
        text: qsTr("Sign In")

        anchors.bottom:parent.bottom
        anchors.horizontalCenterOffset: 0
        anchors.bottomMargin: 23
        anchors.horizontalCenter:parent.horizontalCenter

        onClicked: {

            login.login_request(emailField.text,passwordField.text)
            passwordField.clear()

        }

    }
}




/*##^##
Designer {
    D{i:0;formeditorZoom:1.25}
}
##^##*/
