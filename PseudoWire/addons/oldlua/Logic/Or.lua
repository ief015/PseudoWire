function onSpawn()
	SetInputs({
		"A", "number", 4, 12,
		"B", "number", 4, 36
	})
	SetOutputs({
		"Out", "number", GetWidth() - 2, 12
	})
	SetToolTip("Out = A | B")
	SetImage("Default\\img\\logic_or.png")
end
function onTriggerInput(port)
	local out = IFELSE(
		GetInput(0)==0 and GetInput(1)==0
	, 0, 1)
	
	TriggerOutput(0, out)
	SetToolTip(out .. " = " .. GetInput(0) .. " | " .. GetInput(1))
end
