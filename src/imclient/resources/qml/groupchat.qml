import QtQuick 2.6
import QtQuick.Window 2.2
//import Qt.labs.controls 1.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
//import QtGraphicalEffects 1.0




//聊天显示框
Rectangle {
    id: displayRect


    width : 500
    height: 400

    //anchors.top: titleRect.bottom
    color: "#F7F7F7"
    border.color: "lightgray"

    ListView {
        id: chatMessagesList
        objectName: "chatMessagesList";
        anchors.fill: parent
        anchors.margins: 5
        clip: true
        spacing: 15
        currentIndex: count - 1
        ScrollBar.vertical: ScrollBar { }

        signal linkActivated(string url);

        property string systemID: "0";
        property string peerID: "10";
        property string myID: "11";

        property string peerNickName: "Test";
        property string peerHeadICON: "qrc:/face/3.png";
        property string myHeadICON: "qrc:/face/4.png";
//        property string peerBorderImage: "qrc:/img/chat_to_bg_normal.9.png";
//        property string myBorderImage: "qrc:/img/chat_from_bg_normal.9.png";
        property string peerBorderImage: "qrc:/qml/chat_bg_left.png";
        property string myBorderImage: "qrc:/qml/chat_bg_right.png";


        model: ListModel {
            ListElement {
                userID: "0"
                content: "你好"
            }

        }

        delegate: Item {


            id: delegateItem
            width: parent.width
            height: Math.max(52, 16 + borderImageContent.height)


            property bool isPeer: userID != chatMessagesList.myID ? true : false ;
            property bool isSystem: userID == chatMessagesList.systemID ? true : false ;



            Image {
                id: headPortrait
                width: 48
                height: 48
                x: isPeer ? parent.width - width - 2 : 2
                y: 2

               visible: isSystem ? false : true
                source:isPeer ? chatMessagesList.peerHeadICON : chatMessagesList.myHeadICON

            }
            BorderImage {
                id: msgWrapper
                source: isPeer ? chatMessagesList.peerBorderImage : chatMessagesList.myBorderImage
                width: borderImageContent.width + 40
                height: borderImageContent.height + 20
                x: isPeer ? headPortrait.x - 2 - width :
                                    headPortrait.x + headPortrait.width + 2
                border.left: isPeer ? 6 : 16
                border.top: 38
                border.right: isPeer ? 16 : 6
                border.bottom: 6


                TextArea{
                    id: borderImageContent
                    readOnly: true
                    textFormat: TextEdit.RichText
                    selectByMouse : true
                    focus: true

                    anchors.centerIn: parent
                    width: Math.min(380, implicitWidth)
                    horizontalAlignment: isPeer ? Text.AlignRight : Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 10
                    clip: true
                    //wrapMode: Text.WrapAnywhere
                    text: content
                    font.family: "微软雅黑"

                    onFocusChanged: {
                        if(!activeFocus){
                            borderImageContent.deselect();
                            //borderImageContent.select(0,0);
                        }
                        console.log("onFocusChanged")
                    }

                }


            }

            Component.onCompleted: {
                if(delegateItem.isSystem){
                    headPortrait.visible = false;
                    msgWrapper.source = "";

                    borderImageContent.color = "gray";
                    borderImageContent.font.pointSize = 8;
                    borderImageContent.horizontalAlignment = Text.AlignHCenter

                    console.log("------onCompleted------")
                }

                borderImageContent.linkActivated.connect(chatMessagesList.linkActivated);

            }


        }

    }







}

