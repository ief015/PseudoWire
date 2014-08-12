local value = 0;

function onSpawn()
	SetImage("Default\\img\\io_value_str.png")
	SetOutputs({
		"Out", "string", GetWidth() - 2, GetHeight() / 2
	})
	value = UserInput("string", "Value?")
	
	SetToolTip("Value = \"" .. value .. "\"")
	TriggerOutput(0, value)
end