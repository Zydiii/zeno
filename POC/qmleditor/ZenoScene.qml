import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.2

Rectangle {
    id: thisScene
    color: '#222'
    anchors.fill: collection

    property var collection: null
    property var selectedChildren: []
    property ZenoHalfLink halfLink: null
    property var nodes: []
    property var links: []

    function doSelect(item, multiselect) {
        if (item == null) {
            for (var i in selectedChildren)
                selectedChildren[i].selected = false
            selectedChildren = []
        } else if (multiselect) {
            if (!selectedChildren.includes(item)) {
                selectedChildren.push(item)
                item.selected = true
            } else {
                selectedChildren = selectedChildren.filter(function(e) { return e != item })
                item.selected = false
            }
        } else {
            if (selectedChildren.length == 1 && selectedChildren.includes(item)) {
                for (var i in selectedChildren)
                    selectedChildren[i].selected = false
                selectedChildren = []
                item.selected = false
            } else {
                for (var i in selectedChildren)
                    selectedChildren[i].selected = false
                selectedChildren = [item]
                item.selected = true
            }
        }
    }

    function dumpScene() {
        var r_nodes = []
        for (var i in nodes) {
            var r_node = nodes[i].dumpNode()
            r_nodes.push(r_node)
        }
        var r_links = []
        for (var i in links) {
            var r_link = links[i].dumpLink()
            r_links.push(r_link)
        }
        var r_view = {}
        r_view.x = sceneRect.x
        r_view.y = sceneRect.y
        r_view.scale = sceneRect.scale
        var r_scene = {}
        r_scene.view = r_view
        r_scene.nodes = r_nodes
        r_scene.links = r_links
        return r_scene
    }

    function removeNode(node) {
        nodes = nodes.filter(function (e) { return e != node })
        node.detachNode()
        node.destroy()
    }

    function removeLink(link) {
        links = links.filter(function (e) { return e != link })
        link.srcSocket.removeLink(link)
        link.dstSocket.removeLink(link)
        link.destroy()
    }

    function deleteSelection() {
        for (var i in selectedChildren) {
            var o = selectedChildren[i]
            if (typeof o.deleteThisLink == 'function')
                o.deleteThisLink()
        }
        for (var i in selectedChildren) {
            var o = selectedChildren[i]
            if (typeof o.deleteThisNode == 'function')
                o.deleteThisNode()
        }
    }

    function addNode(args) {
        args.scene = thisScene
        var node = compZenoNode.createObject(sceneRect, args)
        nodes.push(node)
    }

    function addNodeByName(kind, position) {
        var ids = nodes.filter(function(node) {
            return node.name.startsWith(kind)
        }).map(function(node) {
            return node.name.slice(kind.length)
        })
        var id = 1
        while (ids.includes(id.toString()))
            id++
        var name = kind + id
        addNode({
            kind: kind,
            name: name,
            x: position.x,
            y: position.y,
        })
    }

    function addLink(args) {
        args.scene = thisScene
        var link = compZenoLink.createObject(sceneRect, args)
        args.srcSocket.attachLink(link)
        args.dstSocket.attachLink(link)
        links.push(link)
    }

    function linkInput(input) {
        if (halfLink == null) {
            halfLink = compZenoHalfLink.createObject(sceneRect, {
                scene: thisScene,
                srcSocket: null,
                dstSocket: input,
                mousePos: input.getPos(),
            })
        } else {
            if (halfLink.srcSocket != null) {
                addLink({
                    srcSocket: halfLink.srcSocket,
                    dstSocket: input,
                })
            }
            linkDestroy()
        }
    }

    function linkOutput(output) {
        if (halfLink == null) {
            halfLink = compZenoHalfLink.createObject(sceneRect, {
                scene: thisScene,
                srcSocket: output,
                dstSocket: null,
                mousePos: output.getPos(),
            })
        } else {
            if (halfLink.dstSocket != null) {
                addLink({
                    srcSocket: output,
                    dstSocket: halfLink.dstSocket,
                })
            }
            linkDestroy()
        }
    }

    function linkDestroy() {
        if (halfLink != null) {
            halfLink.destroy()
            halfLink = null
        }
    }

    function mousePosition(mpos) {
        if (halfLink != null) {
            halfLink.mousePos = mpos
        }
    }

    function showAddNodeMenu(position) {
        if (position != null) {
            addNodeMenu.x = position.x
            addNodeMenu.y = position.y
            addNodeMenu.visible = true
        } else {
            addNodeMenu.visible = false
        }
    }

    Item {
        id: sceneRect
        x: 0
        y: 0
        scale: 1

        Component {
            id: compZenoNode
            ZenoNode {}
        }

        Component {
            id: compZenoLink
            ZenoLink {}
        }

        Component {
            id: compZenoHalfLink
            ZenoHalfLink {}
        }

        ColumnLayout {
            id: addNodeMenu
            visible: false
            spacing: 0
            z: 5

            Repeater {
                model: thisScene.collection.descs

                Button {
                    text: modelData.kind
                    Layout.fillWidth: true
                    height: 4

                    contentItem: Text {
                        text: parent.text
                        color: parent.hovered ? '#cc6' : '#333'
                    }

                    background: Rectangle {
                        color: parent.hovered ? '#333' : '#699'
                        border.color: parent.hovered ? '#699' : '#333'
                        border.width: 2
                        radius: 4
                    }

                    onClicked: {
                        addNodeByName(text, Qt.point(addNodeMenu.x, addNodeMenu.y))
                        addNodeMenu.visible = false
                    }
                }
            }
        }

        MouseArea {
            id: mouseArea
            width: 100000
            height: 100000
            x: -width / 2
            y: -height / 2

            hoverEnabled: true
            acceptedButtons: Qt.LeftButton

            onClicked: {
                if (!thisScene.focus)
                    thisScene.focus = true
                thisScene.showAddNodeMenu(null)
                if (halfLink != null) {
                    thisScene.linkDestroy()
                } else {
                    thisScene.doSelect(null)
                }
            }

            onPositionChanged: {
                if (!thisScene.focus)
                    thisScene.focus = true
                var mpos = Qt.point(mouse.x + x, mouse.y + y)
                thisScene.mousePosition(mpos)
            }
        }

        MouseArea {
            width: mouseArea.width
            height: mouseArea.height
            x: mouseArea.x
            y: mouseArea.y

            acceptedButtons: Qt.MiddleButton
            drag.target: parent

            onWheel: {
                var new_scale = sceneRect.scale
                if (wheel.angleDelta.y > 0) {
                    new_scale *= 1.2
                } else if (wheel.angleDelta.y < 0) {
                    new_scale /= 1.2
                }
                sceneRect.x += (wheel.x + x) * (sceneRect.scale - new_scale)
                sceneRect.y += (wheel.y + y) * (sceneRect.scale - new_scale)
                sceneRect.scale = new_scale
            }
        }

        MouseArea {
            width: mouseArea.width
            height: mouseArea.height
            x: mouseArea.x
            y: mouseArea.y

            acceptedButtons: Qt.RightButton

            onClicked: {
                var mpos = Qt.point(mouse.x + x, mouse.y + y)
                thisScene.showAddNodeMenu(mpos)
            }
        }
    }

    Keys.onDeletePressed: {
        deleteSelection()
    }

    Component.onCompleted: {
        thisScene.addNode({
            kind: 'readobj',
            name: 'readobj1',
            x: 64,
            y: 128,
        })
        thisScene.addNode({
            kind: 'transform',
            name: 'transform1',
            x: 352,
            y: 64,
        })
    }
}