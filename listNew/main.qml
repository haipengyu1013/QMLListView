import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    visible: true
    width: 800
    height: 600
    title: qsTr("Hello World")


    ListView {
        id: listView
        anchors.fill: parent
        snapMode: ListView.SnapToItem
        spacing: 10
        property int checkCount
        property int tempIndx: -1
        interactive: false
        model: historyModel

        delegate: Rectangle {
            width: listView.width
            height: 40

            Text {
                anchors.centerIn: parent
                font.pixelSize: 24
                text: itemTime + " [" + (itemRead ? "已读" : "未读") + "]"
                color: "black"
            }

            Rectangle {
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                width: 40
                height: 40
                radius: 20
                color: itemChecked ? "red" : "#00000000"
                border.width: 2
                border.color: "white"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (itemChecked)
                        listView.checkCount--
                    else
                        listView.checkCount++

                    print("onClicked")
                    historyModel.setValue(index, "itemChecked", !itemChecked)
                }
            }
        }


        MouseArea {
            anchors.fill: parent

            propagateComposedEvents: true
            onClicked: {
                mouse.accepted = false
            }

            onPressed: {
                listView.tempIndx = listView.indexAt(mouseX , mouseY)
            }

            onPressAndHold: {

            }

            onPositionChanged: {
                historyModel.selectItems(listView.tempIndx , listView.indexAt(mouseX , mouseY) , 0 , 0 , 0)
            }
            onReleased: {
                historyModel.clearAreaItems();
            }
        }
    }

    GridView {
        id: gridView
        anchors.fill: parent
        cellHeight: 70
        cellWidth: 70
        interactive: false
        visible: false

        model: historyModel
        delegate: Rectangle {
            width: gridView.cellWidth
            height: gridView.cellWidth
            radius: gridView.cellWidth / 2
            color: itemChecked ? "red" : "blue"
            border.width: 2
            border.color: "white"

            Text {
                anchors.centerIn: parent
                font.pixelSize: 24
                text: index
                color: "white"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    //                    if (itemChecked)
                    //                        listView.checkCount--
                    //                    else
                    //                        listView.checkCount++

                    print("onClicked")
                    historyModel.setValue(index, "itemChecked", !itemChecked)
                }
            }
        }

        property int tempIndx: -1


        MouseArea {
            anchors.fill: parent
            propagateComposedEvents: true
            onClicked: {
                mouse.accepted = false
            }

            onPressed: {
                gridView.tempIndx = gridView.indexAt(mouseX , mouseY)
            }

            onPressAndHold: {

            }

            onPositionChanged: {
                historyModel.selectItems(gridView.tempIndx , gridView.indexAt(mouseX , mouseY) , 1 , gridView.width / gridView.cellWidth )
            }

            onReleased: {
                historyModel.clearAreaItems();
            }
        }
    }

    Row {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 40
        spacing: 40

        Rectangle {
            width: 200
            height: 40
            color: "green"

            Text {
                anchors.centerIn: parent
                font.pixelSize: 20
                color: "white"
                text: "全选 " + listView.checkCount
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (listView.checkCount == listView.count)
                        listView.checkCount = 0
                    else
                        listView.checkCount = listView.count

                    for (var i = 0; i < listView.count; i++)
                        historyModel.setValue(i, "itemChecked", listView.checkCount)
                }
            }
        }

        Rectangle {
            width: 200
            height: 40
            color: "green"

            Text {
                anchors.centerIn: parent
                font.pixelSize: 20
                color: "white"
                text: "已读"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (listView.checkCount == 0)
                        return

                    for (var i = 0; i < listView.count; i++) {
                        if (historyModel.value(i, "itemChecked"))
                            historyModel.setValue(i, "itemRead", true)
                    }
                }
            }
        }

        Rectangle {
            width: 200
            height: 40
            color: "green"

            Text {
                anchors.centerIn: parent
                font.pixelSize: 20
                color: "white"
                text: "删除"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (listView.checkCount == 0)
                        return

                    if (listView.checkCount == listView.count) {
                        listView.checkCount = 0
                        historyModel.clear()
                        return
                    }
                    listView.checkCount = 0
                    historyModel.remove()
                }
            }
        }
    }
}
