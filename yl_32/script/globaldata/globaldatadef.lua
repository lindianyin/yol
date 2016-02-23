if not (MODULE_GAMESERVER or MODULE_GC_SERVER) then
	return;
end

local nId = 0;
local function IdAccumulator(nSegment)
	nId = nSegment or nId + 1;
	return nId;
end

KGOLBALDATA_DEF = {
	
	-- ---------------------------------------------------------------------
	-- 任务系统全局变量
	emKGLOBAL_DATA_TASK_BEGIN									= IdAccumulator(0x00000000),
	emKGLOBAL_DATA_TASK_ACCUM_BEGIN								= IdAccumulator(0x00000001),
	emKGLOBAL_DATA_TASK_ACCUM_END								= IdAccumulator(0x00001000),
	emKGLOBAL_DATA_RECOMMEND_BEGIN								= IdAccumulator(0x00001001),
	emKGLOBAL_DATA_RECOMMEND_END								= IdAccumulator(0x00001015),
	emKGLOBAL_DATA_IS_FIRST_INIT								= IdAccumulator(0x00001016),


	-- ---------------------------------------------------------------------
	-- 社会发展水平
	emKGLOBAL_DATA_SOCIETY_RANK_BEGIN							= IdAccumulator(0x00010000),
	emKGLOBAL_DATA_SOCIETY_RANK_VALUE							= IdAccumulator(),				-- 当前社会发展水平
	emKGLOBAL_DATA_SOCIETY_RANK_VERSION							= IdAccumulator(),				-- 正在统计的社会发展水平版本
	emKGLOBAL_DATA_SOCIETY_RANK_SUM_LOW32						= IdAccumulator(),				-- 已统计出的玩家水平低32位
	emKGLOBAL_DATA_SOCIETY_RANK_SUM_HIGH32						= IdAccumulator(),				-- 已统计出的玩家水平高32位
	emKGLOBAL_DATA_SOCIETY_RANK_CNT								= IdAccumulator(),

	-- 故事系统
	emKGLOBAL_DATA_STORY_BEGIN								= IdAccumulator(0x00020000),
	emKGLOBAL_DATA_STORY_SRV_INIT_TIME							= IdAccumulator(),
	emKGLOBAL_DATA_STORY_END								= IdAccumulator(),
}
