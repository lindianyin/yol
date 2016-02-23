
Include("script/ai/ai_param.lua")

--声明状态变量
local STATE_STANDBY = 1

function AI:Setup(ai)
    local state
    local action
    local param = AI.g_AIParam[ai.nAIType]

    --注册自定义动作

    --待机状态
    state = ai.NewState(STATE_STANDBY)
    state.HandleEvent(AI_EVENT.ON_DIALOG, 2)

	--选择时间发起者
    action = ai.NewAction(2, AI_ACTION.SELECT_EVENT_SRC)
    action.SetBranch(3, 5)

    --打开NPC对话窗口
    action = ai.NewAction(3, AI_ACTION.NPC_OPEN_DIALOG_WINDOW)
    action.SetBranch(5, 5)

    --设置状态
    action = ai.NewAction(5, AI_ACTION.SET_STATE)
    action.SetParam(STATE_STANDBY)

    ai.SetInitState(STATE_STANDBY)
end