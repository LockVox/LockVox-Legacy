import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

import "./Client/qml"
import Client 1.0
import Channel 1.0
import Message 1.0

ApplicationWindow {

    //flags: Qt.FramelessWindowHint
    width: 640
    height: 480
    visible: true
    title: "Lock-Vox"
/*
    ListView {
        objectName: "listChannels"
        id: listChannel


        x: 940
        y: 70
        width: 154
        height: 472
        visible: false
        clip: true
        model: ChannelModel {
            m_channelsList: channelsList
        }

        delegate: Delegate_Channel {

        }

        onCurrentIndexChanged: {
            console.log(currentIndex)
            //item.currentIndexChanged(currentIndex)
           }
    }*/

    Screen01{
        anchors.fill: parent
        //listChannel.onCurrentIndexChanged:  console.log(listChannel.currentIndex)

        quit.onTriggered: menuBar.quit()
        disconnect.onTriggered: menuBar.disconnect()
        change_server.onTriggered: menuBar.change_server()

        quit_confirm.onClicked: quit_popup.confirmQuit(1)
        quit_back.onClicked: quit_popup.confirmQuit(0)

    }
}
