/* -------------------------------------------------------------------------
//	文件名		：	kastarpathfinder.h
//	创建者		：	simon
//	创建时间	：	2009/12/2 19:58:05
//	功能描述	：	astar algorithm wrapper
// Acknowledgments: P. Lester
// -----------------------------------------------------------------------*/
#ifndef __KASTARPATHFINDER_H__
#define __KASTARPATHFINDER_H__

// -------------------------------------------------------------------------


class KAstarPathFinder
{
public:
	KAstarPathFinder();
	virtual ~KAstarPathFinder();

public:
	BOOL	Init(INT nBattleFieldWidth, INT nBattleFieldHeight);
	BOOL	Uninit();
	BOOL	ClearResult();

	INT		FindPath(INT startingX, INT startingY, INT targetX, INT targetY);

	VOID		ReadPath();

	BOOL	IsInPath(INT x, INT y);

	VOID		TestAstar();

	INT*		m_pBlocks;
	INT m_nPathLength;     //stores length of the found path for critter
	INT m_nPathLocation;   //stores current position along the chosen path for critter		
	INT* m_pPathBank;
	INT GetWidth() const { return m_nMapWidth; }
	INT GetHeight() const { return m_nMapHeight; }
protected:
private:
	INT m_nMapWidth;
	INT m_nMapHeight;
	INT onClosedList;

	static const INT tileSize = 1;
	static const INT notfinished = 0, notStarted = 0;// path-related constants
	static const INT found = 1, nonexistent = 2; 
	static const INT walkable = 0, unwalkable = 1;// m_pBlocks array constants

	//Create needed arrays
	INT*		m_pOpenList; //1 dimensional array holding ID# of open list items
	INT*		m_pWhichList;  //2 dimensional array used to record whether a cell is on the open list or on the closed list.
	INT*		m_pOpenX; //1d array stores the x location of an item on the open list
	INT*		m_pOpenY; //1d array stores the y location of an item on the open list
	INT*		m_pParentX; //2d array to store parent of each cell (x)
	INT*		m_pParentY; //2d array to store parent of each cell (y)
	INT*		m_pFcost;	//1d array to store F cost of a cell on the open list
	INT*		m_pGcost; 	//2d array to store G cost for each cell.
	INT*		m_pHcost;	//1d array to store H cost of a cell on the open list

	//Path reading variables
	INT m_nPathStatus;
	INT m_nPathX;
	INT m_nPathY;

};


extern KAstarPathFinder g_cAstarPathFinder;
// -------------------------------------------------------------------------

#endif /* __KASTARPATHFINDER_H__ */
