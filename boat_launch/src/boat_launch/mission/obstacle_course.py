#!/usr/bin/python
import boat_scripting
import numpy as np
from uf_common.orientation_helpers import xyz_array, quat_to_rotvec
from txros import util
from boat_launch.mission import move_on_line

@util.cancellableInlineCallbacks
def main(nh, entrance='x', exit='1'):
    boat = yield boat_scripting.get_boat(nh)

    try:
        boat.switch_path_planner('a_star_rpp')
        while True:
            gates = yield boat.get_gates()
            if len(gates) == 0:
                print 'No gates detected'
                continue

            current_boat_pos = boat.odom.position
            yaw = quat_to_rotvec(boat.odom.orientation)[2] % np.pi

            # Zip up numpy positions in enu - boat_pos
            gate_pos = [xyz_array(g.position) - current_boat_pos for g in gates]
            gates = zip(gate_pos, gates)

            # Filter out yaw 


            gate = None
            if len(gates) > 0:
                gate = min(gates, key = lambda x: np.linalg.norm(x[0]))
            else:
                gate = gates[0]

            #print gate

            # Translate back to enu
            gate_pos = gate[0] + current_boat_pos
            gate_orientation = gate[1].yaw + np.pi/2


            if abs(gate_orientation - yaw) < np.pi / 2:
                yield move_on_line.main(nh, gate_pos, np.array([0, 0, gate_orientation]))
            else:
                yield move_on_line.main(nh, gate_pos, np.array([0, 0, gate_orientation]), flip=True)


    finally:
        boat.default_state()