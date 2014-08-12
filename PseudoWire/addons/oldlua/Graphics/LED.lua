function onSpawn()
	SetImage("Default\\img\\io_led_off.png")
	SetInputs({
		"In", "number", GetWidth()/2, GetHeight() - 2
	})
	
	SetToolTip("Off")
end

function onTriggerInput(port)
	if(GetInput(0) == 0) then
		SetToolTip("Off")
		SetImage("Default\\img\\io_led_off.png")
	else
		SetToolTip("On")
		SetImage("Default\\img\\io_led_on.png")
	end
end