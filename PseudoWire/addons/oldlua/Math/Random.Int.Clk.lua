function onSpawn()
	SetImage("Default\\img\\math_randintclk.png")
	SetInputs({
		"Clk", "number", 2, 12,
		"Min", "number", 2, 28,
		"Max", "number", 2, 36,
	})
	SetOutputs({
		"Out", "number", GetWidth() - 2, GetWidth()/2
	})
	SetToolTip("Out = Min < Random < Max")
	math.randomseed(os.time())
end

local out = 0;

function onTriggerInput(port)
	if not(GetInput(0)==0) then
		if GetInput(1) > GetInput(2) then
			out = math.random()
			SetToolTip("Out = " .. out)
		else
			out = math.random() * (GetInput(2) - GetInput(1)) + GetInput(1)
			SetToolTip("Out = " .. GetInput(1) .." < ".. out .. " < "  .. GetInput(2))
		end
		TriggerOutput(0, out)
	end
end