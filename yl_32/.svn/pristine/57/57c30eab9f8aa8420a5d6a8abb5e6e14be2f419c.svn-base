/* -------------------------------------------------------------------------
//	文件名		：	kastarpathfinder.cpp
//	创建者		：	simon
//	创建时间	：	2009/12/2 21:53:58
//	功能描述	：	infrastructure of Client AutoPath & BattleField AutoPath
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "nsbase/kastarpathfinder.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KAstarPathFinder g_cAstarPathFinder;

// -------------------------------------------------------------------------
KAstarPathFinder::KAstarPathFinder()
{

}

KAstarPathFinder::~KAstarPathFinder()
{
	Uninit();
}

INT KAstarPathFinder::FindPath( INT nStartX, INT nStartY, INT nTargetX, INT nTargetY)
{
	INT onOpenList=0, parentXval=0, parentYval=0,
		a=0, b=0, m=0, u=0, v=0, temp=0, corner=0, numberOfOpenListItems=0,
		addedGCost=0, tempGcost = 0, path = 0, x=0, y=0,
		tempx, pathX, pathY, cellPosition,
		newOpenListItemID=0;

	//1. Convert location data (in pixels) to coordinates in the m_pBlocks array.
	INT startX = nStartX/tileSize;
	INT startY = nStartY/tileSize;	
	nTargetX = nTargetX/tileSize;
	nTargetY = nTargetY/tileSize;

	//2.Quick Path Checks: Under the some circumstances no path needs to
	//	be generated ...

	//	If starting location and target are in the same location...
	if (startX == nTargetX && startY == nTargetY && m_nPathLocation > 0)
		return found;
	if (startX == nTargetX && startY == nTargetY && m_nPathLocation == 0)
		return nonexistent;

	//	If target square is unwalkable, return that it's a nonexistent path.
	if (m_pBlocks[nTargetX * m_nMapHeight + nTargetY] == unwalkable)
		goto noPath;

	//3.Reset some variables that need to be cleared
	for (x = 0; x < m_nMapWidth; x++)
	{
		for (y = 0; y < m_nMapHeight;y++)
			m_pWhichList [x * (m_nMapHeight + 1) + y] = 0;
	}
	onClosedList = 2; //changing the values of onOpenList and onClosed list is faster than redimming m_pWhichList() array
	onOpenList = 1;
	m_nPathLength  = notStarted;//i.e, = 0
	m_nPathLocation  = notStarted;//i.e, = 0
	m_pGcost[startX * (m_nMapHeight + 1) + startY] = 0; //reset starting square's G value to 0

	//4.Add the starting location to the open list of squares to be checked.
	numberOfOpenListItems = 1;
	m_pOpenList[1] = 1;//assign it as the top (and currently only) item in the open list, which is maintained as a binary heap (explained below)
	m_pOpenX[1] = startX ; m_pOpenY[1] = startY;

	//5.Do the following until a path is found or deemed nonexistent.
	do
	{

		//6.If the open list is not empty, take the first cell off of the list.
		//	This is the lowest F cost cell on the open list.
		if (numberOfOpenListItems != 0)
		{

			//7. Pop the first item off the open list.
			parentXval = m_pOpenX[m_pOpenList[1]];
			parentYval = m_pOpenY[m_pOpenList[1]]; //record cell coordinates of the item
			m_pWhichList[parentXval * (m_nMapHeight + 1) + parentYval] = onClosedList;//add the item to the closed list

			//	Open List = Binary Heap: Delete this item from the open list, which
			//  is maintained as a binary heap. For more information on binary heaps, see:
			//	http://www.policyalmanac.org/games/binaryHeaps.htm
			numberOfOpenListItems = numberOfOpenListItems - 1;//reduce number of open list items by 1	

			//	Delete the top item in binary heap and reorder the heap, with the lowest F cost item rising to the top.
			m_pOpenList[1] = m_pOpenList[numberOfOpenListItems+1];//move the last item in the heap up to slot #1
			v = 1;

			//	Repeat the following until the new item in slot #1 sinks to its proper spot in the heap.
			do
			{
				u = v;		
				if (2*u+1 <= numberOfOpenListItems) //if both children exist
				{
					//Check if the F cost of the parent is greater than each child.
					//Select the lowest of the two children.
					if (m_pFcost[m_pOpenList[u]] >= m_pFcost[m_pOpenList[2*u]]) 
						v = 2*u;
					if (m_pFcost[m_pOpenList[v]] >= m_pFcost[m_pOpenList[2*u+1]]) 
						v = 2*u+1;		
				}
				else
				{
					if (2*u <= numberOfOpenListItems) //if only child #1 exists
					{
						//Check if the F cost of the parent is greater than child #1	
						if (m_pFcost[m_pOpenList[u]] >= m_pFcost[m_pOpenList[2*u]]) 
							v = 2*u;
					}
				}

				if (u != v) //if parent's F is > one of its children, swap them
				{
					temp = m_pOpenList[u];
					m_pOpenList[u] = m_pOpenList[v];
					m_pOpenList[v] = temp;			
				}
				else
					break; // otherwise, exit loop

			}
			while (TRUE); // reorder the binary heap


			//7.Check the adjacent squares. (Its "children" -- these path children
			//	are similar, conceptually, to the binary heap children mentioned
			//	above, but don't confuse them. They are different. Path children
			//	are portrayed in Demo 1 with grey pointers pointing toward
			//	their parents.) Add these adjacent child squares to the open list
			//	for later consideration if appropriate (see various if statements
			//	below).
			for (b = parentYval-1; b <= parentYval+1; b++){
				for (a = parentXval-1; a <= parentXval+1; a++){

					//	If not off the map (do this first to avoid array out-of-bounds errors)
					if (a != -1 && b != -1 && a != m_nMapWidth && b != m_nMapHeight){

						//	If not already on the closed list (items on the closed list have
						//	already been considered and can now be ignored).			
						if (m_pWhichList[a * (m_nMapHeight + 1) + b] != onClosedList) { 

							//	If not a wall/obstacle square.
							if (m_pBlocks [a * m_nMapHeight + b] != unwalkable) { 

								// diagonal move is completely forbidden
								if (a != parentXval && b != parentYval)
									corner = unwalkable;
								else
									corner = walkable;

								//	Don't cut across corners
								//corner = walkable;	
								//if (a == parentXval-1) 
								//{
								//	if (b == parentYval-1)
								//	{
								//		if (m_pBlocks[parentXval-1][parentYval] == unwalkable
								//			|| m_pBlocks[parentXval][parentYval-1] == unwalkable) \
								//			corner = unwalkable;
								//	}
								//	else if (b == parentYval+1)
								//	{
								//		if (m_pBlocks[parentXval][parentYval+1] == unwalkable
								//			|| m_pBlocks[parentXval-1][parentYval] == unwalkable) 
								//			corner = unwalkable; 
								//	}
								//}
								//else if (a == parentXval+1)
								//{
								//	if (b == parentYval-1)
								//	{
								//		if (m_pBlocks[parentXval][parentYval-1] == unwalkable 
								//			|| m_pBlocks[parentXval+1][parentYval] == unwalkable) 
								//			corner = unwalkable;
								//	}
								//	else if (b == parentYval+1)
								//	{
								//		if (m_pBlocks[parentXval+1][parentYval] == unwalkable 
								//			|| m_pBlocks[parentXval][parentYval+1] == unwalkable)
								//			corner = unwalkable; 
								//	}
								//}	
								if (corner == walkable) {

									//	If not already on the open list, add it to the open list.			
									if (m_pWhichList[a * (m_nMapHeight + 1) + b] != onOpenList) 
									{	

										//Create a new open list item in the binary heap.
										newOpenListItemID = newOpenListItemID + 1; //each new item has a unique ID #
										m = numberOfOpenListItems+1;
										m_pOpenList[m] = newOpenListItemID;//place the new open list item (actually, its ID#) at the bottom of the heap
										m_pOpenX[newOpenListItemID] = a;
										m_pOpenY[newOpenListItemID] = b;//record the x and y coordinates of the new item

										//Figure out its G cost
										//if (abs(a-parentXval) == 1 && abs(b-parentYval) == 1)
										//	addedGCost = 14;//cost of going to diagonal squares	
										//else	
											addedGCost = 10;//cost of going to non-diagonal squares				
										m_pGcost[a * (m_nMapHeight + 1) + b] = m_pGcost[parentXval * (m_nMapHeight + 1) + parentYval] + addedGCost;

										//Figure out its H and F costs and parent
										m_pHcost[m_pOpenList[m]] = 10*(abs(a - nTargetX) + abs(b - nTargetY));
										m_pFcost[m_pOpenList[m]] = m_pGcost[a * (m_nMapHeight + 1) + b] + m_pHcost[m_pOpenList[m]];
										m_pParentX[a * (m_nMapHeight + 1) + b] = parentXval ; m_pParentY[a * (m_nMapHeight + 1) + b] = parentYval;	

										//Move the new open list item to the proper place in the binary heap.
										//Starting at the bottom, successively compare to parent items,
										//swapping as needed until the item finds its place in the heap
										//or bubbles all the way to the top (if it has the lowest F cost).
										while (m != 1) //While item hasn't bubbled to the top (m=1)	
										{
											//Check if child's F cost is < parent's F cost. If so, swap them.	
											if (m_pFcost[m_pOpenList[m]] <= m_pFcost[m_pOpenList[m/2]])
											{
												temp = m_pOpenList[m/2];
												m_pOpenList[m/2] = m_pOpenList[m];
												m_pOpenList[m] = temp;
												m = m/2;
											}
											else
												break;
										}
										numberOfOpenListItems = numberOfOpenListItems+1;//add one to the number of items in the heap

										//Change m_pWhichList to show that the new item is on the open list.
										m_pWhichList[a * (m_nMapHeight + 1) + b] = onOpenList;
									}

									//8.If adjacent cell is already on the open list, check to see if this 
									//	path to that cell from the starting location is a better one. 
									//	If so, change the parent of the cell and its G and F costs.	
									else //If m_pWhichList(a,b) = onOpenList
									{

										//Figure out the G cost of this possible new path
										//if (abs(a-parentXval) == 1 && abs(b-parentYval) == 1)
										//	addedGCost = 14;//cost of going to diagonal tiles	
										//else	
											addedGCost = 10;//cost of going to non-diagonal tiles				
										tempGcost = m_pGcost[parentXval * (m_nMapHeight + 1) + parentYval] + addedGCost;

										//If this path is shorter (G cost is lower) then change
										//the parent cell, G cost and F cost. 		
										if (tempGcost < m_pGcost[a * (m_nMapHeight + 1) + b]) //if G cost is less,
										{
											m_pParentX[a * (m_nMapHeight + 1) + b] = parentXval; //change the square's parent
											m_pParentY[a * (m_nMapHeight + 1) + b] = parentYval;
											m_pGcost[a * (m_nMapHeight + 1) + b] = tempGcost;//change the G cost			

											//Because changing the G cost also changes the F cost, if
											//the item is on the open list we need to change the item's
											//recorded F cost and its position on the open list to make
											//sure that we maintain a properly ordered open list.
											for (INT x = 1; x <= numberOfOpenListItems; x++) //look for the item in the heap
											{
												if (m_pOpenX[m_pOpenList[x]] == a && m_pOpenY[m_pOpenList[x]] == b) //item found
												{
													m_pFcost[m_pOpenList[x]] = m_pGcost[a * (m_nMapHeight + 1) + b] + m_pHcost[m_pOpenList[x]];//change the F cost

													//See if changing the F score bubbles the item up from it's current location in the heap
													m = x;
													while (m != 1) //While item hasn't bubbled to the top (m=1)	
													{
														//Check if child is < parent. If so, swap them.	
														if (m_pFcost[m_pOpenList[m]] < m_pFcost[m_pOpenList[m/2]])
														{
															temp = m_pOpenList[m/2];
															m_pOpenList[m/2] = m_pOpenList[m];
															m_pOpenList[m] = temp;
															m = m/2;
														}
														else
															break;
													} 
													break; //exit for x = loop
												} //If m_pOpenX(m_pOpenList(x)) = a
											} //For x = 1 To numberOfOpenListItems
										}//If tempGcost < m_pGcost(a,b)

									}//else If m_pWhichList(a,b) = onOpenList	
								}//If not cutting a corner
							}//If not a wall/obstacle square.
						}//If not already on the closed list 
					}//If not off the map
				}//for (a = parentXval-1; a <= parentXval+1; a++){
			}//for (b = parentYval-1; b <= parentYval+1; b++){

		}//if (numberOfOpenListItems != 0)

		//9.If open list is empty then there is no path.	
		else
		{
			path = nonexistent; break;
		}  

		//If target is added to open list then path has been found.
		if (m_pWhichList[nTargetX * (m_nMapHeight + 1) + nTargetY] == onOpenList)
		{
			path = found; break;
		}

	}
	while (TRUE);//Do until path is found or deemed nonexistent

	//10.Save the path if it exists.
	if (path == found)
	{

		//a.Working backwards from the target to the starting location by checking
		//	each cell's parent, figure out the length of the path.
		pathX = nTargetX; pathY = nTargetY;
		do
		{
			//Look up the parent of the current cell.	
			tempx = m_pParentX[pathX * (m_nMapHeight + 1) + pathY];		
			pathY = m_pParentY[pathX * (m_nMapHeight + 1) + pathY];
			pathX = tempx;

			//Figure out the path length
			m_nPathLength = m_nPathLength + 1;
		}
		while (pathX != startX || pathY != startY);

		//b.Resize the data bank to the right size in bytes
		//INT* pTmp = new INT[m_nPathLength * 2];
		//memcpy(pTmp, m_pPathBank, sizeof(INT) * m_nPathLength * 2);
		//delete m_pPathBank;
		//m_pPathBank = pTmp;
		//m_pPathBank = (INT*) realloc (m_pPathBank, m_nPathLength*8);

		//c. Now copy the path information over to the databank. Since we are
		//	working backwards from the target to the start location, we copy
		//	the information to the data bank in reverse order. The result is
		//	a properly ordered set of path data, from the first step to the
		//	last.
		pathX = nTargetX ; pathY = nTargetY;
		cellPosition = m_nPathLength*2;//start at the end	
		do
		{
			cellPosition = cellPosition - 2;//work backwards 2 integers
			m_pPathBank [cellPosition] = pathX;
			m_pPathBank [cellPosition+1] = pathY;

			//d.Look up the parent of the current cell.	
			tempx = m_pParentX[pathX * (m_nMapHeight + 1) + pathY];		
			pathY = m_pParentY[pathX * (m_nMapHeight + 1) + pathY];
			pathX = tempx;

			//e.If we have reached the starting square, exit the loop.	
		}
		while (pathX != startX || pathY != startY);	

	}
	return path;


	//13.If there is no path to the selected target, set the pathfinder's
	//	m_nPathX and m_nPathY equal to its current location and return that the
	//	path is nonexistent.
noPath:
	m_nPathX = nStartX;
	m_nPathY = nStartY;
	return nonexistent;
}

void KAstarPathFinder::ReadPath() /*If a path exists, read the path data */ /* from the pathbank. */
{
	m_nPathLocation = 1; //set m_nPathLocation to 1st step
	while (m_nPathLocation < m_nPathLength)
	{
		INT a = m_pPathBank [m_nPathLocation*2-2];
		INT b = m_pPathBank [m_nPathLocation*2-1];
		m_nPathLocation = m_nPathLocation + 1;
		m_pWhichList[a * (m_nMapHeight + 1) + b] = 3;//draw dotted path
	}
}

BOOL KAstarPathFinder::IsInPath( INT x, INT y )
{
	m_nPathLocation = 1; //set m_nPathLocation to 1st step
	while (m_nPathLocation < m_nPathLength)
	{
		INT a = m_pPathBank [m_nPathLocation*2-2];
		INT b = m_pPathBank [m_nPathLocation*2-1];
		m_nPathLocation = m_nPathLocation + 1;
		if (a == x && b == y)
			return TRUE;
	} 
	return FALSE;
}

VOID KAstarPathFinder::TestAstar()
{
	//Sleep(3000);
	INT nMapWidth = 64, nMapHeight = 64;
	Init(nMapWidth, nMapHeight);

	m_pBlocks[6 * m_nMapHeight + 6] = unwalkable;
	m_pBlocks[7 * m_nMapHeight + 5] = unwalkable;
	m_pBlocks[2 * m_nMapHeight + 7] = unwalkable;

	// 地图中间超级障碍
	for (INT n = 1; n < nMapWidth/* - 1*/; ++n)
	{
		m_pBlocks[nMapWidth / 2 * nMapHeight + n] = unwalkable;
	}

	//INT nStartX = 2;
	//INT nStartY = 5;
	//INT nTargetX = 8;
	//INT nTargetY = 7;
	INT nStartX = 2;
	INT nStartY = 1;
	INT nTargetX = 2/*nMapWidth - 1*/;
	INT nTargetY = 1/*nMapHeight - 1*/;

	DWORD dwTimeStart = KSysService::GetElapse();
	//for (INT n = 0; n < 10000; ++n)
	//{
	//	ClearResult();
		FindPath(nStartX, nStartY, nTargetX, nTargetY);
	//}
	DWORD dwTimeEnd = KSysService::GetElapse();

	printf("Elapsed Time: %d ms\n", dwTimeEnd - dwTimeStart);

	//for (INT y = 0; y < m_nMapHeight; ++y)
	//{
	//	for (INT x = 0; x < m_nMapWidth; ++x)
	//	{
	//		if ((nStartX == x && nStartY == y) || (nTargetX == x && nTargetY == y))
	//			printf("★");
	//		else if (IsInPath(x, y))
	//			printf("￥");
	//		else if (m_pBlocks[x * m_nMapHeight + y] == unwalkable)
	//			printf("▓");
	//		else
	//			printf("囗");
	//	}
	//	printf("\n");
	//}

	m_nPathLocation = 1; //set m_nPathLocation to 1st step
	while (m_nPathLocation <= m_nPathLength)
	{
		INT a = m_pPathBank [m_nPathLocation*2-2];
		INT b = m_pPathBank [m_nPathLocation*2-1];
		m_nPathLocation = m_nPathLocation + 1;
		printf("->[%d, %d]", a, b);
	}
	printf("\n Cost Energy: %d", m_nPathLength);

	return;
}

BOOL KAstarPathFinder::Init( INT nBattleFieldWidth, INT nBattleFieldHeight )
{
	KGC_CONFIRM_RET_FALSE(nBattleFieldWidth > 0 && nBattleFieldHeight > 0);

	// 保存地图宽高
	m_nMapHeight = nBattleFieldHeight;
	m_nMapWidth = nBattleFieldWidth;

	m_pPathBank = new INT[m_nMapWidth * m_nMapHeight * 2];

	m_pBlocks = new INT[m_nMapWidth * m_nMapHeight];
	m_pWhichList = new INT[(m_nMapWidth+1) * (m_nMapHeight + 1)];
	m_pParentX = new INT[(m_nMapWidth+1) * (m_nMapHeight + 1)];
	m_pParentY = new INT[(m_nMapWidth+1) * (m_nMapHeight + 1)];
	m_pOpenList = new INT[m_nMapWidth * m_nMapHeight + 2];

	m_pOpenX = new INT[m_nMapWidth * m_nMapHeight + 2];
	m_pOpenY = new INT[m_nMapWidth * m_nMapHeight + 2];
	m_pFcost = new INT[m_nMapWidth * m_nMapHeight + 2];
	m_pGcost = new INT[(m_nMapWidth+1) * (m_nMapHeight + 1)];
	m_pHcost = new INT[m_nMapWidth * m_nMapHeight + 2];

	// ClearResult blocking
	memset(m_pBlocks, 0, sizeof(INT) * m_nMapWidth * m_nMapHeight);
	ClearResult();

	return TRUE;
}

BOOL KAstarPathFinder::ClearResult()
{
	onClosedList = 10;
	m_nPathLength = 0;
	m_nPathLocation = 0;

	// 结果清零
	if (m_nMapHeight > 0 && m_nMapWidth > 0)
	{
		memset(m_pPathBank, 0, sizeof(INT) * m_nMapWidth * m_nMapHeight * 2);

		memset(m_pWhichList, 0, sizeof(INT) * (m_nMapWidth+1) * (m_nMapHeight + 1));
		memset(m_pParentX, 0, sizeof(INT) * (m_nMapWidth+1) * (m_nMapHeight + 1));
		memset(m_pParentY, 0, sizeof(INT) * (m_nMapWidth+1) * (m_nMapHeight + 1));
		memset(m_pOpenList, 0, sizeof(INT) * (m_nMapWidth * m_nMapHeight + 2));

		memset(m_pOpenX, 0, sizeof(INT) * (m_nMapWidth * m_nMapHeight + 2));
		memset(m_pOpenY, 0, sizeof(INT) * (m_nMapWidth * m_nMapHeight + 2));
		memset(m_pFcost, 0, sizeof(INT) * (m_nMapWidth * m_nMapHeight + 2));
		memset(m_pGcost, 0, sizeof(INT) * (m_nMapWidth+1) * (m_nMapHeight + 1));
		memset(m_pHcost, 0, sizeof(INT) * (m_nMapWidth * m_nMapHeight + 2));
	}

	return TRUE;
}

BOOL KAstarPathFinder::Uninit()
{
	m_nMapHeight = 0;
	m_nMapWidth = 0;
	m_nPathLength = 0;
	m_nPathLocation = 0;

	KG_DELETE(m_pPathBank);

	KG_DELETE(m_pBlocks);
	KG_DELETE(m_pWhichList);
	KG_DELETE(m_pParentX);
	KG_DELETE(m_pParentY);
	KG_DELETE(m_pOpenList);

	KG_DELETE(m_pOpenX);
	KG_DELETE(m_pOpenY);
	KG_DELETE(m_pFcost);
	KG_DELETE(m_pGcost);
	KG_DELETE(m_pHcost);

	return TRUE;
}

