#!/usr/bin/python

from __future__ import division

import signal

import txros
from txros import util

from twisted.internet import defer, reactor

import boat_scripting


@util.cancellableInlineCallbacks
def main(nh):
    boat = yield boat_scripting.get_boat(nh)
    
    yield boat.deploy_hydrophone()

@util.cancellableInlineCallbacks
def _main():
    try:
    	nh = txros.NodeHandle.from_argv('deploy_hydrophone', anonymous=True)
        yield main(nh)
    except Exception:
        traceback.print_exc()
    finally:
        reactor.stop()

def _start():
    signal.signal(signal.SIGINT, lambda signum, frame: reactor.callFromThread(task.cancel))
    task = _main().addErrback(lambda fail: fail.trap(defer.CancelledError))

if __name__ == '__main__':
    reactor.callWhenRunning(_start)
    reactor.run()
