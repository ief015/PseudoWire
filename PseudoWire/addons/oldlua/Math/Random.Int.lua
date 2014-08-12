function onSpawn()
	SetImage("Default\\img\\math_randint.png")
	SetInputs({
		"Min", "number", 2, 12,
		"Max", "number", 2, 20,
	})
	SetOutputs({
		"Out", "number", GetWidth() - 2, GetWidth()/2
	})
	SetToolTip("Out = Min < Random < Max")
	math.randomseed(os.time())
end

local out = 0;

function onTick(tick, elapsed, ticktime)
	if GetInput(0) > GetInput(1) then
		out = math.random()
		SetToolTip("Out = " .. out)
	else
		out = math.random() * (GetInput(1) - GetInput(0)) + GetInput(0)
		SetToolTip("Out = " .. GetInput(0) .." < ".. out .. " < "  .. GetInput(1))
	end
	TriggerOutput(0, out)
end