function 0w(){
rostopic pub /wrench geometry_msgs/WrenchStamped "header:
  seq: 0
  stamp:
    secs: 0
    nsecs: 0
  frame_id: ''
wrench:
  force:
    x: 0.0
    y: 0.0
    z: 0.0
  torque:
    x: 0.0
    y: 0.0
    z: 0.0"
}

function h(){
    roscd boat_launch/src/boat_launch/mission/
    python -c 'import station_hold; station_hold.hold_at_current_pos()' 
}

alias sim="rosrun boatsim sim"
alias vis="rosrun boatsim path_visualizer.py"
alias azi="roslaunch azi_drive run.launch"
alias controller="roslaunch boat_launch controller.launch"
alias run="roslaunch boat_launch run.launch"

function core(){
roscore &
echo ""
echo "ROSCORE STARTED" 
echo ""
echo ""
echo "KILL HANDLER STARTED" 
echo ""
rosrun kill_handling kill_master 
}