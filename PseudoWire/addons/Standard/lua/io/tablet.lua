local width, height = 0, 0
local inside = false
local button = 0
local function PointInside(x, y)
	return (x > 4 and y > 4 and x < width-4 and y < height-4)
end

function Component:onSpawn()
	self:SetImage("io_tablet.png")
	width = self:GetWidth()
	height = self:GetHeight()
	
	self:SetOutputs({
		{"Inside", 0, width-2, 12},
		{"X", 0, width-2, 28},
		{"Y", 0, width-2, 36}
	})
end

function Component:onMouseMovedInside(x, y, dx, dy)

	if(PointInside(x, y)) then
		if(not inside) then
			self:TriggerOutput(1, 1)
			inside = true
		end
		self:TriggerOutput(2, x-4)
		self:TriggerOutput(3, y-4)
	else
		if(inside) then
			self:TriggerOutput(1, 0)
			inside = false
		end
	end
end

function Component:onMouseLeave(x, y)
	if(inside) then
		if(not PointInside(x, y)) then
			self:TriggerOutput(1, 0)
		end
		inside = false
	end
end
