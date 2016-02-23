-- 编写者：lidianyi
-- 版本号：34

Include('script/ai/customfunctions.lua')
Include('script/ai/ai_param.lua')
Include('script/ai/argumentstrings.lua')

local g_AIDebugInfo = {}

-- 自定义附加脚本
local nFirstTime;
local nSecendTime;
local nColdDownTime;
local nCount = 0;

function AI:Setup(ai)
    local state
    local action
    local param = AI.g_AIParam[ai.nAIType]

    -- 注册自定义动作
    ai.RegisterUserAction(2505, 'AI:RenGetTime')
    ai.RegisterUserAction(2509, 'AI:RenColdDownChange')

    -- 初始
    state = ai.NewState(461)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 487)

    -- 记录原点
    action = ai.NewAction(487, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(514)
    g_AIDebugInfo[487] = '[487] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(514, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(560)
    g_AIDebugInfo[514] = '[514] SET_PRIMARY_TIMER(8)'

    -- 设置状态
    action = ai.NewAction(560, AI_ACTION.SET_STATE)
    action.SetParam(489) -- 寻敌
    g_AIDebugInfo[560] = '[560] SetParam(State489)'

    -- 寻敌
    state = ai.NewState(489)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 536)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 485)

    -- 搜索敌人
    action = ai.NewAction(536, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(1)
    action.SetBranch(522, 514)
    g_AIDebugInfo[536] = '[536] SEARCH_ENEMY(1)'

    -- 将目标添加到威胁列表
    action = ai.NewAction(522, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(520)
    g_AIDebugInfo[522] = '[522] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(520, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(531)
    g_AIDebugInfo[520] = '[520] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(531, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(505)
    g_AIDebugInfo[531] = '[531] NPC_INIT_SKILL_CD()'

    -- 获取当前时间
    action = ai.NewAction(505, 2505)
    action.SetBranch(472)
    g_AIDebugInfo[505] = '[505] AI:RenGetTime()'

    -- 设置主定时器
    action = ai.NewAction(472, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(562)
    g_AIDebugInfo[472] = '[472] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(562, AI_ACTION.SET_STATE)
    action.SetParam(484) -- 战斗状态
    g_AIDebugInfo[562] = '[562] SetParam(State484)'

    -- 战斗状态
    state = ai.NewState(484)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 511)

    -- 选择第一威胁
    action = ai.NewAction(511, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(509, 499)
    g_AIDebugInfo[511] = '[511] SELECT_FIRST_THREAT()'

    -- 计算冷却
    action = ai.NewAction(509, 2509)
    action.SetBranch(471, 497)
    g_AIDebugInfo[509] = '[509] AI:RenColdDownChange()'

    -- 释放技能
    action = ai.NewAction(471, AI_ACTION.CAST_SKILL)
    action.SetParam(576, 1)
    action.SetBranch(472, 462)
    g_AIDebugInfo[471] = '[471] CAST_SKILL(576, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(462, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(472, 499)
    g_AIDebugInfo[462] = '[462] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 设置系统无敌
    action = ai.NewAction(499, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(523)
    g_AIDebugInfo[499] = '[499] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(523, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(494, 494)
    g_AIDebugInfo[523] = '[523] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(494, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(80)
    action.SetBranch(564)
    g_AIDebugInfo[494] = '[494] SET_PRIMARY_TIMER(80)'

    -- 设置状态
    action = ai.NewAction(564, AI_ACTION.SET_STATE)
    action.SetParam(500) -- 无敌返回状态
    g_AIDebugInfo[564] = '[564] SetParam(State500)'

    -- 无敌返回状态
    state = ai.NewState(500)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 488)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 488)

    -- 返回
    action = ai.NewAction(488, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(527, 527)
    g_AIDebugInfo[488] = '[488] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(527, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(566)
    g_AIDebugInfo[527] = '[527] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(566, AI_ACTION.SET_STATE)
    action.SetParam(498) -- 返回成功
    g_AIDebugInfo[566] = '[566] SetParam(State498)'

    -- 返回成功
    state = ai.NewState(498)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 515)

    -- 清除DEBUFF
    action = ai.NewAction(515, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(469)
    g_AIDebugInfo[515] = '[515] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(469, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(514)
    g_AIDebugInfo[469] = '[469] SET_SYSTEM_SHEILD(0)'

    -- Npc标准技能选择
    action = ai.NewAction(497, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(521, 472)
    g_AIDebugInfo[497] = '[497] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(521, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(495, 532)
    g_AIDebugInfo[521] = '[521] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(495, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(472, 462)
    g_AIDebugInfo[495] = '[495] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(532, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(568)
    g_AIDebugInfo[532] = '[532] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(568, AI_ACTION.SET_STATE)
    action.SetParam(528) -- 追击状态
    g_AIDebugInfo[568] = '[568] SetParam(State528)'

    -- 追击状态
    state = ai.NewState(528)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 503)

    -- 比较返回点距离
    action = ai.NewAction(503, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(param['AttackRange'])
    action.SetBranch(499, 521, 521)
    g_AIDebugInfo[503] = '[503] RETURN_POSITION_DISTANCE_COMPARE(param[AttackRange])'

    -- 选择事件发起者
    action = ai.NewAction(485, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(522, 499)
    g_AIDebugInfo[485] = '[485] SELECT_EVENT_SRC()'

    ai.SetInitState(461)
end

-- 调试方法
function AI:DebugOnCallAction(action_id)
    Log('[AI] Call action: ' .. g_AIDebugInfo[action_id])
end

-- 本AI图内自定义脚本
function AI:RenGetTime(pNpc, actionData)
    -- 输入自定义动作实现脚本
	nFirstTime = KGameBase.GetTime();
    return 1
end

function AI:RenColdDownChange(npc, actionData)
    -- 输入自定义动作实现脚本
	nSecendTime = KGameBase.GetTime();
	nColdDownTime = ((15 - 2 * nCount >= 3) and (15 - 2 * nCount) or 3);
	if nSecendTime - nFirstTime >= nColdDownTime then
		nFirstTime = nSecendTime;
		nCount = nCount + 1;
    	return 1;
    end
    return 2;
end

