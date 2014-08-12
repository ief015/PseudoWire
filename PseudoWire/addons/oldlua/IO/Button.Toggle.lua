function onSpawn()
	SetImage("Default\\img\\io_button_open.png")
	SetOutputs({
		"Out", "number", GetWidth() - 2, GetHeight()/2
	})
	SetToolTip("Toggle Button")
end

local out = 0

function onClicked(x, y, button)
	if out==0 then 
		out = 1
		SetImage("Default\\img\\io_button_closed.png")
	else
		out = 0
		SetImage("Default\\img\\io_button_open.png")
	end
	SetToolTip("Out = " .. out)
	TriggerOutput(0, out)
end

local txt = Text()
function onDraw()
	txt.Caption = "Push"
	txt.Position = Vector(11, 20)
	txt.Size = 10
	txt.Color = Color(0, 0, 0, IFELSE(out == 0, 100, 200))
	Draw(txt)
end

function onRemove()
	txt:Kill()
end