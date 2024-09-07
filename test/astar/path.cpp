// A C++ Program to implement A* Search Algorithm
#include <bits/stdc++.h>
#include <time.h> 
using namespace std;

#define ROW 1000
#define COL 1000
#define BLOCK 3
#define TARGET 3
#define NODES_NUM 20000

int max_row = ROW, max_col = COL;
int grid[ROW][COL];
int rowPerBlock = max_row / BLOCK, colPerBlock = max_col / BLOCK;
int sum = 0;
int paths[(BLOCK + 1) * ((TARGET - 1) * BLOCK + 1) * 2][(BLOCK + 1) * ((TARGET - 1) * BLOCK + 1) * 2][ROW + COL][2];
int pointList[(BLOCK + 1) * ((TARGET - 1) * BLOCK + 1) * 2][2];
int pointLen = 0;
int foundNum = 0;
int noNum = 0;
int ansList[3][ROW + COL][2];
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
	FILE *file = fopen("E:\\bishe\\astar_cpp\\testNodes.txt", "r");
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
	return ((double)sqrt(
		(row - dest.first) * (row - dest.first)
		+ (col - dest.second) * (col - dest.second)));
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
	while (!Path.empty()) {
		pair<int, int> p = Path.top();
		Path.pop();
		//printf("-> (%d,%d) ", p.first, p.second);
	}

	return;
}

int isBorder(int row, int col) {
	for (int i = 0; i < pointLen; i++) {
		if (pointList[i][0] == row && pointList[i][1] == col) {
			return 1;
		}
	}
	return 0;
}

int calIndex(int row, int col) {
	for (int i = 0; i < (BLOCK + 1) * ROW * 2; i++) {
		if (row == pointList[i][0] && col == pointList[i][1]) {
			return i;
		}
	}
	return -1;
}

void storePath(Pair src, Pair dest)
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
	int srcIndex = calIndex(src.first, src.second);
	int dstIndex = calIndex(dest.first, dest.second);
	if (srcIndex == -1 || dstIndex == -1) {
		return;
	}
	int num = 0;
	while (!Path.empty()) {
		pair<int, int> p = Path.top();
		Path.pop();
		//printf("-> (%d,%d) ", p.first, p.second);

		paths[srcIndex][dstIndex][++num][0] = p.first;
		paths[srcIndex][dstIndex][num][1] = p.second;
	}
	paths[srcIndex][dstIndex][0][0] = num;

	return;
}

void restorePath(int dst_x, int dst_y, int tag)
{
	//printf("\nThe Path is ");
	int row = dst_x;
	int col = dst_y;

    int index = 0;
    if (tag == 2) {
        index = 0;
    } else if (tag == 3) {
        index = 2;
    }

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
    int num = 0;
	while (!Path.empty()) {
		pair<int, int> p = Path.top();
		Path.pop();
		//printf("-> (%d,%d) ", p.first, p.second);
		//printf("(%d,%d) ", p.first, p.second);
		ansList[index][++num][0] = p.first;
        ansList[index][num][1] = p.second;
	}
	//printf("\n");
    ansList[index][0][0] = num;

	return;
}

void aStarSearch(Pair src, Pair dest, int tag)
{
	// If the source is out of range
	if (isValid(src.first, src.second) == false) {
		fail_num++;
		printf("Source is invalid\n");
		return;
	}

	// If the destination is out of range
	if (isValid(dest.first, dest.second) == false) {
		fail_num++;
		printf("Destination is invalid\n");
		return;
	}

	// Either the source or the destination is blocked
	if (isUnBlocked(src.first, src.second) == false
		|| isUnBlocked(dest.first, dest.second)
			== false) {
		fail_num++;
		//noNum++;
		//printf("(%d, %d) -> (%d, %d) is blocked\n", src.first, src.second, dest.first, dest.second);
		printf("Source or the destination is blocked\n");
		return;
	}

	// If the destination cell is the same as source cell
	if (isDestination(src.first, src.second, dest)
		== true) {
		fail_num++;
		//foundNum++;
		printf("We are already at the destination\n");
		return;
	}

	bool closedList[max_row][max_col];
	memset(closedList, false, sizeof(closedList));


	int i, j;

	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
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

	set<pPair> openList;

	openList.insert(make_pair(0.0, make_pair(i, j)));

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
                //printf("currentNode: (%d, %d)\n", curRow, curCol);
                if (isDestination(curRow, curCol, dest) == true) {
                	success_num++;
                    // Set the Parent of the destination cell
                    cellDetails[curRow][curCol].parent_i = i;
                    cellDetails[curRow][curCol].parent_j = j;
                    //printf("The destination cell is found\n");
                    //foundNum++;
                    if (tag == 0) {
                    	tracePath(dest);
					}
                    else if (tag == 1) {
                        //printf("store: (%d, %d) -> (%d, %d)\n", src.first, src.second, dest.first, dest.second);
                    	storePath(src, dest);
					} else if (tag == 2 || tag == 3) {
						restorePath(curRow, curCol, tag);
					}
                    foundDest = true;
                    return;
                }
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

	if (foundDest == false) {
		printf("Failed to find the Destination Cell\n");
		printf("(%d, %d): %d -> (%d, %d): %d\n", src.first, src.second, grid[src.first][src.second], 
													dest.first, dest.second, grid[dest.first][dest.second]);
		//noNum++;
	}
	fail_num++;
		

	return;
}

void aStarInit(int row, int col) {
    printf("start init...\n");
    clock_t start, end;
    double cpu_time_used;
    printf("??");

    start = clock();
    
	int block_row = row / BLOCK;
	int block_col = col / BLOCK;
	int offest = 0;
	for (int i = 0; i < BLOCK + 1; i++) {
		for (int j = 0; j < ((TARGET - 1) * BLOCK + 1); j++) {
			pointList[i * ((TARGET - 1) * BLOCK + 1) + j][0] = (i == 0) ? 0 : i * block_col - 1;
			pointList[i * ((TARGET - 1) * BLOCK + 1) + j][1] = (j == 0) ? 0 : j * (block_col / (TARGET - 1)) - 1;
			offest += 1;
		}
	}
	for (int i = 0; i < BLOCK + 1; i++) {
		for (int j = 0; j < ((TARGET - 1) * BLOCK + 1); j++) {
			pointList[i * ((TARGET - 1) * BLOCK + 1) + j + offest][0] = (j == 0) ? 0 : j * (block_row / (TARGET - 1)) - 1;
			pointList[i * ((TARGET - 1) * BLOCK + 1) + j + offest][1] = (i == 0) ? 0 : i * block_row - 1;
		}
	}

    pointLen = 2 * offest;
    printf("::%d::\n", pointLen);

    for (int i = 0; i < offest; i++) {
        int x = pointList[i][0];
        int y = pointList[i][1];
        if (isUnBlocked(x, y)) {
            continue;
        }
        for (int j = 1; j < block_col / TARGET; j++) {
            if (isValid(x, y - j)) {
                if (isUnBlocked(x, y - j) && pointList[i - 1][1] < y - j) {
                    y = y - j;
                    break;
                }
            }
            if (isValid(x, y + j)) {
                if (isUnBlocked(x, y + j) && pointList[i + 1][1] > y + j) {
                    y = y + j;
                    break;
                }
            }
        }
        pointList[i][0] = x;
        pointList[i][1] = y;
    }
    for (int i = offest; i < 2 * offest; i++) {
        int x = pointList[i][0];
        int y = pointList[i][1];
        if (isUnBlocked(x, y)) {
            continue;
        }
        for (int j = 1; j < block_row / TARGET; j++) {
            if (isValid(x - j, y)) {
                if (isUnBlocked(x - j, y) && pointList[i - 1][0] < x - j) {
                    x = x - j;
                    break;
                }
            }
            if (isValid(x + j, y)) {
                if (isUnBlocked(x + j, y) && pointList[i + 1][0] > x + j) {
                    x = x + j;
                    break;
                }
            }
        }
        pointList[i][0] = x;
        pointList[i][1] = y;
    }
//    for (int i = 0; i < 2 * offest; i++) {
//    	printf("::(%d, %d)\n", pointList[i][0], pointList[i][1]);
//	}
	
	Pair src = make_pair(0,0);
	Pair dst = make_pair(0,0);
	for (int i = 0; i < 2 * offest; i++) {
		src.first = pointList[i][0];
		src.second = pointList[i][1];
		if (isUnBlocked(src.first, src.second) == false) {
			continue;
		}
		for (int j = 0; j < 2 * offest; j++) {
			dst.first = pointList[j][0];
			dst.second = pointList[j][1];
			//printf("check: (%d, %d) -> (%d, %d)\n", src.first, src.second, dst.first, dst.second);
			if (isUnBlocked(dst.first, dst.second) == false) {
				continue;
			}
			if (src.first == dst.first && src.second == dst.second) {
				continue;
			}
			//printf("start:: (%d, %d) -> (%d, %d)\n", src.first, src.second, dst.first, dst.second);
			aStarSearch(src, dst, 1);
		}
		printf("%d \\ %d\n", i, 2 * offest);
	}
	//printf("found: %d\nnoFound: %d\n", foundNum, noNum);
    printf("finish init!\n");
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("\n函数aStarInit的执行时间为: %.5lf 秒.\n", cpu_time_used);
}

Pair calDest(int src_x, int src_y, int dst_x, int dst_y) {
	//printf("%d, %d; %d, %d\n",src_x, src_y, dst_x, dst_y);
	int rowOffest, colOffest;
	if (src_x < dst_x) {
		if (rowPerBlock - (src_x % rowPerBlock) < dst_x - src_x) {
			rowOffest = rowPerBlock - (src_x % rowPerBlock);
		} else {
			rowOffest = dst_x - src_x;
		}
	} else if (src_x > dst_x) {
		if (src_x % rowPerBlock < src_x - dst_x) {
			rowOffest = - (src_x % rowPerBlock);
		} else {
			rowOffest = dst_x - src_x;
		}
	} else {
		rowOffest = 0;
	}
	if (src_y < dst_y) {
		if (colPerBlock - (src_y % colPerBlock) < dst_y - src_y) {
			colOffest = colPerBlock - (src_y % colPerBlock);
		} else {
			colOffest = dst_y - src_y;
		}
	} else if (src_y > dst_y) {
		if (src_y % colPerBlock < src_y - dst_y) {
			colOffest = - (src_y % colPerBlock);
		} else {
			colOffest = dst_y - src_y;
		}
	} else {
		colOffest = 0;
	}
	int x = src_x + rowOffest - 1;
	int y = src_y + colOffest - 1;
	//printf("offest:(%d, %d); (%d, %d)\n", rowOffest, colOffest, x, y);
    int min = 10086;
    int index = 0;
	if ((x + 1) % rowPerBlock == 0) {
        for (int i = 0; i < pointLen / 2; i++) {
            if (pointList[i][0] == x) {
                if (min > abs(pointList[i][1] - y)) {
                    min = abs(pointList[i][1] - y);
                    index = i;
                }
            }
        }
    }
    else if ((y + 1) % colPerBlock == 0) {
        for (int i = pointLen / 2; i < pointLen; i++) {
            if (pointList[i][1] == y) {
                if (min > abs(pointList[i][0] - x)) {
                    min = abs(pointList[i][0] - x);
                    index = i;
                }
            }
        }
    }
    else {
        //printf("in one block\n");
        aStarSearch(make_pair(src_x, src_y), make_pair(dst_x, dst_y), 0);
        return make_pair(-1, -1);
    }
    return make_pair(pointList[index][0], pointList[index][1]);
}


// Driver program to test above function
int main()
{
	int row;
	int col;
	
	FILE *inputFile = fopen("E:\\bishe\\astar_cpp\\input.txt", "r");
	FILE *outputFile = fopen("E:\\bishe\\astar_cpp\\output.txt", "w");
    if (inputFile == NULL || outputFile == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }
    
    fscanf(inputFile, "%d,%d\n", &row, &col);
    max_row = row;
    max_col = col;
    rowPerBlock = max_row / BLOCK;
    colPerBlock = max_col / BLOCK;
    
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

    aStarInit(row, col);
    success_num = 0;
    fail_num = 0;
    

    //printf(";;;(%d, %d)\n", dst.first, dst.second);
    

    printf("\n");
    
    start_x = 4;
    start_y = 6;
    end_x = 195;
    end_y = 196;

	// Source is the left-most bottom-most corner
	Pair src = make_pair(start_x, start_y);

	// Destination is the left-most top-most corner
	Pair dest = make_pair(end_x, end_y);

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
		
	Pair midDst1 = calDest(src.first, src.second, dest.first, dest.second);
	if (midDst1.first == -1 && midDst1.second == -1) {
		continue;
	}
	Pair midDst2 = calDest(dest.first, dest.second, src.first, src.second);

	if (isBorder(src.first, src.second)) {
		ansList[0][0][0] = 1;
		ansList[0][1][0] = src.first;
		ansList[0][1][1] = src.second;
		//printf("1\n");
	} else {
		aStarSearch(src, midDst1, 2);
		//printf("2\n");
	}
    if (isBorder(dest.first, dest.second)) {
		ansList[2][0][0] = 1;
		ansList[2][1][0] = dest.first;
		ansList[2][1][1] = dest.second;
		//printf("3\n");
	} else {
		aStarSearch(dest, midDst2, 3);
		//printf("4\n");
	}
    int len1 = ansList[0][0][0];
    int len3 = ansList[2][0][0];
    
    int sIndex = calIndex(midDst1.first, midDst1.second);
    int eIndex = calIndex(midDst2.first, midDst2.second);
    int len2 = paths[sIndex][eIndex][0][0];
    path_count += len1 + len2 + len3;
	}
//	for (int i = 1; i < len1; i++) {
//        fprintf(outputFile, "-> (%d, %d)", ansList[0][i][0], ansList[0][i][1]);
//    }
//    //printf("\n");
//    for (int i = 1; i <= len2; i++) {
//        fprintf(outputFile, "-> (%d, %d)", paths[sIndex][eIndex][i][0], paths[sIndex][eIndex][i][1]);
//    }
//    //printf("\n");
//    for (int i = len3 - 1; i >= 1; i--) {
//        fprintf(outputFile, "-> (%d, %d)", ansList[2][i][0], ansList[2][i][1]);
//    }
//}
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    fprintf(outputFile, "\n函数aStarSearch的执行时间为: %.5lf 秒.\n", cpu_time_used);
    printf("\n函数aStarSearch的执行时间为: %.5lf 秒.\n", cpu_time_used);
    printf("寻路成功:%d，寻路失败:%d\n", success_num, fail_num);
    
    fclose(inputFile);
    fclose(outputFile);
    
    printf("\nsum: %d\n", sum);
    
    printf("count: %d\n", path_count);

	return (0);
}



