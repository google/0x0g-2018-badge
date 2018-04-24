#!/usr/bin/python
from math import cos, sin, pi, radians
import time
import sys

out = '''(module {0} (layer F.Cu) (tedit {1:8X})
  (fp_text reference REF** (at 0 8) (layer F.SilkS)
    (effects (font (size 1 1) (thickness 0.15)))
  )
  (fp_text value ZEROG-GRAPHIC (at 0 0) (layer F.Fab)
    (effects (font (size 1 1) (thickness 0.15)))
  )
'''.format("ZEROG-GRAPHIC", int(time.time()))

LED_CIRCLE_RADIUS = 28 #mm
LED_SEGMENTS = 4
SEGMENT_WIDTH = 1 #mm
CENTER = (0, 0)
SEGMENT_DEG_HALF = 0.5

def pointOnCircleDegrees(center, radius, angle):
    return pointOnCircle(center, radius, radians(angle))

def pointOnCircle(center, radius, angle):
    x = center[0] + (radius * cos(angle))
    y = center[1] + (radius * sin(angle))
    return (x, y)

def line(start, end, layer="F.Fab", width=0.15):
    global out
    out += '  (fp_line (start {} {}) (end {} {}) (layer {}) (width {}))\n'.format(
        start[0], start[1], end[0], end[1], layer, width)

out += '  (fp_circle (center 0 0) (end 30 0) (layer F.Fab) (width 0.15))\n'

for n in xrange(2):
    if n == 0:
        SEGMENT_DEG_START = 30-90
        SEGMENT_DEG_STOP = 135-90
    else:
        SEGMENT_DEG_START = 270-30
        SEGMENT_DEG_STOP = 270-135
    degreesPerSegment = (SEGMENT_DEG_STOP-SEGMENT_DEG_START)/float(LED_SEGMENTS)
    for x in xrange(LED_SEGMENTS):
        #top guide
        sys.stderr.write('Segment {} of side {}: {}\n'.format(x+1, n, 10*(SEGMENT_DEG_START + (degreesPerSegment*x) - 90)))
        lBndTop = pointOnCircleDegrees(CENTER, LED_CIRCLE_RADIUS-SEGMENT_WIDTH, SEGMENT_DEG_START + degreesPerSegment*x)
        rBndTop = pointOnCircleDegrees(CENTER, LED_CIRCLE_RADIUS+SEGMENT_WIDTH, SEGMENT_DEG_START + degreesPerSegment*x)
        #sys.stderr.write('Y{}{} = {}\n'.format(n, x+1, lBndTop[1]))
        line(lBndTop, rBndTop)
        #line guide bottom
        topCenter = pointOnCircleDegrees(CENTER, LED_CIRCLE_RADIUS, SEGMENT_DEG_START-SEGMENT_DEG_HALF + degreesPerSegment*x)
        botCenter = pointOnCircleDegrees(CENTER, LED_CIRCLE_RADIUS, SEGMENT_DEG_START+SEGMENT_DEG_HALF + degreesPerSegment*x)
        line(topCenter, botCenter)

out += ')\n'

print out
