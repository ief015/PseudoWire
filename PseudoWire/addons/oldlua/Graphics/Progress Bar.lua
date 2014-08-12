local width = 0

function onSpawn()
	SetImage("Default\\img\\graphics_progressdisp.png")
	SetInputs({
		"Value", "number", 2, 6,
		"Min", "number", 2, 14,
		"Max", "number", 2, 22
	})
	width = GetWidth()-8
	SetToolTip("Min < Value < Max")
end

function onDraw()
	if HasLinkAt(0) then
		local min = IFELSE(HasLinkAt("Min"), GetInput("Min"), 0)
		local max = IFELSE(HasLinkAt("Max"), GetInput("Max"), 1)
		if(min >= max) then min = 0 end
		local val = Clamp(GetInput(0), min, max)
		
		SetToolTip((val-min)/(max-min)*100 .. "%")
		
		DrawRect(5, 3, (val-min)/(max-min)*width, GetHeight()-6, 110, 120, 175, 175)
	end
end