/*
 * moving_object.h
 *
 *  Created on: Oct 15, 2014
 *      Author: Alan J. Hamlet
 */

#ifndef MOVING_OBJECT_H_
#define MOVING_OBJECT_H_
#include <sensor_msgs/LaserScan.h>

const float pi = 3.14159;
const float clusterThreshold = 0.5;  // Max distance between 2 points in same cluster
const float segmentThreshold = 0.1;  // Max distance a point can be from a line segment and still belong
const float max_distance = 30;       // Max range of the lidar. Ranges above this threshold will be ignored.

typedef struct PointStruct{
	float X;
	float Y;
	bool occluded;
	struct PointStruct *NextPoint;
} PointStruct;
typedef PointStruct *Point;

// List of points forming a straight line in an object
typedef struct SegmentStruct{
	Point FirstPoint;
	Point LastPoint;
	int numPts;
	SegmentStruct *nextSegment;
} SegmentStruct;
typedef SegmentStruct *Segment;

// List of segments composing an object
typedef struct SegmentListStruct{
	unsigned int numSegments;
	Segment firstSegment;
	Segment lastSegment;
} SegmentListStruct;
typedef SegmentListStruct *SegmentList;

// Object definition
typedef struct ObjectStruct{
	SegmentList segmentList;
	double length;
	double width;
	int movingProb;
	Segment boundingBox;            // Not a segment, a point list specifying the corners of the object's bounding box
	struct ObjectStruct *prevObject;
	struct ObjectStruct *nextObject;
} ObjectStruct;
typedef ObjectStruct *Object;

// List of potentially moving objects
typedef struct ObjectListStruct{
	unsigned int numberOfObjects;
	Object firstObject;
	Object lastObject;
} ObjectListStruct;
typedef ObjectListStruct *ObjectList;

// Initializes a point struct
Point PointInit(){
	Point newPoint = NULL;
	newPoint = (Point)malloc(sizeof(PointStruct));
	newPoint->X = 0;
	newPoint->Y = 0;
	newPoint->NextPoint = NULL;
	return (newPoint);
}

// Initializes a segment struct
Segment SegmentInit(void){
	Segment newSegment = NULL;
	newSegment = (Segment)malloc(sizeof(SegmentStruct));
	newSegment->numPts = 0;
	newSegment->FirstPoint = NULL;
	newSegment->LastPoint = NULL;
	newSegment->nextSegment = NULL;
	return (newSegment);
}

// Creates a segment from scan ranges.
// Provide ranges, sizeof(ranges), first range angle, and the angle increment
Segment CreateSegment(float ranges[], int size, float startAngle, float angle_increment){
	Segment newSegment = SegmentInit();
	newSegment->numPts = size;
	for (int i = 0; i < size; i++){
		Point tempPoint = PointInit();
		tempPoint->X = ranges[i]*cos(startAngle + i*angle_increment);
		tempPoint->Y = ranges[i]*sin(startAngle + i*angle_increment);
		if(i == 0)
			newSegment->FirstPoint = tempPoint;
		else
			newSegment->LastPoint->NextPoint = tempPoint;
		newSegment->LastPoint = tempPoint;
		newSegment->LastPoint->NextPoint = NULL;
	}
	newSegment->nextSegment = NULL;
	return (newSegment);
}

// Initializes a SegmentList struct
SegmentList SegmentListInit(void){
	SegmentList newSegmentList = NULL;
	newSegmentList = (SegmentList)malloc(sizeof(SegmentListStruct));
	Segment newSegment = SegmentInit();
	newSegmentList->numSegments = 0;
	newSegmentList->firstSegment = newSegment;
	newSegmentList->lastSegment = newSegment;
	return (newSegmentList);
}

// Combines 2 segmentLists into 1
SegmentList MergeSegmentLists( SegmentList segmentList1, SegmentList segmentList2){
	SegmentList result = SegmentListInit();
	if (segmentList1->numSegments){
		result->numSegments = segmentList1->numSegments + segmentList2->numSegments;
		segmentList1->lastSegment->nextSegment = segmentList2->firstSegment;
		result->firstSegment = segmentList1->firstSegment;
		result->lastSegment = segmentList2->lastSegment;
		return (result);
	}
	else
		return (segmentList2);
}

// Initializes an Object struct
Object ObjectInit(){
	Object newObject = NULL;
	newObject = (Object)malloc(sizeof(ObjectStruct));
	newObject->length = 0;
	newObject->width = 0;
	newObject->movingProb = 0;
	newObject->nextObject = NULL;
	newObject->prevObject = NULL;
	newObject->segmentList = SegmentListInit();
	return (newObject);
}

double CartesianDist(Point p1, Point p2){
	double distance;
	distance = sqrt( (p2->X - p1->X)*(p2->X - p1->X) + (p2->Y - p1->Y)*(p2->Y - p1->Y) );
	return (distance);
}

// Creates an Object from a SegmentList
Object CreateObject(SegmentList segmentList){
	Object newObject = ObjectInit();
	newObject->segmentList = segmentList;
	newObject->movingProb = 50;
	newObject->nextObject = NULL;
	newObject->prevObject = NULL;
	if (segmentList->numSegments == 1){
		newObject->length = CartesianDist(segmentList->firstSegment->FirstPoint, segmentList->firstSegment->LastPoint);
		newObject->width = 0;
	}
	else if (segmentList->numSegments == 2){
		double segmentOneLength = CartesianDist(segmentList->firstSegment->FirstPoint, segmentList->firstSegment->LastPoint);
		double segmentTwoLength = CartesianDist(segmentList->lastSegment->FirstPoint, segmentList->lastSegment->LastPoint);
		if (segmentOneLength > segmentTwoLength)
			newObject->length = segmentOneLength;
		else
			newObject->length = segmentTwoLength;
	}
	return (newObject);
}

// Initializes an ObjectList struct
ObjectList ObjectListInit(){
	ObjectList newObjectList = NULL;
	newObjectList = (ObjectList)malloc(sizeof(ObjectListStruct));
	newObjectList->firstObject = NULL;
	newObjectList->lastObject = NULL;
	newObjectList->numberOfObjects = 0;
	return (newObjectList);
}

ObjectList AddObjectToList(ObjectList list, Object object){
	if (list->numberOfObjects == 0)
		list->firstObject = object;
	else
		list->lastObject->nextObject = object;
	list->lastObject = object;
	list->numberOfObjects += 1;
	return (list);
}

/* This function determines the distance between two points in a laser scan.
 * The inputs are the ranges returned for the 2 points and the angle between them.
 * Law of cosines
 *
 */
float dist(float range1, float range2, float angle){
	float distance;
	float a_squared = range1*range1 + range2*range2 - 2*range1*range2*cos(angle);
	distance = sqrt(a_squared);
	return (distance);
}

/* This function calculates the angle between the ray to the first point and the line
 * connecting the 2 given points.
 * Inputs are the ranges to the 2 points and the angle between the points (rads).
 */
float d_angle(float range1, float range2, float angle){
	float distance;
	float x1 = range1;
	float y1 = 0;
	float x2 = range2*cos(angle);
	float y2 = range2*sin(angle);
	float theta = atan2( (y2 - y1), (x2 - x1) );
	return (theta);
}

/* This function calculates the shortest distance from point C to the line AB.
 * The inputs are range returns from the lidar scan and the angle between the scanlines of
 * B or C and A.
 */
float distToLine(float A, float B, float C, float BAngle, float CAngle){
	float AB_angle = d_angle(A, B, BAngle);
	float AC_angle = d_angle(A, C, CAngle);
	float angle_CAB = AB_angle - AC_angle;
	float distance = dist(A, C, CAngle)*sin(angle_CAB);
	return (fabs(distance));
}

// Assumes object is 2 segments
bool IsObjectConvex(Object object){
	bool result;
	Point p1 = object->segmentList->firstSegment->FirstPoint;
	Point p2 = object->segmentList->firstSegment->LastPoint;
	Point p3 = object->segmentList->lastSegment->LastPoint;
	return (result);
}

void CalcBoundingBox(ObjectList list){
	Object object = list->firstObject;
	while (object){
		if (object->segmentList->numSegments == 1){

		}

		object = object->nextObject;
	}
}

SegmentList SegmentLaserScan(float ranges[], int size, float start_angle, float angle_increment){
	SegmentList newSegmentList = SegmentListInit();
	int first = 0;
	int last = size - 1;
	float maxDistance = 0;
	float distance = 0;
	unsigned int index = 0;
	for ( int j = first + 1; j < last; j++){
		distance = distToLine(ranges[first], ranges[last], ranges[j], angle_increment*(last), angle_increment*(j));
		if (distance > maxDistance){
			maxDistance = distance;
			index = j;
		}
	}
	if (maxDistance > segmentThreshold){
		SegmentList segmentList1 = SegmentLaserScan(ranges, index+1, start_angle, angle_increment);
		float newRanges[last - index + 1];
		for (int i = 0; i < (last - index + 1); i++)
			newRanges[i] = ranges[index + i];
		SegmentList segmentList2 = SegmentLaserScan(newRanges, last - index + 1, index*angle_increment+start_angle, angle_increment);
		newSegmentList = MergeSegmentLists( segmentList1, segmentList2);
	}
	else{
		Segment segment1 = CreateSegment(ranges, size, start_angle, angle_increment);
		newSegmentList->numSegments = 1;
		newSegmentList->firstSegment = segment1;
		newSegmentList->lastSegment = segment1;
	}
	return (newSegmentList);
}

ObjectList ClusterAndSegment(const sensor_msgs::LaserScan& msg){

	// First, points are clustered into objects based on their distance to adjacent points
	int clusterLabel[msg.ranges.size()];
	int occlusions[msg.ranges.size()];
	int currentLabel = 1;
	clusterLabel[0] = currentLabel;
	for (int i = 1; i < msg.ranges.size(); i++){
		if ( dist(msg.ranges[i], msg.ranges[i-1], msg.angle_increment) > clusterThreshold )
			//clusterLabel += 1;
			currentLabel += 1;
		if (msg.ranges[i] > max_distance)
			clusterLabel[i] = 0;
		else
			clusterLabel[i] = currentLabel;
	}

	// Next, clusters are segmented, made into objects, and added to the object list
	currentLabel = 1;
	ObjectList objectList = ObjectListInit();
	int start = 0;
	int end;
	for (int i = 1; i < msg.ranges.size(); i++){
		if (clusterLabel[i] != clusterLabel[i-1]){
			end = i-1;
			if ( (end - start) > 2 && clusterLabel[i-1] != 0){
				float newRanges[i - start + 1];
				for (int j = 0; j < (end - start + 1); j++)
						newRanges[j] = msg.ranges[start+j];
				SegmentList newSegmentList = SegmentLaserScan(newRanges, (end - start + 1), start*msg.angle_increment + msg.angle_min, msg.angle_increment);
				if (start == 0 || (msg.ranges[start] > msg.ranges[start-1] && clusterLabel[start-1] != 0))
					newSegmentList->firstSegment->FirstPoint->occluded = 1;
				if (end == msg.ranges.size()-1 || (msg.ranges[end] > msg.ranges[end + 1] && clusterLabel[end+1] != 0) )
					newSegmentList->lastSegment->LastPoint->occluded = 1;
				Object newObject = CreateObject(newSegmentList);
				objectList = AddObjectToList(objectList, newObject);
			}
			start = i;
		}
		else if ( i == msg.ranges.size() - 1 ){
			end = i;
			if ( (end - start) > 2 ){
				float newRanges[i - start + 1];
				for (int j = 0; j < (end - start + 1); j++)
						newRanges[j] = msg.ranges[start+j];
				SegmentList newSegmentList = SegmentLaserScan(newRanges, (end - start + 1), start*msg.angle_increment + msg.angle_min, msg.angle_increment);
				Object newObject = CreateObject(newSegmentList);
				objectList = AddObjectToList(objectList, newObject);
			}
		}
	}
	return (objectList);
}

ObjectList CompareObjectLists(ObjectList newObjectList, ObjectList oldObjectList){
	ObjectList result = ObjectListInit();
	Object newObject = newObjectList->firstObject;
	Object oldObject = oldObjectList->firstObject;
	while(newObject){
		while(oldObject){


		}
	}

	return (result);
}


#endif /* MOVING_OBJECT_H_ */
