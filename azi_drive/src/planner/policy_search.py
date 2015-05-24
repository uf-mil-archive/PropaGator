import numpy as np


class Policy_Search(object):
    def __init__(self):
        ''' Parameterize the policy search in a few state variables with good start guesses
        '''
        return NotImplemented

    def policy_cost(self, policy):
        '''Determine the cost of a particular policy

        Ideas: 
            - integral(u) + time
            - integral(u) + integral(crossp(pt[n], pt[n - 1] in frame pt[n-2]))
                (Total curvature, or rather total angle change -- curl would be better than this)

        '''
        return NotImplemented

    def compute_policy(self, start, end):
        '''Compute a locally optimal control policy for a start and end state'''
        return NotImplemented

if __name__ == '__main__':
    ps = Policy_Search()
