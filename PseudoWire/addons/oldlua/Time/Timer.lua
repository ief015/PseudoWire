local out = 0

function onSpawn()
	SetImage("Default\\img\\time_timer.png")
	SetInputs({
		"Clk", "number", 2, 12,
		"Reset", "number", 2, 20
	})
	SetOutputs({
		"Out", "number", GetWidth() - 2, GetHeight()/2
	})
	SetToolTip("Timer\nInputs - Clk: Enable. Reset: Reset timer to 0.\nOutputs - Out: Time elapsed.")
end

function onTriggerInput(port)
	if (port == 1) and (not(GetInput(1) == 0)) then
		out = 0
		TriggerOutput(0, out)
		SetToolTip("Timer = " .. out)
	end
end

function onTick(tick, gametime, ticktime)
	if not(GetInput(0) == 0) then
		if GetInput(0) > 0 then out = out + ticktime else out = out - ticktime end
		SetToolTip("Timer = " .. out)
		TriggerOutput(0, out)
	end
end