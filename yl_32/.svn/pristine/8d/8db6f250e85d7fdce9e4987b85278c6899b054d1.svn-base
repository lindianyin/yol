-- FILE        : standardai.lua
-- SOURCE      : standardai.vsd
-- AUTHOR      : simon
-- DESCRIPTION :
-- KEYWORDS    :

Include("script/ai/ai_param.lua")
--Include("script/ai/standardaicustom.lua")

-- LOGIC-DEFINE SECTION ----------------------------------------

local g_AIDebugInfo = {}

function AI:ClearSelection()
	return 1
end

function AI:Setup(ai)
    local state
    local action
    local param = AI.g_AIParam[ai.nAIType]
    AI[ai.nAIType] = g_AIDebugInfo

    -- REGISTER CUSTOM ACTIONS ---------------------------------

    ai.RegisterUserAction(1001, "ClearSelection")

    -- DEFINE ACTIONS ------------------------------------------

    action = ai.NewAction(772, AI_ACTION.CHECK_SKILL_FAILED_COUNTER)
    action.SetParam(3)
    action.SetBranch(115,172)
    g_AIDebugInfo[772] = "[772]CHECK_SKILL_FAILED_COUNTER(3)"

    -- 是否是主动怪
    action = ai.NewAction(79, AI_ACTION.PARAM_COMPARE)
    action.SetParam(param.IsPositive, 1)
    action.SetBranch(25,25,14)
    g_AIDebugInfo[79] = "[79]PARAM_COMPARE(param.IsPositive, 1)"

    action = ai.NewAction(462, AI_ACTION.SELECT_STATE)
    action.SetParam(param.WanderRange)
    action.SetBranch(303,520,615)
    g_AIDebugInfo[462] = "[462]SELECT_STATE(param.WanderRange)"

    action = ai.NewAction(90, AI_ACTION.NEED_ESCAPE)
    action.SetParam(param.EscapeRange, param.EscapeLife)
    action.SetBranch(219,274)
    g_AIDebugInfo[90] = "[90]NEED_ESCAPE(param.EscapeRange, param.EscapeLife)"

    -- 警戒范围内搜索
    action = ai.NewAction(378, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param.AlertRange)
    action.SetBranch(353,462)
    g_AIDebugInfo[378] = "[378]SEARCH_ENEMY(param.AlertRange)"

    action = ai.NewAction(50, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param.AlertRange)
    action.SetBranch(360,14)
    g_AIDebugInfo[50] = "[50]SEARCH_ENEMY(param.AlertRange)"

    action = ai.NewAction(58, AI_ACTION.NPC_INIT_SKILL_CD)
    action.SetBranch(115)
    g_AIDebugInfo[58] = "[58]NPC_INIT_SKILL_CD()"

    -- 正常AI间隔
    action = ai.NewAction(14, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(24)
    g_AIDebugInfo[14] = "[14]SET_PRIMARY_TIMER(16)"

    action = ai.NewAction(8, AI_ACTION.RECORD_ORIGIN_POSITION)
    action.SetBranch(1074)
    g_AIDebugInfo[8] = "[8]RECORD_ORIGIN_POSITION()"

    action = ai.NewAction(51, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(601)
    g_AIDebugInfo[51] = "[51]RECORD_RETURN_POSITION()"

    action = ai.NewAction(254, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(261,51)
    g_AIDebugInfo[254] = "[254]SELECT_EVENT_SRC()"

    action = ai.NewAction(261, AI_ACTION.ADD_TARGET_TO_THREAT_LIST)
    action.SetBranch(51)
    g_AIDebugInfo[261] = "[261]ADD_TARGET_TO_THREAT_LIST()"

    -- 选择目标失败就是没有仇恨了，结束战斗
    action = ai.NewAction(83, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(901,172)
    g_AIDebugInfo[83] = "[83]SELECT_FIRST_THREAT()"

    action = ai.NewAction(108, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(param.EscapeInterval)
    action.SetBranch(321)
    g_AIDebugInfo[108] = "[108]SET_PRIMARY_TIMER(param.EscapeInterval)"

    -- 战斗状态AI间隔
    action = ai.NewAction(115, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(1)
    action.SetBranch(125)
    g_AIDebugInfo[115] = "[115]SET_PRIMARY_TIMER(8)"

    action = ai.NewAction(149, AI_ACTION.KEEP_ORIGIN_DIRECTION)
    action.SetBranch(984)
    g_AIDebugInfo[149] = "[149]KEEP_ORIGIN_DIRECTION()"

    action = ai.NewAction(172, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(1)
    action.SetBranch(179)
    g_AIDebugInfo[172] = "[172]SET_SYSTEM_SHEILD(1)"

    action = ai.NewAction(179, AI_ACTION.RETURN)
    action.SetParam(0)
    action.SetBranch(187,187)
    g_AIDebugInfo[179] = "[179]RETURN(0)"

    -- 返回超时
    action = ai.NewAction(187, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(160)
    action.SetBranch(133)
    g_AIDebugInfo[187] = "[187]SET_PRIMARY_TIMER(160)"

    action = ai.NewAction(194, AI_ACTION.RETURN)
    action.SetParam(1)
    action.SetBranch(149,149)
    g_AIDebugInfo[194] = "[194]RETURN(1)"

    -- 返回后两秒不打人
    action = ai.NewAction(202, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(32)
    action.SetBranch(161)
    g_AIDebugInfo[202] = "[202]SET_PRIMARY_TIMER(32)"

    action = ai.NewAction(209, AI_ACTION.SET_SYSTEM_SHEILD)
    action.SetParam(0)
    action.SetBranch(462)
    g_AIDebugInfo[209] = "[209]SET_SYSTEM_SHEILD(0)"

    action = ai.NewAction(219, AI_ACTION.NPC_STANDARD_SKILL_SELECTOR)
    action.SetBranch(225,998)
    g_AIDebugInfo[219] = "[219]NPC_STANDARD_SKILL_SELECTOR()"

    action = ai.NewAction(225, AI_ACTION.NPC_KEEP_SKILL_CAST_RANGE)
    action.SetBranch(954,115)
    g_AIDebugInfo[225] = "[225]NPC_KEEP_SKILL_CAST_RANGE()"

    action = ai.NewAction(231, AI_ACTION.NPC_CAST_SELECT_SKILL)
    action.SetBranch(532,772)
    g_AIDebugInfo[231] = "[231]NPC_CAST_SELECT_SKILL()"

    action = ai.NewAction(274, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(param.EscapeTime)
    action.SetBranch(322)
    g_AIDebugInfo[274] = "[274]SET_SECONDARY_TIMER(param.EscapeTime)"

    action = ai.NewAction(25, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param.SearchPlayerDistance)
    action.SetBranch(261,50)
    g_AIDebugInfo[25] = "[25]SEARCH_ENEMY(param.SearchPlayerDistance)"

    -- 如果手工添加了仇恨，就打仇恨表里的
    action = ai.NewAction(342, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(261,79)
    g_AIDebugInfo[342] = "[342]SELECT_FIRST_THREAT()"

    action = ai.NewAction(353, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(375)
    g_AIDebugInfo[353] = "[353]SET_PRIMARY_TIMER(8)"

    action = ai.NewAction(360, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(param.AlertOverTime)
    action.SetBranch(397)
    g_AIDebugInfo[360] = "[360]SET_SECONDARY_TIMER(param.AlertOverTime)"

    action = ai.NewAction(387, AI_ACTION.FACE_TARGET)
    action.SetParam(1)
    action.SetBranch(353)
    g_AIDebugInfo[387] = "[387]FACE_TARGET(1)"

    action = ai.NewAction(397, AI_ACTION.STAND)
    action.SetBranch(353)
    g_AIDebugInfo[397] = "[397]STAND()"

    action = ai.NewAction(415, AI_ACTION.NPC_OPEN_DIALOG_WINDOW)
    action.SetBranch(587)
    g_AIDebugInfo[415] = "[415]NPC_OPEN_DIALOG_WINDOW()"

    action = ai.NewAction(420, AI_ACTION.STAND)
    action.SetBranch(435)
    g_AIDebugInfo[420] = "[420]STAND()"

    action = ai.NewAction(427, AI_ACTION.FACE_TARGET)
    action.SetParam(param.IsAutoTurn)
    action.SetBranch(415)
    g_AIDebugInfo[427] = "[427]FACE_TARGET(param.IsAutoTurn)"

    action = ai.NewAction(435, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(427,415)
    g_AIDebugInfo[435] = "[435]SELECT_EVENT_SRC()"

    action = ai.NewAction(452, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(459)
    g_AIDebugInfo[452] = "[452]SET_PRIMARY_TIMER(16)"

    -- 设置闲逛的间隔
    action = ai.NewAction(484, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(param.WanderInterval)
    action.SetBranch(342)
    g_AIDebugInfo[484] = "[484]SET_SECONDARY_TIMER(param.WanderInterval)"

    action = ai.NewAction(520, AI_ACTION.WANDER)
    action.SetParam(param.WanderRange, param.WanderDistance)
    action.SetBranch(484)
    g_AIDebugInfo[520] = "[520]WANDER(param.WanderRange, param.WanderDistance)"

    action = ai.NewAction(303, AI_ACTION.DO_PATROL)
    action.SetBranch(14,14)
    g_AIDebugInfo[303] = "[303]DO_PATROL()"

    action = ai.NewAction(404, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(16)
    action.SetBranch(14)
    g_AIDebugInfo[404] = "[404]SET_SECONDARY_TIMER(16)"

    -- 20秒后恢复原来的朝向
    action = ai.NewAction(244, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(320)
    action.SetBranch(452)
    g_AIDebugInfo[244] = "[244]SET_SECONDARY_TIMER(320)"

    action = ai.NewAction(288, AI_ACTION.SELECT_FIRST_THREAT)
    action.SetBranch(261,452)
    g_AIDebugInfo[288] = "[288]SELECT_FIRST_THREAT()"

    action = ai.NewAction(548, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(569)
    g_AIDebugInfo[548] = "[548]SET_PRIMARY_TIMER(16)"

    action = ai.NewAction(322, AI_ACTION.RANDOM_RUN)
    action.SetParam(param.EscapeRange)
    action.SetBranch(108)
    g_AIDebugInfo[322] = "[322]RANDOM_RUN(param.EscapeRange)"

    action = ai.NewAction(386, AI_ACTION.RANDOM_RUN)
    action.SetParam(1280)
    action.SetBranch(548)
    g_AIDebugInfo[386] = "[386]RANDOM_RUN(1280)"

    action = ai.NewAction(581, AI_ACTION.CLEAN_DEBUFF)
    action.SetBranch(209)
    g_AIDebugInfo[581] = "[581]CLEAN_DEBUFF()"

    -- 不自动转向的NPC，不会维持20s的对话状态
    action = ai.NewAction(587, AI_ACTION.PARAM_COMPARE)
    action.SetParam(param.IsAutoTurn, 0)
    action.SetBranch(244,462,462)
    g_AIDebugInfo[587] = "[587]PARAM_COMPARE(param.IsAutoTurn, 0)"

    action = ai.NewAction(601, AI_ACTION.STAND)
    action.SetBranch(58)
    g_AIDebugInfo[601] = "[601]STAND()"

    action = ai.NewAction(606, AI_ACTION.FACE_TARGET)
    action.SetParam(1)
    action.SetBranch(58)
    g_AIDebugInfo[606] = "[606]FACE_TARGET(1)"

    action = ai.NewAction(615, AI_ACTION.KEEP_ORIGIN_DIRECTION)
    action.SetBranch(404)
    g_AIDebugInfo[615] = "[615]KEEP_ORIGIN_DIRECTION()"

    action = ai.NewAction(681, AI_ACTION.STAND)
    action.SetBranch(231)
    g_AIDebugInfo[681] = "[681]STAND()"

    action = ai.NewAction(687, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(386)
    g_AIDebugInfo[687] = "[687]RECORD_RETURN_POSITION()"

    action = ai.NewAction(733, AI_ACTION.SEARCH_ENEMY)
    action.SetParam(param.SearchPlayerDistance)
    action.SetBranch(261,378)
    g_AIDebugInfo[733] = "[733]SEARCH_ENEMY(param.SearchPlayerDistance)"

    action = ai.NewAction(782, AI_ACTION.KEEP_RANGE)
    action.SetParam(32, param.AttackRange)  --修改
    action.SetBranch(962,508)
    g_AIDebugInfo[782] = "[782]KEEP_RANGE(32, param.AttackRange)"

    -- 战斗状态AI间隔
    action = ai.NewAction(508, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(515)
    g_AIDebugInfo[508] = "[508]SET_PRIMARY_TIMER(8)"

    action = ai.NewAction(810, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(818)
    g_AIDebugInfo[810] = "[810]SET_PRIMARY_TIMER(16)"

    action = ai.NewAction(827, AI_ACTION.KEEP_RANGE)
    action.SetParam(32, param.AttackRange) --修改
    action.SetBranch(970,810)
    g_AIDebugInfo[827] = "[827]KEEP_RANGE(32, param.AttackRange)"

    action = ai.NewAction(532, AI_ACTION.NPC_SET_SEC_TIMER_BY_SKILL_ANI)
    action.SetBranch(797,115)
    g_AIDebugInfo[532] = "[532]NPC_SET_SEC_TIMER_BY_SKILL_ANI()"

    action = ai.NewAction(105, AI_ACTION.NPC_SET_SEC_TIMER_BY_SKILL_REST)
    action.SetBranch(1006,83)
    g_AIDebugInfo[105] = "[105]NPC_SET_SEC_TIMER_BY_SKILL_REST()"

    action = ai.NewAction(889, AI_ACTION.NPC_CLEAR_SELECT_SKILL_CD)
    action.SetBranch(83)
    g_AIDebugInfo[889] = "[889]NPC_CLEAR_SELECT_SKILL_CD()"

    action = ai.NewAction(894, AI_ACTION.NPC_RESET_SELECT_SKILL_CD)
    action.SetBranch(532)
    g_AIDebugInfo[894] = "[894]NPC_RESET_SELECT_SKILL_CD()"

    action = ai.NewAction(901, AI_ACTION.IS_TARGE_IN_WATER)
    action.SetBranch(923,90)
    g_AIDebugInfo[901] = "[901]IS_TARGE_IN_WATER()"

    action = ai.NewAction(937, AI_ACTION.IS_TARGE_IN_WATER)
    action.SetBranch(916,90)
    g_AIDebugInfo[937] = "[937]IS_TARGE_IN_WATER()"

    action = ai.NewAction(854, AI_ACTION.IS_TARGET_EXIST)
    action.SetBranch(827,810)
    g_AIDebugInfo[854] = "[854]IS_TARGET_EXIST()"

    action = ai.NewAction(916, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(16)
    action.SetBranch(933)
    g_AIDebugInfo[916] = "[916]SET_PRIMARY_TIMER(16)"

    action = ai.NewAction(923, AI_ACTION.SET_SECONDARY_TIMER)
    action.SetParam(80)
    action.SetBranch(916)
    g_AIDebugInfo[923] = "[923]SET_SECONDARY_TIMER(80)"

    action = ai.NewAction(936, AI_ACTION.IS_TARGET_EXIST)
    action.SetBranch(937,83)
    g_AIDebugInfo[936] = "[936]IS_TARGET_EXIST()"

    action = ai.NewAction(954, AI_ACTION.FACE_TARGET)
    action.SetParam(1)
    action.SetBranch(681)
    g_AIDebugInfo[954] = "[954]FACE_TARGET(1)"

    action = ai.NewAction(962, AI_ACTION.FACE_TARGET)
    action.SetParam(1)
    action.SetBranch(508)
    g_AIDebugInfo[962] = "[962]FACE_TARGET(1)"

    action = ai.NewAction(970, AI_ACTION.FACE_TARGET)
    action.SetParam(1)
    action.SetBranch(810)
    g_AIDebugInfo[970] = "[970]FACE_TARGET(1)"

    action = ai.NewAction(978, AI_ACTION.GROUP_REVIVE)
    action.SetBranch(202)
    g_AIDebugInfo[978] = "[978]GROUP_REVIVE()"

    -- 判断是否要重生所在组
    action = ai.NewAction(984, AI_ACTION.PARAM_COMPARE)
    action.SetParam(param.ReviveTeammate, 1)
    action.SetBranch(978,978,202)
    g_AIDebugInfo[984] = "[984]PARAM_COMPARE(param.ReviveTeammate, 1)"

    action = ai.NewAction(998, AI_ACTION.IS_NEED_KEEP_RANGE)
    action.SetBranch(782,115)
    g_AIDebugInfo[998] = "[998]IS_NEED_KEEP_RANGE()"

    action = ai.NewAction(1006, AI_ACTION.IS_NEED_KEEP_RANGE)
    action.SetBranch(854,810)
    g_AIDebugInfo[1006] = "[1006]IS_NEED_KEEP_RANGE()"

    action = ai.NewAction(1014, AI_ACTION.ENTER_PATROL_PATH)
    action.SetParam(param.PatrolPathID, param.PatrolOrderIndex)
    action.SetBranch(462)
    g_AIDebugInfo[1014] = "[1014]ENTER_PATROL_PATH(param.PatrolPathID, param.PatrolOrderIndex)"

    action = ai.NewAction(545, AI_ACTION.SET_PRIMARY_TIMER)
    action.SetParam(8)
    action.SetBranch(634)
    g_AIDebugInfo[545] = "[545]SET_PRIMARY_TIMER(8)"

    action = ai.NewAction(1074, AI_ACTION.RECORD_RETURN_POSITION)
    action.SetBranch(1014)
    g_AIDebugInfo[1074] = "[1074]RECORD_RETURN_POSITION()"

    action = ai.NewAction(1080, AI_ACTION.NPC_RESET_SELECT_SKILL_CD)
    action.SetBranch(83)
    g_AIDebugInfo[1080] = "[1080]NPC_RESET_SELECT_SKILL_CD()"

    action = ai.NewAction(1086, AI_ACTION.NPC_RESET_SELECT_SKILL_CD)
    action.SetBranch(687)
    g_AIDebugInfo[1086] = "[1086]NPC_RESET_SELECT_SKILL_CD()"

    action = ai.NewAction(1036, 1001)
    action.SetParam()
    action.SetBranch(462)

    action = ai.NewAction(24, AI_ACTION.SET_STATE)
    action.SetParam(21)  -- 设置状态：主状态
    g_AIDebugInfo[24] = "[24]SET_STATE(21) --主状态"
    action = ai.NewAction(125, AI_ACTION.SET_STATE)
    action.SetParam(122)  -- 设置状态：战斗状态
    g_AIDebugInfo[125] = "[125]SET_STATE(122) --战斗状态"
    action = ai.NewAction(133, AI_ACTION.SET_STATE)
    action.SetParam(130)  -- 设置状态：返回状态
    g_AIDebugInfo[133] = "[133]SET_STATE(130) --返回状态"
    action = ai.NewAction(161, AI_ACTION.SET_STATE)
    action.SetParam(158)  -- 设置状态：NPC_SLEEP
    g_AIDebugInfo[161] = "[161]SET_STATE(158) --NPC_SLEEP"
    action = ai.NewAction(321, AI_ACTION.SET_STATE)
    action.SetParam(270)  -- 设置状态：逃跑状态
    g_AIDebugInfo[321] = "[321]SET_STATE(270) --逃跑状态"
    action = ai.NewAction(375, AI_ACTION.SET_STATE)
    action.SetParam(350)  -- 设置状态：警戒状态
    g_AIDebugInfo[375] = "[375]SET_STATE(350) --警戒状态"
    action = ai.NewAction(459, AI_ACTION.SET_STATE)
    action.SetParam(443)  -- 设置状态：对话状态
    g_AIDebugInfo[459] = "[459]SET_STATE(443) --对话状态"
    action = ai.NewAction(569, AI_ACTION.SET_STATE)
    action.SetParam(555)  -- 设置状态：恐惧状态
    g_AIDebugInfo[569] = "[569]SET_STATE(555) --恐惧状态"
    action = ai.NewAction(515, AI_ACTION.SET_STATE)
    action.SetParam(240)  -- 设置状态：战斗分支未选中技能
    g_AIDebugInfo[515] = "[515]SET_STATE(240) --战斗分支未选中技能"
    action = ai.NewAction(797, AI_ACTION.SET_STATE)
    action.SetParam(793)  -- 设置状态：SkillAniFrame
    g_AIDebugInfo[797] = "[797]SET_STATE(793) --SkillAniFrame"
    action = ai.NewAction(818, AI_ACTION.SET_STATE)
    action.SetParam(806)  -- 设置状态：SkillRestFrame
    g_AIDebugInfo[818] = "[818]SET_STATE(806) --SkillRestFrame"
    action = ai.NewAction(933, AI_ACTION.SET_STATE)
    action.SetParam(907)  -- 设置状态：判断水中超时状态
    g_AIDebugInfo[933] = "[933]SET_STATE(907) --判断水中超时状态"
    action = ai.NewAction(634, AI_ACTION.SET_STATE)
    action.SetParam(539)  -- 设置状态：吟唱状态
    g_AIDebugInfo[634] = "[634]SET_STATE(539) --吟唱状态"
    -- DEFINE STATES -------------------------------------------

    -- 初始状态
    state = ai.NewState(1)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 8)

    -- 主状态
    state = ai.NewState(21)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 342)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 462)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 254)
    state.HandleEvent(AI_EVENT.ON_DIALOG, 420)
    state.HandleEvent(AI_EVENT.ON_IN_FEAR, 687)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 462)
    state.HandleEvent(AI_EVENT.ON_BEGIN_PREPARE, 545)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 484)

    -- 战斗状态
    state = ai.NewState(122)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 83)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 172)
    state.HandleEvent(AI_EVENT.ON_IN_FEAR, 386)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 225)
    state.HandleEvent(AI_EVENT.ON_BEGIN_PREPARE, 545)

    -- 返回状态
    state = ai.NewState(130)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 194)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 194)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 194)

    -- NPC_SLEEP
    state = ai.NewState(158)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 581)

    -- 逃跑状态
    state = ai.NewState(270)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 322)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 83)
    state.HandleEvent(AI_EVENT.ON_IN_FEAR, 687)

    -- 警戒状态
    state = ai.NewState(350)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 261)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 733)
    state.HandleEvent(AI_EVENT.ON_IN_FEAR, 687)
    state.HandleEvent(AI_EVENT.ON_BEGIN_PREPARE, 545)

    -- 对话状态
    state = ai.NewState(443)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 288)
    state.HandleEvent(AI_EVENT.ON_ATTACKED, 254)
    state.HandleEvent(AI_EVENT.ON_DIALOG, 435)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 1036)
    state.HandleEvent(AI_EVENT.ON_IN_FEAR, 687)
    state.HandleEvent(AI_EVENT.ON_BEGIN_PREPARE, 545)

    -- 恐惧状态
    state = ai.NewState(555)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 386)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 386)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 386)
    state.HandleEvent(AI_EVENT.ON_OUT_FEAR, 83)
    state.HandleEvent(AI_EVENT.ON_BEGIN_PREPARE, 545)

    -- 战斗分支未选中技能
    state = ai.NewState(240)  -- 重载战斗状态的事件
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 83)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 83)
    state.HandleEvent(AI_EVENT.ON_IN_FEAR, 386)
    state.HandleEvent(AI_EVENT.ON_PATH_FAILED, 172)
    state.HandleEvent(AI_EVENT.ON_BEGIN_PREPARE, 545)

    -- SkillAniFrame
    state = ai.NewState(793)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 105)
    state.HandleEvent(AI_EVENT.ON_IN_FEAR, 687)
    state.HandleEvent(AI_EVENT.ON_BEGIN_PREPARE, 545)

    -- SkillRestFrame
    state = ai.NewState(806)
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 854)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 83)
    state.HandleEvent(AI_EVENT.ON_IN_FEAR, 687)
    state.HandleEvent(AI_EVENT.ON_PATH_SUCCESS, 854)

    -- 吟唱状态
    state = ai.NewState(539)
    state.HandleEvent(AI_EVENT.ON_CAST_SKILL, 894)
    state.HandleEvent(AI_EVENT.ON_IN_FEAR, 1086)
    state.HandleEvent(AI_EVENT.ON_CAST_SKILL_FAILED, 889)
    state.HandleEvent(AI_EVENT.ON_BREAK_PREPARE, 1080)

    -- 判断水中超时状态
    state = ai.NewState(907)  -- 暂时用于防玩家入水卡怪
    state.HandleEvent(AI_EVENT.ON_PRIMARY_TIMER, 936)
    state.HandleEvent(AI_EVENT.ON_SECONDARY_TIMER, 172)
    state.HandleEvent(AI_EVENT.ON_BEGIN_PREPARE, 545)

    ai.SetInitState(1)
end

function AI:DebugOnCallAction(action_id, nAIType)
    print("[AI] Call action: "..AI[nAIType][action_id])
end
