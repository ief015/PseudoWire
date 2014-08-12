local title = ""

function onSpawn()
	SetImage("Default\\img\\graphics_screen.png")
	SetInputs({
		"In", "number", 2, GetHeight()/2
	})
	ShowToolTip(false)
	
	title = UserInput("string", "Enter a title.")
end

function onDraw()
	local txt = Text()
	if HasLinkAt(0) then
		txt.Caption = tostring(GetInput(0))
		txt.Position = Vector(10, 9)
		txt.Size = 11
		txt.Color = Color(255, 255, 255, 200)
		Draw(txt)
	end
	txt.Caption = title
	txt.Size = 9
	txt.Color = Color(175, 100, 175, 200)
	txt.Origin = Vector(txt:GetWidth()/2, 0)
	txt.Position = Vector(GetWidth()/2, 1)
	Draw(txt)
	txt:Kill()
end