function onSpawn()
	SetImage("Default\\img\\math_randclk.png")
	SetInputs({
		"Clk", "number", 2, GetWidth()/2
	})
	SetOutputs({
		"Out", "number", GetWidth() - 2, GetWidth()/2
	})
	SetToolTip("Out = Random")
	math.randomseed(os.time())
end

local out = 0;

function onTriggerInput(port)
	if not(GetInput(0)==0) then
		out = math.random()
		SetToolTip("Out = " .. out)
	end
end