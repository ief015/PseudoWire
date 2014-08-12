local out = 0
local on = false
local mul = 1

function Component:onSpawn()
	self:SetImage("time_accum.png")
	self:SetToolTip("Accumulator")
	self:SetInputs({
		{"On", TYPE_NUMBER, 2, 12},
		{"Reset", TYPE_NUMBER, 2, 20},
		{"A", TYPE_NUMBER, 2, 36}
	})
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, self:GetHeight()/2}
	})
end

function Component:onThink(dt)
	if(on) then
		out = out + (dt / 1000 * mul)
		self:TriggerOutput(1, out)
	end
end

local prev_on = false
function Component:onInputsTriggered()
	on = (self:GetInputValue(1) ~= 0)
	
	if(self:GetInputValue(2) ~= 0) then
		out = 0
		self:TriggerOutput(1, out)
	end
	
	if(self:IsConnected(3)) then
		mul = self:GetInputValue(3)
	end
	
	if(prev_on ~= on) then
		if(on) then
			self:SetToolTip("Accumulator\nAccumulating...")
		else
			self:SetToolTip("Accumulator")
		end
		prev_on = on
	end
end
