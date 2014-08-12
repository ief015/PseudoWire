function onSpawn()
	SetImage("Default\\img\\math_sub.png")
	SetInputs({
		"A", "number", 4, 12,
		"B", "number", 4, 20,
		"C", "number", 4, 28,
		"D", "number", 4, 36
	})
	SetOutputs({
		"Out", "number", GetWidth() - 2, 12
	})
	SetToolTip("A - B - C - D = Out")
end

function onTriggerInput(port)
	-- Output to Port 0,			return difference of all inputs
	local out = GetInput(0) - GetInput(1) - GetInput(2) - GetInput(3)
	SetToolTip(GetInput(0) .. " - " .. GetInput(1) .. " - " .. GetInput(2) .. " - " .. GetInput(3) .. " = " .. out)
	TriggerOutput(0, out)
end

--function onMouseMoved(x, y) end