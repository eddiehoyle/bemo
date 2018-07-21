import pybemo as p
graph = p.Graph()

node = p.CustomNode()
graph.add(node)
graph.execute()

graph.output("path").connect(node.input("path"))

node0 = p.CustomNode()
node1 = p.CustomNode()
node2 = p.CustomNode()

group = p.Group()
group.add([node0, node1, node2])

group.getInputs()
# Result: [
#    <Plug(owner="node0", type="input", property="path")>,
#    <Plug(owner="node1", type="input", property="path")>,
#    <Plug(owner="node2", type="input", property="path")>,
#]

inputs0 = node0.getInputs()
# Result: {
#    "path": "",
#}
node0.getInput("path")
# Result: ""

# ---------------------------------------------------------------------------- #

# How do I create custom nodes?
# How does API know how to create custom node?

from pybemo import cmds
node = cmds.createNode("MyCustomNode")

# Plugins?

from pybemo import Node
from pybemo import PluginID
from pybemo import PluginManager
from pybemo import Plug

# Used to identify plugins
myCustomPluginID = PluginID(0x870FE)

# My node
class MyCustomNode(Node):
    """Everything here is allocated when a user runs:
    cmds.createNode("MyCustomNode")
    """
    def execute(self):
        """Entry point"""
        print "Hello!"
def header(self):
    """Description, icons, etc"""
    return {"name": "MyCustomNode",
            "description": "My custom node!",
            "icon": "res/myCustomIcon.png"}
def layout():
    """Plugs, input/output, etc"""
    return [Plug("path", Plug.Direction.Input, Plug.String),
            Plug("frame", Plug.Direction.Input, Plug.Int),
            Plug("image", Plug.Direction.Output, Plug.Int),]
def create():
    """Creator function"""
    return MyCustomNode()
def bemoInitialisePlugin():
    PluginManager.registerNode("MyCustomNode",
                               myCustomPluginID,
                               MyCustomNode::create,
                               MyCustomNode::header,
                               MyCustomNode::layout,)
