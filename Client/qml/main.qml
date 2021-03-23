import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

import "./Client/qml"
import Client 1.0
import Channel 1.0
import Message 1.0

ApplicationWindow {

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
    }
}


