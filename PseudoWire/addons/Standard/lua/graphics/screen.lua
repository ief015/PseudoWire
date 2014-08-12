local txt = Text()
local update = false

function Component:onSpawn()
	self:SetImage("graphics_screen.png")
	self:SetInputs({
		{"A", TYPE_ANY, 2, self:GetHeight()/2}
	})
	
	txt:SetSize(12)
	txt:SetText("0")
	txt:SetOrigin(Vector(0, math.floor(txt:GetHeight()/2)))
	txt:SetPos(Vector(8, math.floor(self:GetHeight()/2)-2))
end

function Component:onInputsTriggered()
	update = true
	--txt:SetText(tostring(self:GetInputValue(1)))
end

function Component:onDraw()
	if(update) then	
		if(self:IsConnected(1)) then
			txt:SetText(tostring(self:GetInputValue(1)))
		else
			txt:SetText("")
		end
		update = false
	end
	self:Draw(txt)
end
