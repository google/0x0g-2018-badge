#!/usr/bin/python
import svgwrite
from math import cos, sin, pi, radians

LED_CIRCLE_RADIUS = 56 #mm
LED_SEGMENTS = 5
SEGMENT_WIDTH = 2 #mm
CENTER = (100, 100)
SEGMENT_DEG_HALF = 2.5

def pointOnCircleDegrees(center, radius, angle):
    return pointOnCircle(center, radius, radians(angle))

def pointOnCircle(center, radius, angle):
    x = center[0] + (radius * cos(angle))
    y = center[1] + (radius * sin(angle))
    return (x, y)

dwg = svgwrite.Drawing('generated.svg', profile='tiny')
dwg.add(dwg.circle(center=CENTER, r=60, fill='none', stroke=svgwrite.rgb(10, 10, 16, '%')))

for n in xrange(2):
    if n == 0:
        SEGMENT_DEG_START = 30-90
        SEGMENT_DEG_STOP = 135-90
    else:
        SEGMENT_DEG_START = 270-30
        SEGMENT_DEG_STOP = 270-135
    degreesPerSegment = (SEGMENT_DEG_STOP-SEGMENT_DEG_START)/LED_SEGMENTS
    for x in xrange(LED_SEGMENTS):
        #top guide
        lBndTop = pointOnCircleDegrees(CENTER, LED_CIRCLE_RADIUS-SEGMENT_WIDTH, SEGMENT_DEG_START-SEGMENT_DEG_HALF + degreesPerSegment*x)
        rBndTop = pointOnCircleDegrees(CENTER, LED_CIRCLE_RADIUS, SEGMENT_DEG_START-SEGMENT_DEG_HALF + degreesPerSegment*x)
        dwg.add(dwg.line(lBndTop, rBndTop, stroke=svgwrite.rgb(10, 10, 16, '%'), layer='F.Fab'))
        #top guide
        lBndBottom = pointOnCircleDegrees(CENTER, LED_CIRCLE_RADIUS-SEGMENT_WIDTH, SEGMENT_DEG_START+SEGMENT_DEG_HALF + degreesPerSegment*x)
        rBndBottom = pointOnCircleDegrees(CENTER, LED_CIRCLE_RADIUS, SEGMENT_DEG_START+SEGMENT_DEG_HALF + degreesPerSegment*x)
        dwg.add(dwg.line(lBndBottom, rBndBottom, stroke=svgwrite.rgb(10, 10, 16, '%'), layer='F.Fab'))
        #line guide top
        topCenter = pointOnCircleDegrees(CENTER, LED_CIRCLE_RADIUS, SEGMENT_DEG_START-SEGMENT_DEG_HALF + degreesPerSegment*x)
        botCenter = pointOnCircleDegrees(CENTER, LED_CIRCLE_RADIUS, SEGMENT_DEG_START+SEGMENT_DEG_HALF + degreesPerSegment*x)
        dwg.add(dwg.line(topCenter, botCenter, stroke=svgwrite.rgb(10, 10, 16, '%'), layer='F.Fab'))
        #line guide bottom
        topCenter = pointOnCircleDegrees(CENTER, LED_CIRCLE_RADIUS-SEGMENT_WIDTH, SEGMENT_DEG_START-SEGMENT_DEG_HALF + degreesPerSegment*x)
        botCenter = pointOnCircleDegrees(CENTER, LED_CIRCLE_RADIUS-SEGMENT_WIDTH, SEGMENT_DEG_START+SEGMENT_DEG_HALF + degreesPerSegment*x)
        dwg.add(dwg.line(topCenter, botCenter, stroke=svgwrite.rgb(10, 10, 16, '%'), layer='F.Fab'))


dwg.save()
