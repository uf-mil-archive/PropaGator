 # traptest.sh

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

alias sim="rosrun boat_sim sim"
alias vis="rosrun boat_sim path_visualizer.py"

alias azi="roslaunch azi_drive run_azi.launch"
alias run="roslaunch boat_launch run.launch"
alias nav="roslaunch boat_launch navigation.launch"
alias lidar="roslaunch lidar_vision lidar.launch"

function controller(){
#sudo xboxdrv --detach-kernel-driver & 
roslaunch boat_launch controller.launch port:=$1
}

function core(){
echo "ROSCORE STARTED" 
echo "KILL HANDLER STARTED" 
rosrun kill_handling kill_master &
roscore 
}

function mission(){
echo "rosrun mission_core run_missions boat_launch.mission.$1"
rosrun mission_core run_missions boat_launch.mission.$1

}

