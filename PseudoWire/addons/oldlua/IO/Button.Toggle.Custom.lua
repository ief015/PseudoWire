local val_off = 0
local val_on = 1

function onSpawn()
	SetImage("Default\\img\\io_button_open.png")
	SetOutputs({
		"Out", "number", GetWidth() - 2, GetHeight()/2
	})
	
	val_off = UserInput("number", "Value for off? Default is 0.")
	val_on = UserInput("number", "Value for on? Default is 1.")
	
	SetToolTip("Out = " .. val_off)
end

local out = 0

function onClicked(x, y, button)
	if out==val_off then 
		out = val_on
		SetImage("Default\\img\\io_button_closed.png")
	else
		out = val_off
		SetImage("Default\\img\\io_button_open.png")
	end
	SetToolTip("Out = " .. out)
	TriggerOutput(0, out)
end

function onDraw()
	local txt = Text()
	txt.Caption = "Push"
	txt.Position = Vector(11, 20)
	txt.Size = 10
	txt.Color = Color(0, 0, 0, IFELSE(out == 0, 100, 200))
	Draw(txt)
	txt:Kill()
end