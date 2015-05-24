from __future__ import division
import numpy as np

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
    def hermite_spline(self, *pts, time):
        '''Compute the continuous hermite interpolation through some 
            set of points in  a state space
        '''
        return NotImplemented

    @classmethod
    def hermite_interpolate(self, start_pt, start_m, end_pt, end_m, t):
        '''hermite_interpolate(start_pt, start_m, end_pt, end_m, t):

            m->derivative at that point [1]

        Problems inherent:
            - Tough to limit 2nd derivative without heavy math
            - Limiting angular acceleration? Could spline in theta space, 
                but then we have a really wacky interdependence problem
            - This always takes a curved path between two points, while we 
                can eventually start taking a direct path
        '''

        start_pt_term = ((2 * (t**3)) - (3 * (t**2)) + 1) * start_pt
        start_dv_term = ((t**3) - (2 * (t**2)) + t) * start_m

        end_pt_term = ((-2 * (t**3)) + (3 * (t**2))) * end_pt
        end_dv_term = ((t**3) - (t**2)) * end_m

        return start_pt_term + start_dv_term + end_pt_term + end_dv_term


if __name__ == '__main__':
    start_pt = np.array([0.0, 0.0])
    start_m = np.array([1.0, 1.0])

    end_pt = np.array([7.0, 7.0])

    for t in np.arange(0, 1.05, 0.05):
        print Hermite.hermite_interpolate(start_pt, start_m, end_pt, np.array([0.0, 0.0]), t)
