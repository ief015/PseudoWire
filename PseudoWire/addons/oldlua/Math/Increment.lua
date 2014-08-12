function onSpawn()
	SetImage("Default\\img\\math_inc.png")
	SetInputs({
		"Clk", "number", 4, 12,
		"Reset", "number", 4, 20,
		"A", "number", 4, 36
	})
	SetOutputs({
		"Out", "number", GetWidth() - 2, 12
	})
	SetToolTip("Inc += A")
end

local out = 0;
function onTriggerInput(port)
	if (port == 0) and (not(GetInput(0) == 0)) then
		out = out + IFELSE(HasLinkAt("A"), GetInput(2), 1) --if there is no link at "A", default inc by 1
		TriggerOutput(0, out)
	elseif (port == 1) and (not(GetInput(1) == 0)) then
		out = 0
		TriggerOutput(0, out)
	end
	SetToolTip("Inc += " .. GetInput(2) .. " -> Count = ".. out)
end