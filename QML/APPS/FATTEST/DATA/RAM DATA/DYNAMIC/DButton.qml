
import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Button {
  id: button
  style: touchStyle
  iconSource: ""
  Component {
    id: touchStyle
    ButtonStyle {
      padding.left: button.iconSource != "" ? 38 : 14
      padding.right: 14
      background: Item {
        anchors.fill: parent
        implicitWidth: 160
        implicitHeight: 30
        
        Image {
          id: icon
          x: 11
          y: 8
          z: 1
          height: 16
          width: 16
          source: button.iconSource
          visible: button.iconSource != ""
        }
        
        Rectangle {
          id: rectangle
          anchors.fill: parent
          color: (button.checked || button.pressed)
                 ? creatorTheme.welcome_ForegroundPrimaryColor
                 : (button.hovered
                    ? creatorTheme.welcome_HoverColor
                    : creatorTheme.welcome_ButtonBackgroundColor)
          border.width: 1
          border.color: (button.checked || button.pressed)
                        ? creatorTheme.welcome_ForegroundPrimaryColor
                        : creatorTheme.welcome_ForegroundSecondaryColor
          radius: creatorTheme.FlatToolBars ? 0 : 4
        }
      }
      
      label: Text {
        id: text
        renderType: Text.NativeRendering
        verticalAlignment: Text.AlignVCenter
        text: button.text
        color: (button.checked || button.pressed)
               ? creatorTheme.welcome_BackgroundColor
               : creatorTheme.welcome_TextColor
        font.pixelSize: 15
        font.bold: false
        smooth: true
      }
    }
  }
}
