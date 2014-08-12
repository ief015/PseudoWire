function onSpawn()
	SetImage("Default\\img\\graphics_screen.png")
	SetInputs({
		"In", "number", 2, GetHeight()/2
	})
	ShowToolTip(false)
end

local txt = Text()
function onDraw()
	if HasLinkAt(0) then
		txt.Caption = tostring(GetInput(0))
		txt.Position = Vector(10, 6)
		txt.Size = 11
		txt.Color = Color(255, 255, 255, 200)
		Draw(txt)
	end
end

function onRemove()
	txt:Kill()
end