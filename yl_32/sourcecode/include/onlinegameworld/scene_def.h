
#pragma once

#include "Core.h"
#include "config/gamedef.h"


#define DIRECTION_BIT_NUM       8
#define DIRECTION_COUNT			(1 << DIRECTION_BIT_NUM)
#define DIRECTION_TURN_SPEED    5

enum MOVE_MODE
{
	mmInvalid = 0,
	mmGround,
	mmWater,
	mmGroundAndWater,

	mmTotal
};

//杂项
#define DEFAULT_MASS			128			    // 单位：g
#define DEFAULT_GRAVITY			(8 * 512 / GAME_FPS / GAME_FPS)	//单位：点/帧^2
//#define SYNC_CHARACTER_DELAY		1	// 同步延时  // 延迟一帧会造成同步问题，改为0
#define SYNC_CHARACTER_DELAY		0	// 同步延时
#define MAX_SYNC_PER_REGION		32	            // 同步量控制参数
#define KD_NEARBY_OBJ_SYNC_MAX		300	            // 一次最多同步多少个附近玩家
#define	KD_SYNC_PACK_MAX		64 * 1024		// 同步缓存最大值

#pragma pack(1)
struct KMOVE_CRITICAL_PARAM
{
	BYTE byMoveState;
	int  nRunSpeed;
	int  nGravity;
	int  nJumpSpeed;
	int  nX;
	int  nY;
	int  nZ;
};
#pragma pack()

#define ID_PREFIX_BIT_NUM	4

// 动态障碍分组的无效值(之所以是0,而不是-1,是为了兼容以前的数据)
#define INVALID_OBSTACLE_GROUP  0
// 有效的动态障碍分组索引号取值范围(0, 4096)
#define MAX_OBSTACLE_GROUP      4096

// 场景和游戏世界的空间划分大小定义
#define MAX_REGION_WIDTH_BIT_NUM	6	//地图中X坐标上最多的Region个数
#define MAX_REGION_HEIGHT_BIT_NUM	6	//地图中Y坐标上最多的Region个数

#define REGION_GRID_WIDTH_BIT_NUM	6	//Region中X坐标上最多的Cell个数
#define REGION_GRID_HEIGHT_BIT_NUM	6	//Region中Y坐标上最多的Cell个数

#define CELL_LENGTH_BIT_NUM			5	//格子中的象素点精度
#define ALTITUDE_BIT_NUM			6

#define MAX_Z_ALTITUDE_BIT_NUM		16	//Z轴的最大值位数(ALTITUDE)
#define MAX_Z_POINT_BIT_NUM         (MAX_Z_ALTITUDE_BIT_NUM + ALTITUDE_BIT_NUM)

#define MOVE_DEST_RANGE_BIT_NUM		12
#define MOVE_DEST_RANGE				(1 << MOVE_DEST_RANGE_BIT_NUM)

#define	MAX_VELOCITY_XY_BIT_NUM		(CELL_LENGTH_BIT_NUM + 2)
#define MAX_VELOCITY_Z_BIT_NUM		(ALTITUDE_BIT_NUM + 6)
// 用于水平速度收敛计算时消除计算误差,注意,这个值最好不要小于收敛计算时的分母数值
#define VELOCITY_ZOOM_BIT_NUM      4
#define VELOCITY_ZOOM_COEFFICIENT (1 << VELOCITY_ZOOM_BIT_NUM)

#define MAX_ZOOM_VELOCITY_BIT_NUM   (MAX_VELOCITY_XY_BIT_NUM + VELOCITY_ZOOM_BIT_NUM)
#define MAX_ZOOM_VELOCITY           ((1 << MAX_ZOOM_VELOCITY_BIT_NUM) - 1)

#define MAX_VELOCITY_XY				((1 << MAX_VELOCITY_XY_BIT_NUM) - 1)
// 注意,Z轴速度是有符号,可以为负值的,目前取值区间为[-512, 511], 10 bits
#define MAX_VELOCITY_Z				((1 << (MAX_VELOCITY_Z_BIT_NUM - 1)) - 1)
#define MIN_VELOCITY_Z				(-(1 << (MAX_VELOCITY_Z_BIT_NUM - 1)))

#define MAX_X_COORDINATE_BIT        (MAX_REGION_WIDTH_BIT_NUM  + REGION_GRID_WIDTH_BIT_NUM  + CELL_LENGTH_BIT_NUM)
#define MAX_Y_COORDINATE_BIT        (MAX_REGION_HEIGHT_BIT_NUM + REGION_GRID_HEIGHT_BIT_NUM + CELL_LENGTH_BIT_NUM)
#define MAX_X_COORDINATE            ((1 << MAX_X_COORDINATE_BIT) - 1)
#define MAX_Y_COORDINATE            ((1 << MAX_Y_COORDINATE_BIT) - 1)

//坐标相关定义
#define MAX_REGION_WIDTH			(1 << MAX_REGION_WIDTH_BIT_NUM)
#define MAX_REGION_HEIGHT			(1 << MAX_REGION_HEIGHT_BIT_NUM)
#define MAX_Z_ALTITUDE				((1 << MAX_Z_ALTITUDE_BIT_NUM) - 1)
#define MAX_Z_POINT				    ((1 << MAX_Z_POINT_BIT_NUM) - 1)

//#define REGION_GRID_WIDTH			(1 << REGION_GRID_WIDTH_BIT_NUM)
//#define REGION_GRID_HEIGHT			(1 << REGION_GRID_HEIGHT_BIT_NUM)

//region 大小为 64 * 64 //change to 32*32
#define REGION_GRID_WIDTH			(1 << REGION_GRID_WIDTH_BIT_NUM)
#define REGION_GRID_HEIGHT			(1 << REGION_GRID_HEIGHT_BIT_NUM)


#define CELL_LENGTH					(1 << CELL_LENGTH_BIT_NUM)
#define POINT_PER_ALTITUDE			(1 << ALTITUDE_BIT_NUM)
#define ALTITUDE_2_CELL				(POINT_PER_ALTITUDE * LOGICAL_CELL_CM_LENGTH/ ALTITUDE_UNIT / CELL_LENGTH)

#define MAX_GRAVITY_BIT_NUM          5
#define MAX_GRAVITY                 ((1 << MAX_GRAVITY_BIT_NUM) - 1)

#define LOGICAL_CELL_CM_LENGTH		50
#define _3D_CELL_CM_LENGTH			100

// 地表高度计量单位(厘米)
#define ALTITUDE_UNIT				((LOGICAL_CELL_CM_LENGTH) / 4.0f)
#define TERRAIN_MIN_HEIGHT			(-65536.0f / 4.0f * ALTITUDE_UNIT)
#define TERRAIN_MAX_HEIGHT			(65536.0f / 4.0f * ALTITUDE_UNIT)

#define ALTITUDE_TO_XYPOINT(Altitude)   ((Altitude) * 8)
#define ZPOINT_TO_XYPOINT(Z)            ((Z) / 8)
#define XYPOINT_TO_ZPOINT(XY)           ((XY) * 8)
#define ZPOINT_TO_ALTITUDE(Z)           ((Z) / POINT_PER_ALTITUDE)

// 陡坡坡度(45度)
#define KD_STEEP_SLOPE_GRADIENT	(DIRECTION_COUNT / 8)
// 陡坡方向与速度方向相差多少内允许通过
#define KD_STEEP_SLOPE_DIRECTION	(DIRECTION_COUNT / 4)


#define KD_PHYSICS_EARTH_GRAVITY		(DEFAULT_GRAVITY * 98 / 80)	// 地球上的重力加速G=9.8m/s^2
#define KD_BASE_JUMP_SPEED				200 // 计算得到G=9.8时跳跃高度2m的数据
#define KD_BASE_RUN_SPEED					20 // 按8m/s = 32p/f换算
#define KD_BASE_SWIM_SPEED              12
#define KD_BASE_WALK_SPEED				8

// 爬坡障碍高度差
#define CLIMB_ABILITY 	    (XYPOINT_TO_ZPOINT(CELL_LENGTH))
// 自动转跳跃高度
#define CRITICAL_JUMP_HEIGHT        XYPOINT_TO_ZPOINT(CELL_LENGTH * 3)

// 主角移动时定位前方目标点的距离,30米
#define MOVE_TARGET_RANGE   (30 * 100 * 32 / 50)

#define MAX_MOVE_STATE_BIT_NUM  5
#define MAX_MOVE_STATE_VALUE    ((1 << MAX_MOVE_STATE_BIT_NUM) - 1)

#define MAX_JUMP_COUNT_BIT 2
#define MAX_JUMP_COUNT ((1 << MAX_JUMP_COUNT_BIT) - 1)

// 最大MapID,MapID取值范围(0, MAX_MAP_ID]
#define MAX_MAP_ID 100

// 合成的动态地图Id
#define DYNAMIC_SCENE_ID(nMapId, nMapCopyIndex) ((nMapId) | (nMapCopyIndex) << 16)

#define GET_SWIM_WATER_LINE(nHeight) ((nHeight) * 7 / 10)
#define IS_UNDER_WATER(nWaterLine, nCharacterHeight) ((nWaterLine) >= (nCharacterHeight) * 9 / 10)

// 地图障碍和场景物体
#define	SCENE_DIRECTORY		"scene"			// 目录
#define	KD_REGION_FILE_EXT			".rgn"			// 后缀
#define	SCENE_LIST_FILE			"/scene/scenelist.txt"	// 地图列表(暂时放这里以后移到setting)
#define	MAP_GROUP_FILE			"/setting/scene/mapgroup.txt"
#define KD_SCENE_NPC_FILE			"npc.txt" // 场景npc表文件
#define KD_SCENE_TRAP_FILE			"trap.txt" // 场景trap表文件
#define KD_SCENE_DOODAD_FILE	"doodad.txt" // 场景doodad文件
#define KD_SCENE_NAVFILE_EXT		".nav"		//导航文件后缀


//地图标记的注释长度
#define MAX_MAP_MARK_COMMENT_LEN 32
#define MAX_MIDMAP_MARK_COMMENT_LEN 32

struct KRegionTrapScript 
{
	CHAR szScript[MAX_NAME_LEN];
};

#define MAKE_IN_RANGE(Value, MinValue, MaxValue)        \
	do                                                  \
	{                                                   \
	ASSERT(MinValue <= MaxValue);                   \
	if ((Value) > (MaxValue)) (Value) = (MaxValue); \
	if ((Value) < (MinValue)) (Value) = (MinValue); \
	}                                                   \
	while (false)


//#ifdef GAME_CLIENT
// 关于坐标转换函数：
// 禁止在服务端调用，禁止在逻辑中使用3D引擎坐标
// 只可以在客户端逻辑<->表现坐标传递的接口处使用
inline BOOL	Logic3DToRepresent3D(INT& rnX, INT& rnY, INT& rnZ)
{
	rnX = rnX * LOGICAL_CELL_CM_LENGTH / CELL_LENGTH;
	rnY = rnY * LOGICAL_CELL_CM_LENGTH / CELL_LENGTH;
	rnZ = INT(rnZ * ALTITUDE_UNIT / POINT_PER_ALTITUDE + TERRAIN_MIN_HEIGHT);

	INT nTmp = rnY;
	rnY = rnZ;
	rnZ = nTmp;

	return TRUE;
}

inline BOOL	Represent3DToLogic3D(INT& rnX, INT& rnY, INT& rnZ)
{
	rnX = rnX * CELL_LENGTH / LOGICAL_CELL_CM_LENGTH;
	rnY = (INT)((rnY - TERRAIN_MIN_HEIGHT) * POINT_PER_ALTITUDE / ALTITUDE_UNIT);
	rnZ = rnZ * CELL_LENGTH / LOGICAL_CELL_CM_LENGTH;

	INT nTmp = rnY;
	rnY = rnZ;
	rnZ = nTmp;

	return TRUE;
}
// 点每帧 ==》米每秒
inline INT LogicSpeedToRepresentSpeed(INT nSpeed)
{
	return nSpeed * GAME_FPS * LOGICAL_CELL_CM_LENGTH / CELL_LENGTH / 100;
}
// 米每秒 ==》点每帧
inline INT RepresentSpeedToLogicSpeed(INT nSpeed)
{
	return nSpeed * 100 * CELL_LENGTH / LOGICAL_CELL_CM_LENGTH / GAME_FPS;
}
//#endif // GAME_CLIENT

// -------------------------------------------------------------------------
