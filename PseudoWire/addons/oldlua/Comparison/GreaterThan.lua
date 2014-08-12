function onSpawn()
	SetImage("Default\\img\\comp_gt.png")	
	SetInputs({
		"A", "number", 4, 12,
		"B", "number", 4, 28
	})
	SetOutputs({
		"Out", "number", GetWidth() - 2, 12
	})
	SetToolTip("Out = A > B")
end

function onTriggerInput(port)
	local out = IFELSE(
		GetInput(0) > GetInput(1)
	, 1, 0)
	
	TriggerOutput(0, out)
	SetToolTip(out .. " = " .. GetInput(0) .. " > " .. GetInput(1))
end
