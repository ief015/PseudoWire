function onSpawn()
	SetImage("Default\\img\\io_button_open.png")
	SetOutputs({
		"Out", "number", GetWidth() - 2, GetHeight()/2
	})
	SetToolTip("Button")
end

local out = 0

function onClicked(x, y, button)
	SetImage("Default\\img\\io_button_closed.png")
	out = 1
	SetToolTip("Out = " .. out)
	TriggerOutput(0, out)
end

function onMouseRelease(x, y, button)
	if out == 0 then return end
	SetImage("Default\\img\\io_button_open.png")
	out = 0
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