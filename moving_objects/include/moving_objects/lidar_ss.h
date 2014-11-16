/*
 * lidar_ss.h
 *
 *  Created on: Oct 1, 2014
 *      Author: alan
 *      Modified by: Dan
 */

#ifndef LIDAR_SS_H_
#define LIDAR_SS_H_
#include <nav_msgs/OccupancyGrid.h>

void grid_init(nav_msgs::OccupancyGrid& grid, int init_value, std::string frame){
	double resolution = 0.25;           //  Grid resolution in m/cell
	double side_len = 30;            // Grid size in meters

	//grid.header.frame_id = "/world";
	grid.header.frame_id = frame;
	grid.info.resolution = resolution;
	grid.info.width = side_len /resolution;
	grid.info.height = side_len / resolution;
	grid.data.resize(grid.info.width * grid.info.height);
	for (int i = 0; i < grid.data.size(); i++) {
		grid.data[i] = init_value;
	}
}

/* This function takes the beginning and end points of a laser measurement and marks the spaces in-between
 * them free in the provided occupancy grid
 *
 */
void raytrace(float x0, float y0, float x1, float y1, nav_msgs::OccupancyGrid& grid)
{

	if (x1 > 30 || y1 > 30)
		return;

	float dx = fabs(x1 - x0);
	float dy = fabs(y1 - y0);

	double resolution = grid.info.resolution;

	int grid_x = int(floor(x0/resolution));
	int grid_y = int(floor(y0/resolution));

	float dt_dx = 1 / dx;
	float dt_dy = 1 / dy;

	float t = 0;

	int n = -1;
	int x_inc, y_inc;
	float t_next_vertical, t_next_horizontal;

	if (dx == 0)
	{
		x_inc = 0;
		t_next_horizontal = dt_dx; // infinity
	}
	else if (x1 > x0)
	{
		x_inc = 1;
		n += (int(floor(x1/resolution)) - grid_x);
		t_next_horizontal = (resolution*(floor(x0/resolution) + 1) - x0) * dt_dx;
	}
	else
	{
		x_inc = -1;
		n += (grid_x - int(floor(x1/resolution)));
		t_next_horizontal = (x0 - resolution*floor(x0/resolution)) * dt_dx;
	}

	if (dy == 0)
	{
		y_inc = 0;
		t_next_vertical = dt_dy; // infinity
	}
	else if (y1 > y0)
	{
		y_inc = 1;
		n += (int(floor(y1/resolution)) - grid_y);
		t_next_vertical = (resolution*(floor(y0/resolution)) + 1 - y0) * dt_dy;
	}
	else
	{
		y_inc = -1;
		n += (grid_y - int(floor(y1/resolution)));
		t_next_vertical = (y0 - resolution*floor(y0/resolution)) * dt_dy;
	}

	int width = sqrt(grid.data.size());

	for (; n > 0; --n)
	{
		int i = width*grid_y + grid_x;
		if ( i < grid.data.size()){
			if (grid.data[i] >= 7)
				grid.data[i] -= 7;
			else
				grid.data[i] = 0;
		}

		if (t_next_vertical < t_next_horizontal)
		{
			grid_y += y_inc;
			t = t_next_vertical;
			t_next_vertical += dt_dy;
		}
		else
		{
			grid_x += x_inc;
			t = t_next_horizontal;
			t_next_horizontal += dt_dx;
		}
	}
}

void update_hit( float x, float y, nav_msgs::OccupancyGrid& grid){
	int width = sqrt(grid.data.size());
	int x_hit = int(floor(x/grid.info.resolution));
	int y_hit = int(floor(y/grid.info.resolution));
	int i = width*y_hit + x_hit;
	if ( i < grid.data.size()){
		if (grid.data[i] <= 85)
			grid.data[i] += 15;
		else
			grid.data[i] = 100;
	}
}

void grid_update(nav_msgs::OccupancyGrid& grid, nav_msgs::OccupancyGrid& temp_grid){
	int width = sqrt(grid.data.size());
	for (int i = 0; i < grid.data.size(); i++){
		if (temp_grid.data[i] == 50){
			if (grid.data[i] > 55)
				grid.data[i] -= 5;
			else if (grid.data[i] < 45)
				grid.data[i] += 5;
			else
				grid.data[i] = 50;
		}
		else{
			int sum = grid.data[i] + temp_grid.data[i] - 50;
		if (sum < 0)
			grid.data[i] = 0;
		else if (sum > 100)
			grid.data[i] = 100;
		else
			grid.data[i] = sum;
		}
	}
}

#endif /* LIDAR_SS_H_ */
