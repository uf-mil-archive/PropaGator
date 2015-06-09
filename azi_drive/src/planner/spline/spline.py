from __future__ import division
import numpy as np


class Waypoint(object):
    def __init__(self, position, velocity=(0.0, 0.0)):
        self.position = np.array(position, np.float32)
        self.velocity = np.array(velocity, np.float32)
        assert len(self.position.shape) == 1, "Waypoint position should be a vector"
        assert len(self.velocity.shape) == 1, "Waypoint velocity should be a vector"
        assert self.velocity.shape == self.position.shape, "Position and velocity should have the same shape"


class Spline(object):
    def __init__(self, pts):        
        # spline = 
        for pt in pts:
            pass


class Hermite(object):
    '''Cube hermite interpolator

    Bibliography:
        [1] http://en.wikipedia.org/wiki/Cubic_Hermite_spline

    Ideas:
        Optimization:
            Constraints:
                dx in world < f1(theta)
                dy in world < f2(theta)

                dtheta in world < f3(v_current)

                force_offset = drag(heading * v)
    '''

    @classmethod
    def spline(self, start_way, end_way, time_range=(0, 1), dx=0.01):
        '''spline(start_way, end_way, time_range=(0, 1), dx=0.01):
        Compute the continuous hermite interpolation through some 
            set of points in  a state space
        Need a way to encode speed, or the amount of time allowed to complete the path (Sub-sample?)
        '''
        _range = np.arange(time_range[0], time_range[1], dx)
        spline = np.zeros((len(_range), 2))
        for n, s in enumerate(_range):
            spline[n] = self.interpolate(start_way, end_way, s)

        return spline

    @classmethod
    def interpolate(self, start_way, end_way, s):
        '''hermite_interpolate(start_waypoint, end_waypoint, s):

            m->derivative at that point [1]

        Problems inherent:
            - Tough to limit 2nd derivative without heavy math
            - Limiting angular acceleration? Could spline in theta space, 
                but then we have a really wacky interdependence problem
            - This always takes a curved path between two points, while we 
                can eventually start taking a direct path
        '''

        start_pt_term = ((2 * (s**3)) - (3 * (s**2)) + 1) * start_way.position
        start_dv_term = ((s**3) - (2 * (s**2)) + s) * start_way.velocity

        end_pt_term = ((-2 * (s**3)) + (3 * (s**2))) * end_way.position
        end_dv_term = ((s**3) - (s**2)) * end_way.velocity

        return start_pt_term + start_dv_term + end_pt_term + end_dv_term

    @classmethod
    def spline_integral(self, spline):
        '''Compute the line integral along the path'''
        length = 0
        if len(spline) == 0:
            return 0.0

        prev_pt = spline[0]
        for current_point in spline:
            length += np.linalg.norm(current_point - prev_pt)
            prev_pt = current_point
        return length

    @classmethod
    def spline_max_q(self, spline, dx=0.01):
        '''Determine the maximum acceleration along the curve'''
        if len(spline) == 0:
            return np.inf

        d2s = np.diff(spline, 2, 0)  / (dx ** 2)  # 2nd order diff on spline
        d2s_magnitude = map(np.linalg.norm, d2s)  # Magnitude of acceleration at a point
        # print len(spline), dx
        return max(d2s_magnitude)

    @classmethod
    def spline_speed(self, spline, dx=0.01):
        if len(spline) == 0:
            return np.array([np.inf])
        velocities = np.diff(spline, 1, 0) / dx
        return velocities

