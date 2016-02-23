-- 编写者：lidianyi
-- 版本号：11

Include('script/ai/customfunctions.lua')
Include('script/ai/ai_param.lua')
Include('script/ai/argumentstrings.lua')

local g_AIDebugInfo = {}

-- 自定义附加脚本
local nFirstTime;
local nSecendTime;
local nCount = 0;

function AI:Setup(ai)
    local state
    local action
    local param = AI.g_AIParam[ai.nAIType]

    -- 注册自定义动作
    ai.RegisterUserAction(2036, 'AI:ShuiMoYanGetTime')
    ai.RegisterUserAction(2044, 'AI:ShuiMoYanGetColdDown')

    -- 初始
    state = ai.NewState(1)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 14)

    -- 记录原点
    action = ai.NewAction(14, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(2)
    g_AIDebugInfo[14] = '[14] RECORD_ORIGIN_POSITION()'

    -- 设置主定时器
    action = ai.NewAction(2, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(133)
    g_AIDebugInfo[2] = '[2] SET_PRIMARY_TIMER(16)'

    -- 设置状态
    action = ai.NewAction(133, AI_ACTION.SET_STATE)
    action.SetParam(4) -- 待机状态
    g_AIDebugInfo[133] = '[133] SetParam(State4)'

    -- 待机状态
    state = ai.NewState(4)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 6)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 9)

    -- 搜索敌人
    action = ai.NewAction(6, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param['SearchPlayerDistance'])
    action.SetBranch(11, 2)
    g_AIDebugInfo[6] = '[6] SEARCH_ENEMY(param[SearchPlayerDistance])'

    -- 将目标添加到威胁列表
    action = ai.NewAction(11, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(16)
    g_AIDebugInfo[11] = '[11] ADD_TARGET_TO_THREAT_LIST()'

    -- 记录返回点
    action = ai.NewAction(16, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(18)
    g_AIDebugInfo[16] = '[16] RECORD_RETURN_POSITION()'

    -- Npc初始化技能CD
    action = ai.NewAction(18, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(36)
    g_AIDebugInfo[18] = '[18] NPC_INIT_SKILL_CD()'

    -- 自定义动作36
    action = ai.NewAction(36, 2036)
    action.SetBranch(30)
    g_AIDebugInfo[36] = '[36] AI:ShuiMoYanGetTime()'

    -- 设置主定时器
    action = ai.NewAction(30, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(135)
    g_AIDebugInfo[30] = '[30] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(135, AI_ACTION.SET_STATE)
    action.SetParam(32) -- 进入战斗
    g_AIDebugInfo[135] = '[135] SetParam(State32)'

    -- 进入战斗
    state = ai.NewState(32)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 48)

    -- 选择第一威胁
    action = ai.NewAction(48, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(44, 105)
    g_AIDebugInfo[48] = '[48] SELECT_FIRST_THREAT()'

    -- 自定义动作44
    action = ai.NewAction(44, 2044)
    action.SetBranch(50, 52)
    g_AIDebugInfo[44] = '[44] AI:ShuiMoYanGetColdDown()'

    -- 释放技能
    action = ai.NewAction(50, AI_ACTION.CAST_SKILL)
    action.SetParam(555, 1)
    action.SetBranch(78, 68)
    g_AIDebugInfo[50] = '[50] CAST_SKILL(555, 1)'

    -- 检查技能失败计数
    action = ai.NewAction(68, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(30, 105)
    g_AIDebugInfo[68] = '[68] CHECK_SKILL_FAILED_COUNTER(3)'

    -- 设置系统无敌
    action = ai.NewAction(105, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(106)
    g_AIDebugInfo[105] = '[105] SET_SYSTEM_SHEILD(1)'

    -- 返回
    action = ai.NewAction(106, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(96, 96)
    g_AIDebugInfo[106] = '[106] RETURN(0)'

    -- 设置主定时器
    action = ai.NewAction(96, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(80)
    action.SetBranch(137)
    g_AIDebugInfo[96] = '[96] SET_PRIMARY_TIMER(80)'

    -- 设置状态
    action = ai.NewAction(137, AI_ACTION.SET_STATE)
    action.SetParam(110) -- 无敌返回状态
    g_AIDebugInfo[137] = '[137] SetParam(State110)'

    -- 无敌返回状态
    state = ai.NewState(110)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 91)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 91)

    -- 返回
    action = ai.NewAction(91, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(107, 107)
    g_AIDebugInfo[91] = '[91] RETURN(1)'

    -- 设置主定时器
    action = ai.NewAction(107, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(139)
    g_AIDebugInfo[107] = '[107] SET_PRIMARY_TIMER(32)'

    -- 设置状态
    action = ai.NewAction(139, AI_ACTION.SET_STATE)
    action.SetParam(100) -- 返回成功
    g_AIDebugInfo[139] = '[139] SetParam(State100)'

    -- 返回成功
    state = ai.NewState(100)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 94)

    -- 清除DEBUFF
    action = ai.NewAction(94, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(104)
    g_AIDebugInfo[94] = '[94] CLEAN_DEBUFF()'

    -- 设置系统无敌
    action = ai.NewAction(104, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(2)
    g_AIDebugInfo[104] = '[104] SET_SYSTEM_SHEILD(0)'

    -- 设置副定时器
    action = ai.NewAction(78, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(80)
    action.SetBranch(141)
    g_AIDebugInfo[78] = '[78] SET_SECONDARY_TIMER(80)'

    -- 设置状态
    action = ai.NewAction(141, AI_ACTION.SET_STATE)
    action.SetParam(84) -- 吟唱状态
    g_AIDebugInfo[141] = '[141] SetParam(State84)'

    -- 吟唱状态
    state = ai.NewState(84)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 116)
    state.HandleEvent(AI_EVENT.ON_CAST_SKILL, 116)
    state.HandleEvent(AI_EVENT.ON_BREAK_PREPARE, 116)
    state.HandleEvent(AI_EVENT.ON_CAST_SKILL_FAILED, 68)

    -- 站立
    action = ai.NewAction(116, AI_ACTION.STAND)
    action.SetBranch(30)
    g_AIDebugInfo[116] = '[116] STAND()'

    -- Npc标准技能选择
    action = ai.NewAction(52, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(55, 30)
    g_AIDebugInfo[52] = '[52] NPC_STANDARD_SKILL_SELECTOR()'

    -- Npc保持技能释放距离
    action = ai.NewAction(55, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(57, 59)
    g_AIDebugInfo[55] = '[55] NPC_KEEP_SKILL_CAST_RANGE()'

    -- Npc释放选定技能
    action = ai.NewAction(57, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(30, 68)
    g_AIDebugInfo[57] = '[57] NPC_CAST_SELECT_SKILL()'

    -- 设置主定时器
    action = ai.NewAction(59, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(4)
    action.SetBranch(143)
    g_AIDebugInfo[59] = '[59] SET_PRIMARY_TIMER(4)'

    -- 设置状态
    action = ai.NewAction(143, AI_ACTION.SET_STATE)
    action.SetParam(61) -- 追击状态
    g_AIDebugInfo[143] = '[143] SetParam(State61)'

    -- 追击状态
    state = ai.NewState(61)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 72)

    -- 比较返回点距离
    action = ai.NewAction(72, AI_ACTION.RETURN_POSITION_DISTANCE_COMPARE)
    action.SetParam(550)
    action.SetBranch(105, 55, 55)
    g_AIDebugInfo[72] = '[72] RETURN_POSITION_DISTANCE_COMPARE(550)'

    -- 选择事件发起者
    action = ai.NewAction(9, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(11, 105)
    g_AIDebugInfo[9] = '[9] SELECT_EVENT_SRC()'

    ai.SetInitState(1)
end

-- 调试方法
function AI:DebugOnCallAction(action_id)
    Log('[AI] Call action: ' .. g_AIDebugInfo[action_id])
end

-- 本AI图内自定义脚本
function AI:ShuiMoYanGetTime(pNpc, actionData)
    -- 输入自定义动作实现脚本
	nFirstTime = KGameBase.GetTime();
    return 1
end

function AI:ShuiMoYanGetColdDown(pNpc, actionData)
    -- 输入自定义动作实现脚本
	nSecendTime = KGameBase.GetTime();
	if nSecendTime - nFirstTime >= (120 - 30 * nCount) then
		nFirstTime = nSecendTime;
		nCount = 1;
		return 1;
	end
    return 2;
end

