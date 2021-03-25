import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick 2.7

import Client 1.0
import Channel 1.0
import Message 1.0

Rectangle {

    objectName: "window"
    id: window
    x: 0
    y: 0
    visible: true
    anchors.fill: parent
    color: "#1b1f21"


    /*
    //Main Obj
    property alias login: login
    property alias listClient: listClient
    property alias register: register
    property alias userInfo: userInfo
    property alias connectServer: connectServer

    //Compenent
    property alias loginButton: loginButton
    property alias registerButton: registerButton


    //Layout
    property alias regLog_layout: regLog_layout
    */
    Rectangle {
        objectName: "rect_login"
        id: rect_log_reg
        color: "#00ffffff"
        border.color: "#1f3d5c"
        border.width: 2
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.rightMargin: 276
        anchors.leftMargin: 20
        anchors.bottomMargin: 14
        anchors.topMargin: 85

        RowLayout {
            id: rowLayout
            height: 53
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.rightMargin: 6
            anchors.leftMargin: 8
            anchors.topMargin: 0

            CustomQpushButton {
                id: registerButton
                width: 111
                text: "Create account"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                Connections {
                    target: registerButton
                    onClicked: window.state = "registerState"
                }
            }

            CustomQpushButton {
                id: loginButton
                text: "Log in"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true

                Connections {
                    target: loginButton
                    onClicked: window.state = ""
                }
            }
        }

        Register {
            objectName: "register"
            id: register
            x: 73
            y: 66
            visible: false
        }

        Login {
            objectName: "login"
            id: login
            x: 73
            y: 88
            width: 200
            height: 257
            visible: true
        }
    }

    Rectangle {

        id: rect_con_server
        x: 387
        y: 85
        width: 235
        height: 165
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 85
        anchors.rightMargin: 25

        color: "#00ffffff"
        border.color: "#1f3d5c"
        border.width: 2

        ConnectServer {
            objectName: "connect_server"
            id: connectServer
            x: 41
            y: 11
            width: 142
            height: 159
        }
    }

    Image {
        id: lock_vox_logo1
        x: parent.width / 2 - lock_vox_logo1.width / 2
        y: 0
        source: "lock_vox_logo1.png"
        width: 271
        height: 71
    }

    ListView {
        id: listClient
        x: 940
        y: 70
        width: 154
        height: 472
        visible: false
        clip: true

        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        model: ClientModel {
            m_clientsList: clientsList
        }

        delegate: Delegate_Client {}
    }

    Rectangle {
        id: parameters_list
        x: 0
        y: 0
        width: 200
        height: 200
        visible: false
        color: "#ffffff"
    }

    CustomQpushButton {
        id: returnHomeButton
        x: 515
        y: 8
        visible: false
    }

    ParametersWidget {
        id: parametersWidget
        x: 209
        y: 0
        visible: false
    }

    CustomQpushButton2 {
        id: returnHomeBtn
        x: 592
        y: 5
        visible: false

        Connections {
            target: returnHomeBtn
            onClicked: window.state = "Home"
        }
    }

    MessageWindow {
        id: messageWindow
        x: 415
        y: 480
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top

        visible: false
    }

    ListView {
        id: listMessage
        x: 180
        y: 960
        width: 235
        height: 199
        visible: false
        clip: true

        anchors.top: messageWindow.top
        anchors.bottom: messageWindow.bottom

        model: MessageModel {
            m_messagesList: messagesList
        }

        delegate: RowLayout {
            Text {
                text: model.from
            }
        }
    }

    Column {
        id: home
        x: 221
        y: 0
        width: 200
        height: 400
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        Row {
            id: leftSideHome
            width: 200
            height: 400
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            UserInfo {
                objectName: "userInfo"
                id: userInfo
                width: 166
                height: 90
                visible: false

                CustomQpushButton {
                    id: parameterButton
                    x: 85
                    y: 33

                    Connections {
                        target: parameterButton
                        onClicked: window.state = "parameterState"
                    }
                }
            }

            ListView {
                id: listChannel
                width: 154
                height: 472
                visible: false
                clip: true

                model: ChannelModel {
                    m_channelsList: channelsList
                }

                delegate: Delegate_Channel {}
            }

            AudioWindow {
                id: audioWindow
                visible: false

                anchors.left: parent.left

                anchors.bottom: parent.bottom
            }
        }
    }

    states: [
        State {
            name: "registerState"

            PropertyChanges {
                target: register
                visible: true
            }

            PropertyChanges {
                target: login
                visible: false
            }

            PropertyChanges {
                target: messageWindow
                visible: false
            }

            PropertyChanges {
                target: parameters_list
                visible: false
            }

            PropertyChanges {
                target: window
                visible: true
            }

            PropertyChanges {
                target: parametersWidget
                visible: false
            }
        },
        State {
            name: "Home"

            PropertyChanges {
                target: columnLayout
                visible: false
            }

            PropertyChanges {
                target: connectServer
                visible: false
            }

            PropertyChanges {
                target: userInfo
                x: 1
                y: 0
                width: 179
                height: 73
                visible: true
                color: "#1b1f21"
            }

            PropertyChanges {
                target: listClient
                x: 486
                y: 70
                width: 154
                height: 410
                visible: true
                anchors.rightMargin: 8
                anchors.bottomMargin: 14
                anchors.topMargin: 51
                anchors.leftMargin: 489
            }

            PropertyChanges {
                target: rect_con_server
                visible: false
            }

            PropertyChanges {
                target: rect_log_reg
                visible: false
                anchors.rightMargin: 319
            }

            PropertyChanges {
                target: lock_vox_logo1
                visible: false
            }

            PropertyChanges {
                target: messageWindow
                x: 178
                y: 0
                width: 288
                height: 480
                visible: true
                anchors.rightMargin: 168
                anchors.bottomMargin: 0
                anchors.leftMargin: 185
                anchors.topMargin: 0
            }

            PropertyChanges {
                target: window
                color: "#2e3538"
            }

            PropertyChanges {
                target: audioWindow
                x: 0
                y: 356
                width: 177
                height: 124
                visible: true
                anchors.rightMargin: 461
                anchors.bottomMargin: 0
                anchors.leftMargin: 1
                anchors.topMargin: 326
            }

            PropertyChanges {
                target: parameters_list
                visible: false
            }

            PropertyChanges {
                target: returnHomeButton
                visible: false
            }

            PropertyChanges {
                target: listChannel
                x: 0
                y: 79
                width: 179
                height: 268
                visible: true
            }

            PropertyChanges {
                target: listMessage
                x: 180
                y: 25
                width: 292
                height: 406
                visible: true
                anchors.topMargin: 33
                anchors.bottomMargin: 48
                anchors.rightMargin: 168
                anchors.leftMargin: 230
            }

            PropertyChanges {
                target: leftSideHome
                x: 1
                y: 0
                width: 180
                height: 480
                rightPadding: 2
                leftPadding: 2
                spacing: 0
                topPadding: 0
                layoutDirection: Qt.LeftToRight
            }

            PropertyChanges {
                target: home
                x: 0
                y: 0
                width: 640
                height: 480
            }

            PropertyChanges {
                target: parameterButton
                x: 94
                y: 53
                width: 77
                height: 20
                text: "parameter"
            }
        },
        State {
            name: "parameterState"
            PropertyChanges {
                target: columnLayout
                visible: false
            }

            PropertyChanges {
                target: connectServer
                visible: false
            }

            PropertyChanges {
                target: userInfo
                x: 0
                y: 0
                width: 179
                height: 73
                visible: false
                color: "#1b1f21"
            }

            PropertyChanges {
                target: listClient
                x: 478
                y: 70
                width: 162
                height: 410
                visible: false
            }

            PropertyChanges {
                target: rect_con_server
                visible: false
            }

            PropertyChanges {
                target: rect_log_reg
                visible: false
                anchors.rightMargin: 319
            }

            PropertyChanges {
                target: lock_vox_logo1
                visible: false
            }

            PropertyChanges {
                target: messageWindow
                visible: false
            }

            PropertyChanges {
                target: window
                color: "#2e3538"
            }

            PropertyChanges {
                target: audioWindow
                x: 0
                y: 356
                visible: false
            }

            PropertyChanges {
                target: parameters_list
                x: 0
                y: 0
                width: 160
                height: 480
                visible: true
                color: "#1b1f21"
            }

            PropertyChanges {
                target: returnHomeButton
                x: 532
                y: 8
                text: "return"
            }

            PropertyChanges {
                target: parametersWidget
                x: 196
                y: 85
                visible: true
            }

            PropertyChanges {
                target: returnHomeBtn
                x: 612
                y: 8
                visible: true
            }
        }
    ]
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:1.100000023841858;height:480;width:640}
}
##^##*/

