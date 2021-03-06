//*****************************************************************************
//
//		Author: Jay Offerdahl
//		Class:	EECS 560 (Data Structures)
//		Lab:	Tues. 11a - 12:50p
//		Lab #: 	10
//
//*****************************************************************************

#include <iostream>
#include <stdlib.h>

void printMaze(int*, int*, int, int, int*);
bool notConnected(int*, int);
int root(int*, int);

int main()
{
	std::cout << "********************************************************************\n\n";
	std::cout << "                       Jay Offerdahl - Lab 10                       \n\n";
	std::cout << "********************************************************************\n\n";

	int length, height;

	while(true) {
		std::cout << "Please enter the dimensions of the grid (Format: [length] [height]):\n";
		std::cin >> length >> height;

		if(length < 1 || height < 1)
			std::cout << "Invalid dimensions. Please try again.\n\n";
		else
			break;

		std::cin.ignore();
	}

	std::cout << "Entered dimensions: length = " << length << ", height = " << height << "\n";

	// Initialize all variables/arrays
	int n = length * height, index, wall, temp, wallIndexToRemove;
	int maze[n], h_walls[n], v_walls[n];
	bool horizontal = false, validWall;

	for(int i = 0; i < n; i++) {
		maze[i] = -1;
		h_walls[i] = 1;
		v_walls[i] = 1;
	}

	// Print the original maze
	printMaze(h_walls, v_walls, length, height, maze);

	srand(9857);

	// Complete the maze using the union find data structure
	while(notConnected(maze, n)) {
		// Select random element & wall
		index = rand() % n;
		wall = rand() % 4;

		// Check validity of wall removal, & store necessary info for removal
		// Use N - 0, E - 1, S - 2, W - 3
		if(wall == 0 && index > length - 1) {
			if(h_walls[index - length] == 1) {
				horizontal = true;
				validWall = true;
				temp = index - length;
				wallIndexToRemove = temp;
			}
		}
		else if(wall == 1 && index % length != length - 1) {
			if(v_walls[index + 1] == 1) {
				horizontal = false;
				validWall = true;
				temp = index + 1;
				wallIndexToRemove = temp;
			}
		}
		else if(wall == 2 && index < (height - 1) * length && h_walls[index] == 1) {
			horizontal = true;
			validWall = true;
			temp = index + length;
			wallIndexToRemove = index;
		}
		else if(wall == 3 && index % length != 0 && v_walls[index] == 1) {
			horizontal = false;
			validWall = true;
			temp = index - 1;
			wallIndexToRemove = index;
		}

		// If approved for wall removal, check additional parameters and remove
		if(validWall) {
			if(root(maze, index) != root(maze, temp)) {
				maze[root(maze, index)] = temp;

				if(horizontal)
					h_walls[wallIndexToRemove] = 0;
				else
					v_walls[wallIndexToRemove] = 0;
			}
			validWall = false;
		}
	}

	// Print the union-found maze
	printMaze(h_walls, v_walls, length, height, maze);

	// Exit the program
	return 0;
}

/**
 * Prints the maze showing which walls have been removed/which walls persist
 * @param h_walls - the array of horizontal walls
 * @param v_walls - the array of vertical walls
 * @param length  - the length (horizontal) of the array
 * @param height  - the height (vertical) of the array
 */
void printMaze(int *h_walls, int *v_walls, int length, int height, int *maze) {
	int level = 0;
	for(int i = 0; i < height * 2 + 1; i++) {
		if(i % 2 == 0) {
			// Print horiztonal wall
			for(int j = 0; j < length; j++) {
				// Always empty
				if(i == 0 && j == 0)
					std::cout << "    ";
				else if(i == height * 2 && j == length - 1)
					std::cout << "-    ";
				else if(i == 0 || i == height * 2)
					std::cout << "----";
				else if(h_walls[j + (level - 1) * length] == 1)
					std::cout << "----";
				else 
					std::cout << "-   ";

				if(j == length - 1 && i != height * 2)
					std::cout << "-";
			}
		}
		else {
			// Print vertical wall
			for(int j = 0; j < length + 1; j++) {
				// Always empty
				if(i == 1 && j == 0)
					std::cout << "    ";
				else if(i == height * 2 - 1 && j == length)
					std::cout << " ";
				else if(j == 0)
					std::cout << "|   ";
				else if(j == length)
					std::cout << "|";
				else if(v_walls[j + level * length] == 1)
					std::cout << "|   ";
				else
					std::cout << "    ";
			}
			level++;
		}
		std::cout << "\n";		
	}
}

/**
 * Determines if all array elements belong to the same set or not
 * @param  maze - the array object
 * @param  n    - the number of elements in the array
 * @return      - true if the maze is connected, false otherwise
 */
bool notConnected(int *maze, int n) {
	int temp = root(maze, 0);

	// Do a linear search, if any negative values, return false
	for(int i = 0; i < n; i++) {
		if(root(maze, i) != temp)
			return true;
	}

	return false;
}

/**
 * Finds the root of this set
 * @param  maze  - the array object
 * @param  index - the index to be examined
 * @return       - the index of the root of this index
 */
int root(int *maze, int index) {
	int currIndex = maze[index];

	if(currIndex == -1)
		return index;
	else {
		while(maze[currIndex] != -1)
			currIndex = maze[currIndex];

		return currIndex;
	}
}