
## Main mission README

This will show you how to write your mission in the proper format for addition into the main mission file. 

There are a few layers the mission files. All functions for moving the boat, subscribing, publishing, or doing any lower level control functions take place in the file called __init__.py in the boat_scripting file. To see examples of how to add publishers and subscribers to the boat, see the _init__ function in the Boat class. Then in the space below create you function to access the topics you need. Look through the many funtions already there for examples

Normally we subscribe to a topic and a function is called everytime a message comes into that topic. This way of subscribing gives us the ability to only check for new messages when we want to and use those messages accordingly. 

To publish to topic:

    self._your_publisher.publish(msg)

To subsribe and send msg back to you mission file:

    msg = yield self._triangle_pos.get_next_message()
    defer.returnValue(msg)

It is in your mission file that you will decide what to do with these messages. Or at least that is what is advised. Think of the boat class a server to access messages. Then in your mission file you atuall ymove the boat depending on these messages

There are two major differences in how we do a mission file that is used in the main mission compared to when we are testing:

    1. Do not subsribe to anything in your mission file, messages will be revieved and sent out through the master node above
    2. Do not create your own boat from the boat_scripting class, rather let it be a parameter in the main funtion -> def main(nh, boat)

Look at the find_cross mission in boat_launch->src->mission for a good example

