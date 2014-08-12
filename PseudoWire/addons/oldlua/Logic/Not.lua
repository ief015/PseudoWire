function onSpawn()
	SetInputs({
		"A", "number", 4, 12,
		"B", "number", 4, 20
	})
	SetOutputs({
		"!A", "number", GetWidth() - 2, 12,
		"!B", "number", GetWidth() - 2, 20
	})
	SetToolTip("X = !X")
	SetImage("Default\\img\\logic_2not.png")
end

function onTriggerInput(port)
	TriggerOutput(port, IFELSE(GetInput(port)==0, 1, 0))
end
