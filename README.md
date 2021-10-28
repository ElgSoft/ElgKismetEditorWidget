# ElgKismetEditorWidget
 This is an Unreal Engine 4.27 editor only plugin to make it possible to use Editor Utility Widgets inside the Blueprint Editor.
 <br>
 The plugin add basic Graph events like **OnGraphCompiled**, **OnNodeSelectionChanged**, **OnFocusGraphChanged**.<br> 
 It also add nodes to create/edit/remove Variables, Functions, Macros, EventDispatchers, EventGraph.

![Kew_SelectWindow](http://elgsoft.com/Images/KEW/Kew_SelectWindow.png)<br>

It support all functionlity needed to recreate the MyBlueprint window inside Editor Utility Widget.<br>
![KEW_myBP](http://elgsoft.com/Images/KEW/KEW_myBP.png)<br>


## Setup

To open Editor widgets inside the Blueprint Graph you can use the EditorWidgetViewer window and select any EditorWidget and it will be open inside that widget.

![Window to select Editor Widget](http://elgsoft.com/Images/KEW/KEW_EditorWidgetViewer_Window.png)<br>
The other way is to register the Editor Widgets in Kismet Editor Widgets section in Project Settings...
Add the Tab label, description and alignment with in the tab.

![Setup Kismet Editor Widgets](http://elgsoft.com/Images/KEW/KEW_Settings.png)<br>
This will make that Editor Widget get it's own item in the Window list and you can dock the Editor Utility Widget inside the graph and it will be saved.<br>
![Select window](http://elgsoft.com/Images/KEW/KEW_Window_DropDown.png)<br>


## BPEditor Context
Thru the KismetEditorWidget Context (BPEditor) you can open built in windows like Native Code Generation Tool, Find, Find and Replace and Class Defaults.
Select things in the MyBlueprint window or clear any selection.
Log Messages to the compiler log and more.

Example can be found in **EW_BPEditor**.

## Events
You can bind events so you can react when the graph change or node selection change.

![Bind graph events](http://elgsoft.com/Images/KEW/KEW_events.png)<br>
Events that are exposed in the plugin are:<br>
**OnEditorRefresh**<br>
**OnFocusGraphChanged** - changed tab<br>
**OnGraphChanged**<br>
**OnGraphCompiled**<br>
**OnNodeSelectionChanged**<br>
<br>

## Drag and drop

The plugin support drag and drop, so you can drop a function from a Editor Widget to a graph, if you drop it on a pin it will try to connect the nodes.
Variables can be drop as Get or Set node.

The drag and drop is setup the same way you would do with regulare widgets.
![Drag detected setup](http://elgsoft.com/Images/KEW/kew_dragdrop.png)<br>
Object that support drag and drop to the graph/nodes/pins:<br>
**Variable**<br>
**VariablePin**<br>
**Function**<br>
**Macro**<br>
**LocalVariable**<br>
**EventDispatcher**<br>
**InterfaceFunction**<br>
**WidgetVariable**<br>

## Nodes
You can get all the nodes in the blueprint/graph or what node(s) that are selected.
Or you can get inspect a node and check what type of K2Node it is, it's member info (class and method name) or info about how many pin(s) it has and what type of pin it is. With pins you can break any connection or connect pin on different nodes.
A node is represented by a **ElgBESGraphNode** object. And each pin on a node is represented by a **ElgBESGraphPin** object.

Example can be found in **EW_NodeList** and **EW_NodeInfo**.

![kew_nodeinfo](http://elgsoft.com/Images/KEW/kew_nodeinfo.png)<br>

### BPEditor Context nodes
From the BPEditor context you can get event when the selection of nodes change, get the currently selected nodes, all the nodes in the blueprint/specific graph/function or select nodes.

![img](http://elgsoft.com/Images/KEW/kew_node_context.png)<br>

### ElgBESGraphNode nodes
With ElgBESGraphNode object you can select it, check if the node is connected to anything, position in the graph, the member info, the pin **ElgBESGraphPin** objects.<br >
![kew_node_nodes](http://elgsoft.com/Images/KEW/kew_node_nodes.png)<br>
![kew_bes_base](http://elgsoft.com/Images/KEW/kew_bes_base.png)<br>

### ElgBESGraphPin nodes
With ElgBESGraphPin object you get what type it is, if it's connected, break it's link, link it to another pin or if it's a data pin promote it to a variable.
![kew_node_pin](http://elgsoft.com/Images/KEW/kew_node_pin.png)<br>

## Variables
You can create, modify or remove variables in the blueprint. A member variable is represented by a **ElgBESGraphVariableMember** object.

Example can be found in **EW_VariableList** and **EW_VarInheritedList**.

### BPEditor Context nodes
From the BPEditor context you can get variables in the blueprint, create new variables or remove one.
![kew_var_context](http://elgsoft.com/Images/KEW/kew_var_context.png)<br>

### ElgBESGraphVariableMember nodes
With ElgBESGraphVariableMember object you can remove, change name, duplicate, change type, set replicationType etc.
![kew_var_node_set](http://elgsoft.com/Images/KEW/kew_var_node_set.png)<br>
![kew_var_node_get](http://elgsoft.com/Images/KEW/kew_var_node_get.png)<br>

## Functions
The plugin expose functionality to work with the functions in a blueprint.
You can get/set the Category, if the function is pure, if it should be deprecated and more.
Or add/remove/edit local variables or it's pins.

A function is represented by a ElgBESGraphFunction object. Each pin in the function is is represented by a **ElgBESGraphFunctionPin** object. Local variables in a function is represented by a **ElgBESGraphVariableLocal** object.

Example can be found in **EW_FunctionList**.

### BPEditor Context nodes
From the BPEditor context you can get functions in the blueprint, create new function or override functions one.
![kew_func_context](http://elgsoft.com/Images/KEW/kew_func_context.png)<br>

### ElgBESGraphFunction nodes
With a ElgBESGraphFunction object you can create local vaiable, get local variables, get or add pins to the function.
![kew_func_nodes](http://elgsoft.com/Images/KEW/kew_func_nodes.png)<br>
![kew_bes_base](http://elgsoft.com/Images/KEW/kew_bes_base.png)<br>

### ElgBESGraphVariableLocal nodes
With the ElgBESGraphVariableLocal object you can remove, change the name, change type, duplicate, set category and set the default value. You can also get all the basic info about the variable, the default value, category, data type and icon.
![kew_local_var_nodes](http://elgsoft.com/Images/KEW/kew_local_var_nodes.png)<br>
![kew_var_base](http://elgsoft.com/Images/KEW/kew_var_base.png)<br>
![kew_bes_base](http://elgsoft.com/Images/KEW/kew_bes_base.png)<br>

### ElgBESGraphFunctionPin nodes

You can add new Input/Output pins, or access existing pin to change the order the pins are listed, change the name, change data type, promote it to a variable.<br>
![kew_func_pin_nodes](http://elgsoft.com/Images/KEW/kew_func_pin_nodes.png)<br>
![kew_bes_base_pin_edit](http://elgsoft.com/Images/KEW/kew_bes_base_pin_edit.png)<br>
![kew_bes_base](http://elgsoft.com/Images/KEW/kew_bes_base.png)<br>


## Interface
The plugin give you access to quary what interfaces the blueprint has or you can add or remove interface.
The interface is represented by a **ElgBESGraphInterface** object, and each function the interface has is a **ElgBESGraphInterfaceFunction**, and each pin on the function are **ElgBESGraphFunctionPin** objects.

Example can be found in **EW_InterfaceList**.

### BPEditor Context nodes
Nodes to add/remove a interface, and get implemented interfaces objects **ElgBESGraphInterface**.
![kew_interface_context](http://elgsoft.com/Images/KEW/kew_interface_context.png)<br>

### ElgBESGraphInterface nodes
From ElgBESGraphInterface object you can get what functions, **ElgBESGraphInterfaceFunction**,the interface has, get some info about the function or check if the function has been implemented in the blueprint or not.
![kew_interface_nodes](http://elgsoft.com/Images/KEW/kew_interface_nodes.png)<br>

### ElgBESGraphInterfaceFunction nodes
From ElgBESGraphInterfaceFunction object you have nodes to implement the function/event, open the function. Or you can use it to get access to the pins of the function, ElgBESGraphFunctionPin.
As the functions are inherited from from the interface you can't add/remove pins or rename the function.
![kew_interface_function](http://elgsoft.com/Images/KEW/kew_interface_function.png)<br>

### ElgBESGraphFunctionPin nodes
On the EgBESGraphFunctionPin object you can get the name, direction, data type or promote it to a variable.
The pins are read only as they are inherited from the interface.<br>
![kew_macro_context](http://elgsoft.com/Images/KEW/kew_interface_pin.png)<br>

## Macros
The plugin give you access to quary what macros the blueprint has or you can add or remove macros.
The macro is represented by a **ElgBESGraphMacro** object, and each pin on the macro are **ElgBESGraphMacroPin** objects.

Example can be found in **EW_MacroList**.

### BPEditor Context nodes
From the BPEditor context you can create new macros or get access to existing macros in the blueprint.
![kew_macro_context](http://elgsoft.com/Images/KEW/kew_macro_context.png)<br>

### ElgBESGraphMacro nodes
From ElgBESGraphMacro object you can open the macro, duplicate it or remove it. Or you can use it to get access to the pins of the macro, ElgBESGraphMacroPin.
![kew_macro_nodes](http://elgsoft.com/Images/KEW/kew_macro_nodes.png)<br>
![kew_bes_base](http://elgsoft.com/Images/KEW/kew_bes_base.png)<br>

### ElgBESGraphMacroPin nodes
On the ElgBESGraphMacroPin object you can get or set the name, direction, data type.
![kew_macro_pin](http://elgsoft.com/Images/KEW/kew_macro_pin.png)<br>
![kew_bes_base](http://elgsoft.com/Images/KEW/kew_bes_base.png)<br>

## UberGraphs
The plugin give you access to UberGraphs (EventGraph) the blueprint has or you can add or remove macros.
The macro is represented by a **ElgBESGraphUber** object.

Example can be found in **EW_UberGrapsList**.

### BPEditor Context nodes
From the BPEditor context you can create new UberGraphs or get access to existing graphs in the blueprint.
![kew_uber_context](http://elgsoft.com/Images/KEW/kew_uber_context.png)<br>

### ElgBESGraphUber nodes
From ElgBESGraphUber object you can open the graph, remove it (as long as it's not the default Event Graph) or get all the nodes that it has.<br>
![kew_uber_nodes](http://elgsoft.com/Images/KEW/kew_uber_nodes.png)<br>
![kew_event_context](http://elgsoft.com/Images/KEW/kew_bes_base.png)<br>

## EventDispatcher
The plugin give you access to EventDispatchers the blueprint has or you can add or remove EventDispatchers.
The EventDispatcher is represented by a ElgBESGraphEventDispatcher object and each pin are a ElgBESGraphEventDispatcherPin

Example can be found in **EW_EventDispatcherList**.

### BPEditor Context nodes
From the BPEditor context you can create new EventDispatcher or get access to existing EventDispatcher in the blueprint.
![kew_event_context](http://elgsoft.com/Images/KEW/kew_event_context.png)<br>

### ElgBESGraphEventDispatcher nodes
From ElgBESGraphEventDispatcher object you can open the graph, remove it (as long as it's not the default Event Graph).
![kew_event_nodes](http://elgsoft.com/Images/KEW/kew_event_nodes.png)<br>
![kew_bes_base](http://elgsoft.com/Images/KEW/kew_bes_base.png)<br>

### ElgBESGraphEventDispatcherPin nodes
From ElgBESGraphEventDispatcherPin object you can change the pin order, remove, change name or change data type.
![kew_bes_base_pin_edit](http://elgsoft.com/Images/KEW/kew_bes_base_pin_edit.png)<br>


## Widgets
The plugin give you access to Widgets that a WidgetBlueprint or EditorUtilityWidget has. You can
Widget is represented by a **ElgBESGraphWidget** object and if exposed as a variable is a **ElgBESGraphWidgetVariable**.

Example can be found in **EW_WidgetTree**.

### BPEditor Context nodes
From the BPEditor context you can access any widgets that has been placed in the designer mode.

![kew_widget_context](http://elgsoft.com/Images/KEW/kew_widget_context.png)<br>

### ElgBESGraphWidget nodes
From ElgBESGraphWidget object check if the widget is exposed as a variable or set that if it should be a variable, same as tick the "Is Variable" inside the Designer.

![kew_widget_nodes](http://elgsoft.com/Images/KEW/kew_widget_nodes.png)<br>
![kew_bes_base](http://elgsoft.com/Images/KEW/kew_bes_base.png)<br>

### ElgBESGraphWidgetVariable nodes
From ElgBESGraphWidgetVariable object you can set the category, get the data type.

![kew_bes_base_var](http://elgsoft.com/Images/KEW/kew_bes_base_var.png)<br>

## PinTypeSelectorWidget
The plugin add a new widget that can be used to select pin data type. Comes with Event for when the type change and nodes to get and set the data type.

![kew_pin_selector](http://elgsoft.com/Images/KEW/kew_pin_selector.png)<br>
![kew_pin_selector_nodes](http://elgsoft.com/Images/KEW/kew_pin_selector_nodes.png)<br>
