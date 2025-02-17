/*
 * SPDX-FileCopyrightText: 2021 Wang Rui <wangrui@jingos.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.12
import org.kde.kirigami 2.15 as Kirigami
import QtQuick.Controls 2.2 as Controls
import QtQuick.Layouts 1.2
import KRecorder 1.0
import QtGraphicalEffects 1.0
import "commonsize.js" as CSJ

Kirigami.Page {
    id:recordMainView

    property int rightWidth: parent.width - mainLeftView.width
    property bool rightDefaultShow : !rightViewPlayer.visible

    leftPadding: 0
    topPadding: 0
    bottomPadding: 0
    rightPadding: 0

    background: Rectangle{
//        color: "#B3000000"
        //black #1C1C21
        color: "#E8EFFF"
    }

    ToastView{
        id:toastShow
    }

    function showToast(tips)
     {
         toastShow.toastContent = tips
         toastShow.x = (root.width - toastShow.width) / 2
         toastShow.y = root.height / 4 * 3
         toastShow.visible = true
     }

    RecordMainLeftView{
        id: mainLeftView

        anchors{
            top: parent.top
            topMargin: 41 *lastAppScaleSize
            bottom: parent.bottom
//            bottomMargin: 10 * lastAppScaleSize
            left: parent.left
            leftMargin: 17 * lastAppScaleSize
        }
        width:parent.width *(CSJ.LeftView.left_view_width/CSJ.ScreenCurrentWidth)
        height: parent.height
        color: "transparent"

        onItemClicked:{
            rightViewPlayer.ppTitle.renamePlayPageTitle(false)
            rightViewPlayer.setFileName()
            if(AudioPlayer.state === AudioPlayer.PlayingState){
                AudioPlayer.stop()
            }
            AudioPlayer.setVolume(100);
            AudioPlayer.setMediaPath(recording.filePath)
            recording.getTags()
            rightViewPlayer.recording = recording
        }
        
        onInsertRecordItem: {
            rightViewPlayer.ppTitle.renamePlayPageTitle(false)
            if(AudioPlayer.state === AudioPlayer.PlayingState){
                AudioPlayer.stop()
            }
            AudioPlayer.setVolume(100);
            AudioPlayer.setMediaPath(recording.filePath)
            recording.getTags()
            rightViewPlayer.recording = recording
        }

        onRecordImageClicked: {
            AudioPlayer.stop()
            pageStack.layers.push("qrc:/RecordPage.qml",{currentVX:0});
        }

        onItemLongClicked: {
        }

        onDeleteClicked: {
            AudioPlayer.stop()
            RecordingModel.deleteRecording(index);
            if (mainLeftView.leftItemCount > 0) {
                rightViewPlayer.recording = RecordingModel.firstRecording()
                if (rightViewPlayer.recording) {
                    AudioPlayer.setMediaPath(rightViewPlayer.recording.filePath)
                }else {
                    AudioPlayer.setMediaPath("")
                }
            } else {
                AudioPlayer.setMediaPath("")
            }
        }

        onRenameClicked: {
            rightViewPlayer.ppTitle.renamePlayPageTitle(true)
        }

    }

    PlayerPage{
        id:rightViewPlayer

        anchors{
            left: mainLeftView.right
            right: parent.right
            top: parent.top
            topMargin: 41 *lastAppScaleSize
            bottom: parent.bottom
            bottomMargin: 30 * lastAppScaleSize
            rightMargin: 23 * lastAppScaleSize//root.width * (CSJ.playPage_Bottom_Left_right_margin/CSJ.ScreenCurrentWidth)
//            leftMargin:root.width* (CSJ.playPage_Bottom_Left_right_margin/CSJ.ScreenCurrentWidth)
            leftMargin: 23 * lastAppScaleSize
        }
        width: parent.width - mainLeftView.width
        height: parent.height
        color: "transparent"
    }
    Rectangle{
        id:rightDefaultView

        anchors.right: parent.right
        anchors.left: rightViewPlayer.left
        visible: !mainLeftView.titleIsEdit
//        color:"#801C1C1C"
        //black #F20E0E0F
        color: "#993C3F48"
        height: parent.height
        MouseArea{
            anchors.fill: parent
            onClicked: {
                mainLeftView.checkBoxHide()
            }
        }
    }



}
