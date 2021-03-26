import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0
import QtQuick 2.7
import QtQuick.Extras 1.4

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
    property alias channels: channels
    color: "#1b1f21"

    //Main Obj
    property alias login: login
    property alias listClient: listClient
    property alias register: register
    property alias userInfo: userInfo
    property alias connectServer: connectServer

    //Compenent
    property alias loginButton: loginButton
    property alias registerButton: registerButton
    property alias listChannel: listChannel

    //Layout
    //property alias regLog_layout: regLog_layout
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
        x: 380
        y: 85
        width: 235
        height: 278
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
            x: 50
            y: 85
            width: 137
            height: 157
        }

        StateServer {
            objectName: "stateServer"
            id: stateServer
            x: 18
            y: 8
            width: 200
            height: 71
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

    //property alias channel:channel
    ListView {

        signal currentIndexChanged(int index)

        objectName: "listChannels"
        id: listChannel

        anchors.top: parent.top

        anchors.bottom: audioWindow.top

        x: 940
        y: 70
        width: 154
        height: 472
        visible: false
        clip: true
        model: ChannelModel {
            m_channelsList: channelsList
        }

        delegate: Delegate_Channel {}
    }

    //property alias MessageWindow: MessageWindow
    property alias listMessage: listMessage
    MessageWindow {
        id: messageWindow
        x: 196
        y: 25
        visible: false

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        ListView {
            id: listMessage
            x: 7
            y: 79
            width: 235
            height: 199
            visible: false
            clip: true

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: messageWindow.left
            anchors.right: messageWindow.right

            model: MessageModel {
                m_messagesList: messagesList
            }

            verticalLayoutDirection: ListView.TopToBottom
            delegate: MessageWidget {
                Text {
                    text: from
                }

                Text {
                    text: content
                }

                //Delegate_Message {}
            }
        }
    }

    AudioWindow {

        anchors.bottom: parent.bottom
        id: audioWindow
        x: 150
        y: 260
        visible: false
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
                        onClicked: window.state = "splashScreenState"
                    }
                }
            }
        }
    }

    Channels {
        id: channels
        objectName: "channels"
        x: 6
        y: 85
        width: 188
        height: 251
    }




    property alias menuBar:menuBar
    property alias quit:quit
    property alias disconnect:disconnect
    property alias change_server:change_server


    MenuBar {
        objectName:"menu_bar"
        id: menuBar

        signal quit()
        signal disconnect()
        signal ping_server()
        signal change_server()


        x: 0
        y: 0
        width: parent.width
        height: 30
        visible: false
        Menu {
            objectName: "menu_quit_button"
            id: menu_quit_button
            Image {}

            Action {
                id:quit
                text: qsTr("&Quit")

            }
            Action {
                text: qsTr("&Disconnect")
            }
        }
        Menu {
            objectName: "menu_state_server"
            id: menu_state_button

            title: qsTr("Default")

            Action {
                text: qsTr("Change Server")
            }
            Action {
                text: qsTr("Ping server")
            }
        }

        delegate: MenuBarItem {
            id: menuBarItem

            contentItem: Text {
                text: menuBarItem.text
                font: menuBarItem.font
                opacity: enabled ? 1.0 : 0.3
                color: menuBarItem.highlighted ? "#ffffff" : "#21be2b"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                implicitWidth: parent.width
                implicitHeight: 30
                opacity: enabled ? 1 : 0.3
                color: menuBarItem.highlighted ? "#21be2b" : "transparent"
            }
        }

        Menu {}
    }

    BusyIndicator {
        id: busyIndicator
        x: 291
        y: 215
        visible: false
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
                x: 495
                y: 70
                width: 145

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
                x: 176
                y: 0
                width: 305
                height: 480
                visible: true
                anchors.rightMargin: 177
                anchors.bottomMargin: 0
                anchors.leftMargin: 176
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
                y: 71
                width: 179
                height: 287
                visible: true
                anchors.bottomMargin: -2
                anchors.topMargin: 71
            }

            PropertyChanges {
                target: listMessage
                x: 17
                y: 40
                width: 271
                height: 366

                visible: true
                keyNavigationWraps: true
                snapMode: ListView.NoSnap
                anchors.topMargin: 21
                anchors.bottomMargin: 61
                anchors.rightMargin: 47
                anchors.leftMargin: 14
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

            PropertyChanges {
                target: channels
                x: 0
                y: 71
                width: 179
                height: 287
            }

            PropertyChanges {
                target: menuBar
                visible: true
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
        },
        State {
            name: "splashScreen"

            PropertyChanges {
                target: loginButton
                visible: false
            }

            PropertyChanges {
                target: connectServer
                visible: false
            }

            PropertyChanges {
                target: stateServer
                visible: false
            }

            PropertyChanges {
                target: rect_con_server
                visible: false
            }

            PropertyChanges {
                target: rect_log_reg
                visible: false
            }

            PropertyChanges {
                target: lock_vox_logo1
                visible: false
            }

            PropertyChanges {
                target: busyIndicator
                x: 246
                y: 202
                width: 148
                height: 148
            }
        },
        State {
            name: "registerState1"
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
        }
    ]
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.8999999761581421;height:480;width:640}
}
##^##*/

