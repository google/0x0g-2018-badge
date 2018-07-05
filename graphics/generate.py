#!/usr/bin/python
#
# Copyright 2018 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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

RESOLUTION = 50
BOARD_RADIUS = 30
CENTER = (0, 0)

WHITE_SIDE_INNER_RADIUS = 24

LED_CIRCLE_RADIUS = 28 #mm
LED_SEGMENTS = 8
SEGMENT_WIDTH = 1 #mm
SEGMENT_DEG_HALF = 0.5
SEGMENT_DEG_START = 22.5-90

DOTTED_SEGMENTS = 100
DOTTED_RADIUS = 23

def pointOnCircleDegrees(center, radius, angle):
    return pointOnCircle(center, radius, radians(angle))

def pointOnCircle(center, radius, angle):
    x = center[0] + (radius * cos(angle))
    y = center[1] + (radius * sin(angle))
    return (x, y)

def interpolateArc(center, radius, startAngle, endAngle, resolution):
    degressPerThingy = endAngle-startAngle
    out = []
    for x in range(resolution):
        out.append(pointOnCircleDegrees(center, radius, startAngle + (float(degressPerThingy) * (x/float(resolution)))))
    return out

def line(start, end, layer="F.Fab", width=0.15):
    global out
    out += '  (fp_line (start {} {}) (end {} {}) (layer {}) (width {}))\n'.format(
        start[0], start[1], end[0], end[1], layer, width)

def polygon(points, layer="F.Fab", width=0.15):
    global out
    out += '\n  (fp_poly\n    (pts \n'
    for point in points:
        out += '      (xy {} {})\n'.format(point[0], point[1])
    out += '    )\n    (layer {})\n    (width {})\n  )'.format(layer, width)

out += '  (fp_circle (center 0 0) (end {} 0) (layer F.Fab) (width 0.15))\n'.format(BOARD_RADIUS)

# LED guides + outer rim
degreesPerSegment = 360/float(LED_SEGMENTS)
for x in xrange(LED_SEGMENTS):
    #top guide
    sys.stderr.write('Segment {}: {}\n'.format(x+1, 10*(SEGMENT_DEG_START + (degreesPerSegment*x) - 90)))
    lBndTop = pointOnCircleDegrees(CENTER, LED_CIRCLE_RADIUS-SEGMENT_WIDTH, SEGMENT_DEG_START + degreesPerSegment*x)
    rBndTop = pointOnCircleDegrees(CENTER, LED_CIRCLE_RADIUS+SEGMENT_WIDTH, SEGMENT_DEG_START + degreesPerSegment*x)
    #sys.stderr.write('Y{}{} = {}\n'.format(n, x+1, lBndTop[1]))
    line(lBndTop, rBndTop)
    #line guide bottom
    topCenter = pointOnCircleDegrees(CENTER, LED_CIRCLE_RADIUS, SEGMENT_DEG_START-SEGMENT_DEG_HALF + degreesPerSegment*x)
    botCenter = pointOnCircleDegrees(CENTER, LED_CIRCLE_RADIUS, SEGMENT_DEG_START+SEGMENT_DEG_HALF + degreesPerSegment*x)
    line(topCenter, botCenter)
    #outer rim
    offset = degreesPerSegment/4
    start = SEGMENT_DEG_START + degreesPerSegment*x + offset
    stop = SEGMENT_DEG_START + degreesPerSegment*(x+1) - offset
    outerPoints = interpolateArc(CENTER, BOARD_RADIUS, start, stop, RESOLUTION)
    innerPoints = interpolateArc(CENTER, WHITE_SIDE_INNER_RADIUS, start, stop, RESOLUTION)
    polygon(outerPoints + innerPoints[::-1], "F.SilkS")

# dotted-semi circle
DOTTED_SEGMENTS /= 2
degreesPerSegment = 360/float(DOTTED_SEGMENTS)
for n in xrange(DOTTED_SEGMENTS):
    if (n % 2) == 1:
        continue
    points = interpolateArc(CENTER, DOTTED_RADIUS, degreesPerSegment*n, degreesPerSegment*(n+1), RESOLUTION)
    for x in range(len(points)):
        if (x+1) < len(points):
            line(points[x], points[x+1], 'F.SilkS')
    #sys.stderr.write(str(x) + '\n')

out += ')\n'

print out
