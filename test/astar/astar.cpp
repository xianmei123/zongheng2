// A C++ Program to implement A* Search Algorithm
#include <bits/stdc++.h>
#include <time.h> 
using namespace std;

#define ROW 2000
#define COL 2000
#define NODES_NUM 20000

int sum = 0;
int max_row = ROW, max_col = COL;
int grid[ROW][COL];
int start_nodes[NODES_NUM][2];
int end_nodes[NODES_NUM][2];
int test_size = 0;
int success_num = 0;
int fail_num = 0;
int path_count = 0;

// Creating a shortcut for int, int pair type
typedef pair<int, int> Pair;

// Creating a shortcut for pair<int, pair<int, int>> type
typedef pair<double, pair<int, int> > pPair;


// A structure to hold the necessary parameters
struct cell {
	// Row and Column index of its parent
	// Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
	int parent_i, parent_j;
	// f = g + h
	double f, g, h;
};

cell cellDetails[ROW][COL];

void readNodes() {
	printf("\n开始读取测试数据...\n");
	clock_t start, end;
    double cpu_time_used;

    start = clock();
	FILE *file = fopen("testNodes.txt", "r");
    if (file == NULL) {
        printf("无法打开文件以进行读取。\n");
        return; // 非法文件指针
    }

    // 读取地图大小
    fscanf(file, "起止点对数%d\n", &test_size);
    int pairsCount = 0;
    int startX, startY, endX, endY;
    int tmp = 0;

    // 读取起止点坐标
    while (!feof(file) && pairsCount < test_size) {
        if (fscanf(file, "起止点 %d: (%d, %d) -> (%d, %d)\n", &tmp,
                    &startX, &startY,
                    &endX, &endY) == 5) {
            // 成功读取一对起止点
            start_nodes[pairsCount][0] = startX;
            start_nodes[pairsCount][1] = startY;
            end_nodes[pairsCount][0] = endX;
            end_nodes[pairsCount][1] = endY;
            pairsCount++;
        } else {
            // 读取失败，可能是因为格式不正确或到达文件末尾
            break;
        }
    }

    // 关闭文件
    fclose(file);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n测试数据读取完成！\n");
    printf("\n函数readNodes的执行时间为: %.5lf 秒.\n", cpu_time_used);
}

// A Utility Function to check whether given cell (row, col)
// is a valid cell or not.
bool isValid(int row, int col)
{
	// Returns true if row number and column number
	// is in range
	return (row >= 0) && (row < max_row) && (col >= 0)
		&& (col < max_col);
}

// A Utility Function to check whether the given cell is
// blocked or not
bool isUnBlocked(int row, int col)
{
	// Returns true if the cell is not blocked else false
	if (grid[row][col] == 0)
		return (true);
	else
		return (false);
}

// A Utility Function to check whether destination cell has
// been reached or not
bool isDestination(int row, int col, Pair dest)
{
	if (row == dest.first && col == dest.second)
		return (true);
	else
		return (false);
}

// A Utility Function to calculate the 'h' heuristics.
double calculateHValue(int row, int col, Pair dest)
{
	// Return using the distance formula
	//sum++;
	double ans = ((double)sqrt(
		(row - dest.first) * (row - dest.first)
		+ (col - dest.second) * (col - dest.second)));
	//printf("(%d, %d): %f\n", row, col, ans);
	return ans;
}

// A Utility Function to trace the path from the source
// to destination
void tracePath(Pair dest)
{
	//printf("\nThe Path is ");
	int row = dest.first;
	int col = dest.second;

	stack<Pair> Path;

	while (!(cellDetails[row][col].parent_i == row
			&& cellDetails[row][col].parent_j == col)) {
		Path.push(make_pair(row, col));
		int temp_row = cellDetails[row][col].parent_i;
		int temp_col = cellDetails[row][col].parent_j;
		row = temp_row;
		col = temp_col;
	}

	Path.push(make_pair(row, col));
	//FILE *outputFile = fopen("E:\\bishe\\astar_cpp\\output1.txt", "w");
	
	while (!Path.empty()) {
		pair<int, int> p = Path.top();
		Path.pop();
		path_count++;
		//fprintf(outputFile, "-> (%d, %d)", p.first, p.second);
	}
	//printf("\n");
	//fclose(outputFile);

	return;
}

// A Function to find the shortest path between
// a given source cell to a destination cell according
// to A* Search Algorithm
void aStarSearch(Pair src, Pair dest)
{
	// If the source is out of range
	if (isValid(src.first, src.second) == false) {
		printf("Source is invalid\n");
		fail_num++;
		return;
	}

	// If the destination is out of range
	if (isValid(dest.first, dest.second) == false) {
		printf("Destination is invalid\n");
		fail_num++;
		return;
	}

	// Either the source or the destination is blocked
	if (isUnBlocked(src.first, src.second) == false
		|| isUnBlocked(dest.first, dest.second)
			== false) {
		printf("Source or the destination is blocked\n");
		fail_num++;
		return;
	}

	// If the destination cell is the same as source cell
	if (isDestination(src.first, src.second, dest)
		== true) {
		printf("We are already at the destination\n");
		fail_num++;
		return;
	}

	// Create a closed list and initialise it to false which
	// means that no cell has been included yet This closed
	// list is implemented as a boolean 2D array
	bool closedList[max_row][max_col];
	memset(closedList, false, sizeof(closedList));

	// Declare a 2D array of structure to hold the details
	// of that cell
//	cell cellDetails[max_row][max_col];

	int i, j;

	for (i = 0; i < max_row; i++) {
		for (j = 0; j < max_col; j++) {
			cellDetails[i][j].f = FLT_MAX;
			cellDetails[i][j].g = FLT_MAX;
			cellDetails[i][j].h = FLT_MAX;
			cellDetails[i][j].parent_i = -1;
			cellDetails[i][j].parent_j = -1;
		}
	}

	// Initialising the parameters of the starting node
	i = src.first, j = src.second;
	cellDetails[i][j].f = 0.0;
	cellDetails[i][j].g = 0.0;
	cellDetails[i][j].h = 0.0;
	cellDetails[i][j].parent_i = i;
	cellDetails[i][j].parent_j = j;

	/*
	Create an open list having information as-
	<f, <i, j>>
	where f = g + h,
	and i, j are the row and column index of that cell
	Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
	This open list is implemented as a set of pair of
	pair.*/
	set<pPair> openList;

	// Put the starting cell on the open list and set its
	// 'f' as 0
	openList.insert(make_pair(0.0, make_pair(i, j)));

	// We set this boolean value as false as initially
	// the destination is not reached.
	bool foundDest = false;
	
	int directions[8][3] = {
                {-1, -1, 1}, {-1, 0, 0}, {-1, 1, 1},
                { 0, -1, 0},             { 0, 1, 0},
                { 1, -1, 1}, { 1, 0, 0}, { 1, 1, 1}
        };

	while (!openList.empty()) {
		pPair p = *openList.begin();

		// Remove this vertex from the open list
		openList.erase(openList.begin());

		// Add this vertex to the closed list
		i = p.second.first;
		j = p.second.second;
		closedList[i][j] = true;

		double gNew, hNew, fNew;
		
		int curRow = i;
        int curCol = j;
        int succ;
        for(succ = 0;succ < 8; succ++) {
            int flag = 0;
            curRow = i + directions[succ][0];
            curCol = j + directions[succ][1];
            flag = directions[succ][2];
            if (isValid(curRow, curCol) == true) {
                // If the destination cell is the same as the
                // current successor
                if (isDestination(curRow, curCol, dest) == true) {
                    // Set the Parent of the destination cell
                    cellDetails[curRow][curCol].parent_i = i;
                    cellDetails[curRow][curCol].parent_j = j;
                    //printf("The destination cell is found\n");
                    tracePath(dest);
                    foundDest = true;
                    success_num++;
                    return;
                }
                    // If the successor is already on the closed
                    // list or if it is blocked, then ignore it.
                    // Else do the following
                else if (closedList[curRow][curCol] == false
                         && isUnBlocked(curRow, curCol)
                            == true) {
                    double addNum = 1.0;
                    if (flag == 1) {
                        addNum = 1.414;
                    }
                    gNew = cellDetails[i][j].g + addNum;
                    hNew = calculateHValue(curRow, curCol, dest);
                    fNew = gNew + hNew;

                    if (cellDetails[curRow][curCol].f == FLT_MAX
                        || cellDetails[curRow][curCol].f > fNew) {
                        openList.insert(make_pair(
                                fNew, make_pair(curRow, curCol)));

                        // Update the details of this cell
                        cellDetails[curRow][curCol].f = fNew;
                        cellDetails[curRow][curCol].g = gNew;
                        cellDetails[curRow][curCol].h = hNew;
                        cellDetails[curRow][curCol].parent_i = i;
                        cellDetails[curRow][curCol].parent_j = j;
                    }
                }
            }
        }
    }

	if (foundDest == false)
		printf("Failed to find the Destination Cell\n");
	fail_num++;
	return;
}

// Driver program to test above function
int main()
{
	int row;
	int col;
	
	FILE *inputFile = fopen("E:\\bishe\\astar_cpp\\input.txt", "r");
	
    if (inputFile == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }
    
    fscanf(inputFile, "%d,%d\n", &row, &col);
    max_row = row;
    max_col = col;
    
    int start_x, start_y;
    int end_x, end_y;
    fscanf(inputFile, "%d,%d\n", &start_x, &start_y);
    fscanf(inputFile, "%d,%d\n", &end_x, &end_y);
    
    int obstacles_count;
    fscanf(inputFile, "%d\n", &obstacles_count);
	
	/* Description of the Grid-
	0--> The cell is not blocked
	1--> The cell is blocked */
	
	for (int i = 0; i < obstacles_count; i++) {
		int tmp_x, tmp_y;
		fscanf(inputFile, "%d,%d\n", &tmp_x, &tmp_y);
		grid[tmp_x][tmp_y] = 1;
	}
	
	readNodes();

	// Source is the left-most bottom-most corner
	Pair src = make_pair(4, 6);

	// Destination is the left-most top-most corner
	Pair dest = make_pair(195, 196);
	
	printf("%d, %d\n%d, %d\n", start_x, start_y, end_x, end_y);

	clock_t start, end;
    double cpu_time_used;

    start = clock();
    
    printf("::\n");

	for (int i = 0; i < test_size; i++) {
		src.first = start_nodes[i][0];
		src.second = start_nodes[i][1];
		dest.first = end_nodes[i][0];
		dest.second = end_nodes[i][1];
		//printf("起止点 %d: (%d, %d) -> (%d, %d)\n", i, src.first, src.second, dest.first, dest.second);
		aStarSearch(src, dest);
	}
//	aStarSearch(src, dest);
	
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("\n函数aStarSearch的执行时间为: %.5lf 秒.\n", cpu_time_used);
    printf("寻路成功:%d，寻路失败:%d\n", success_num, fail_num);
    
    fclose(inputFile);
    
    printf("sum: %d\n", sum);
    
    printf("count: %d\n", path_count);

	return (0);
}

