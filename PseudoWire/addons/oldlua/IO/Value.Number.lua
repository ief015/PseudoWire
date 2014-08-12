local value = 0;

function onSpawn()
	SetImage("Default\\img\\io_value_num.png")
	SetOutputs({
		"Out", "number", GetWidth() - 2, GetHeight() / 2
	})
	value = UserInput("number", "Value?")
	
	SetToolTip("Value = " .. value)
	TriggerOutput(0, value)
end