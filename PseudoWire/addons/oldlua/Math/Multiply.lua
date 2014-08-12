function onSpawn()
	SetImage("Default\\img\\math_mul.png")
	SetInputs({
		"A", "number", 4, 12,
		"B", "number", 4, 28
	})
	SetOutputs({
		"Out", "number", GetWidth() - 2, 12
	})
	SetToolTip("A * B = Out")
end

local out = 0;

function onTriggerInput(port)
	-- Output to Port 0,	return product of all inputs
	out = GetInput(0) * GetInput(1)
	SetToolTip(GetInput(0) .. " * " .. GetInput(1) .. " = " .. out)
	TriggerOutput(0, out)
end