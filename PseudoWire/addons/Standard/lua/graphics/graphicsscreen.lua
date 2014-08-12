local pixel = nil --Shape, created onSpawn
local rt = RenderTexture()

function Component:onSpawn()
	self:SetImage("display_graphics.png")
	
	self:SetInputs({
		{"Clk", TYPE_NUMBER, 3, 12},
		
		{"X", TYPE_NUMBER, 3, 28},
		{"Y", TYPE_NUMBER, 3, 36},
		
		{"R", TYPE_NUMBER, 3, 52},
		{"G", TYPE_NUMBER, 3, 60},
		{"B", TYPE_NUMBER, 3, 68},
		
		{"Clear", TYPE_NUMBER, 3, 84},
		
		{"Display", TYPE_NUMBER, 3, 100}
	})
	
	pixel = game.create_rect(0,0, 1,1, Color(255,0,0))
	rt:Create(128,128)
	rt:Clear()
	rt:Display()
	rt:SetPos(Vector(6,6))
end

local inputs = nil
function Component:onInputsTriggered()
	inputs = self:GetInputsTable()
	
	pixel:SetPos(Vector(inputs[2], inputs[3]))
	pixel:SetColor(Color(inputs[4], inputs[5], inputs[6]))
	
	if(self:IsInputTriggered(7) and inputs[7] ~= 0) then -- clear
		-- rt:Clear(pixel:GetColor())
		rt:Clear()
	elseif(self:IsInputTriggered(1) and inputs[1] ~= 0) then -- draw
		rt:Draw(pixel)
	end
	
	if(self:IsInputTriggered(8) and inputs[8] ~= 0) then -- display
		rt:Display()
	end
end

function Component:onDraw()
	self:Draw(rt)
end
