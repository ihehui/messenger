import QtQuick 2.6
import QtQuick.Window 2.2
//import Qt.labs.controls 1.0
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4





//聊天显示框
Rectangle {
    id: root

    width : 500
    height: 400

    //anchors.top: titleRect.bottom
    color: "#F7F7F7"
    border.color: "lightgray"

    ListView {
        id: chatMessagesList;
        objectName: "chatMessagesList";
        anchors.fill: parent
        anchors.margins: 5
        clip: true
        spacing: 15
        currentIndex: count - 1
        ScrollBar.vertical: ScrollBar { }




        property string systemID: "0";
        //property string peerID: "10";
        property string myID: "11";

        //property string peerNickName: "Test";
        property string peerHeadICON: "qrc:/face/3.png";
        property string myHeadICON: "qrc:/face/4.png";
        property string peerBorderImage: "qrc:/qml/chat_bg_left.png";
        property string myBorderImage: "qrc:/qml/chat_bg_right.png";


        signal linkActivated(string url);
        function appendMessage(usrID, timestamp, msg){
            var peer = (usrID !== myID ? true : false);
            var system = (usrID === systemID ? true : false) ;

            model.append({"userID":usrID, "timestamp":timestamp, "content":msg, "isPeer":peer, "isSystem":system})
        }




        model: ListModel {
            ListElement {
                userID: "0";
                timestamp: ""
                content: "";
                isPeer: false;
                isSystem: false
            }
        }

        delegate: Item {

            id: delegateItem
            width: parent.width
            height: Math.max(52, 22 + borderImageContent.height)

//            property bool isPeer: userID != chatMessagesList.myID ? true : false ;
//            property bool isSystem: userID == chatMessagesList.systemID ? true : false ;


            Image {
                id: headPortrait
                width: 48
                height: 48
                x: isPeer ? 2: parent.width - width - 2
                y: 2

                visible: isSystem ? false : true
                source:isPeer ? chatMessagesList.peerHeadICON : chatMessagesList.myHeadICON

            }
            BorderImage {
                id: msgWrapper
                source: isPeer ? chatMessagesList.peerBorderImage : chatMessagesList.myBorderImage
                width: borderImageContent.width + 40
                height: borderImageContent.height + 20
                x: isPeer ? headPortrait.x + headPortrait.width + 2 : headPortrait.x - 2 - width

                border.left: isPeer ? 16 : 6
                border.top: 38
                border.right: isPeer ? 6 : 16
                border.bottom: 6


                TextArea{
                    id: borderImageContent
                    anchors.centerIn: parent
                    readOnly: true
                    textFormat: TextEdit.RichText
                    selectByMouse : true
                    focus: true

                    width: Math.min(380, implicitWidth)
                    horizontalAlignment: isPeer ? TextEdit.left : TextEdit.AlignRight
                    verticalAlignment: TextEdit.AlignVCenter
                    font.pointSize: 10
                    clip: true
                    wrapMode: TextEdit.WordWrap
                    text: content

                    onFocusChanged: {
                        if(!activeFocus){
                            borderImageContent.deselect();
                            //borderImageContent.select(0,0);
                        }
                        console.log("onFocusChanged")
                    }

                }

//                Text {
//                    id: borderImageContent
//                    anchors.centerIn: parent
//                    width: Math.min(380, implicitWidth)
//                    horizontalAlignment: isPeer == 0 ? Text.AlignRight : Text.AlignLeft
//                    verticalAlignment: Text.AlignVCenter
//                    font.pointSize: 16
//                    clip: true
//                    wrapMode: Text.WrapAnywhere
//                    text: content
//                }




            }


            Component.onCompleted: {

                console.log("isSystem?"+isSystem?"T":"F")
                console.log("isPeer?"+isPeer?"T":"F")


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

        Component.onCompleted: {
            model.clear()
        }

    }







}

