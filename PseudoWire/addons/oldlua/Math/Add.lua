function onSpawn()
	SetInputs({
		"A", "number", 4, 12,
		"B", "number", 4, 20,
		"C", "number", 4, 28,
		"D", "number", 4, 36
	})
	SetOutputs({
		"Out", "number", GetWidth() - 2, 12
	})
	SetToolTip("A + B + C + D = Out")
	SetImage("Default\\img\\math_add.png")
end

local out = 0;

function onTriggerInput(port)
	-- Output to Port 0,			return sum of all inputs
	out = GetInput(0) + GetInput(1) + GetInput(2) + GetInput(3)
	SetToolTip(GetInput(0) .. " + " .. GetInput(1) .. " + " .. GetInput(2) .. " + " .. GetInput(3) .. " = " .. out)
	TriggerOutput(0, out)
end
