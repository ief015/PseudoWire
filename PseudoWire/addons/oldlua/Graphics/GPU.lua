texture = RenderTexture(128, 128)

function onSpawn()
	SetImage("Default\\img\\gpu.png")
	SetInputs({
		"Update", "number", 3, 12,
		"Clk", "number", 3, 8*3,
		"Clr", "number", 3, 8*4,
		
		"vX1", "number", 3, 8*6,
		"vY1", "number", 3, 8*7,
		"vX2", "number", 3, 8*8,
		"vY2", "number", 3, 8*9,
		
		"cR", "number", 3, 8*11,
		"cG", "number", 3, 8*12,
		"cB", "number", 3, 8*13,
		"cA", "number", 3, 8*14,
		
		"Enable", "number", 3, 8*16,
	})
	ShowToolTip(false)
	
	texture.Position = Vector(6, 6)
end

function onDraw()
	if not(GetInput(11) == 0) then --enabled
		Draw(texture)
	end
end

function onTriggerInput(port)
	if not(GetInput(11) == 0) then --enabled
		if (port == 1) and not(GetInput(1) == 0) then --clk!
			texture:Draw(Shape.Rectangle(
				Vector(GetInput(3), GetInput(4)),
				Vector(GetInput(5), GetInput(6)),
				Color(GetInput(7), GetInput(8), GetInput(9), GetInput(10))
			))
		elseif (port == 0) and not(GetInput(0) == 0) then --update
			texture:Update()
		elseif (port == 2) and not(GetInput(2) == 0) then --clear
			texture:Clear()
		end
	end
end