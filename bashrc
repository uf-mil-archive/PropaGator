# traptest.sh

alias sim="rosrun boat_sim sim"
alias azi="roslaunch azi_drive run_azi.launch"
alias run="roslaunch boat_launch run.launch"
alias nav="roslaunch boat_launch navigation.launch"
alias lidar="roslaunch lidar_vision lidar.launch"
alias master_launch_boat="roslaunch boat_launch master_launch_boat.launch"
alias xbox="sudo xboxdrv --detach-kernel-driver"

function controller(){
#sudo xboxdrv --detach-kernel-driver & 
roslaunch boat_launch controller.launch port:=$1
}

function master_launch_remote(){
for i in `seq 1 20`;
do
	echo "LAUNCH XBOXDRV IN SEPARATE TERMINAL WITH 'xbox'"
done
roslaunch boat_launch master_launch_remote.launch
}

function core(){
echo "ROSCORE STARTED" 
echo "KILL HANDLER STARTED" 
rosrun kill_handling kill_master &
roscore 
}

function boat_mission(){
echo "rosrun mission_core run_missions boat_launch.mission.$1"
rosrun mission_core run_missions boat_launch.mission.$1

}

function h(){
  boat_mission station_hold
}

