// qml/Main.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 500
    height: 400
    title: qsTr("Cerberus Viewer")

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20

        TextArea {
            text: dataModel.info
            readOnly: true
            wrapMode: TextEdit.Wrap
            font.pixelSize: 16
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
