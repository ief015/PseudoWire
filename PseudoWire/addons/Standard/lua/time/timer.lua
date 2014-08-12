local t = 0
local on = false

function Component:onSpawn()
	self:SetImage("time_timer.png")
	self:SetToolTip("Timer")
	self:SetInputs({
		{"On", TYPE_NUMBER, 2, 12},
		{"Reset", TYPE_NUMBER, 2, 20}
	})
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, self:GetHeight()/2}
	})
end

function Component:onThink(dt)
	if(on) then
		t = t + (dt / 1000)
		self:TriggerOutput(1, t)
	end
end

local prev_on = false
function Component:onInputsTriggered()
	on = (self:GetInputValue(1) ~= 0)
	
	if(self:GetInputValue(2) ~= 0) then
		t = 0
		self:TriggerOutput(1, t)
	end
	
	if(prev_on ~= on) then
		if(on) then
			self:SetToolTip("Timer\nTiming...")
		else
			self:SetToolTip("Timer")
		end
		prev_on = on
	end
end
