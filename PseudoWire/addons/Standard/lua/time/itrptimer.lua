local pcnt = 0
local rate = 0
local min = 0
local max = 1
local on = false
local finished = false

function Component:DoSend(p)
	self:TriggerOutput(1, p * (max-min) + min)
end

function Component:onSpawn()
	self:SetImage("time_int.png")
	self:SetToolTip("Interpolation Timer")
	self:SetInputs({
		{"On", TYPE_NUMBER, 2, 12},
		{"Min", TYPE_NUMBER, 2, 20},
		{"Max", TYPE_NUMBER, 2, 28},
		{"Reset", TYPE_NUMBER, 2, 36}
	})
	self:SetOutputs({
		{"Time", TYPE_NUMBER, self:GetWidth()-2, 12},
		{"Finished", TYPE_NUMBER, self:GetWidth()-2, 20}
	})
end

function Component:onThink(dt)
	if(on and not finished) then
		pcnt = pcnt + ((dt/1000) / rate)
		
		if(pcnt >= 1) then
			pcnt = 1
			finished = true
			self:TriggerOutput(2, 1)
		end
		
		self:DoSend(pcnt)
	end
end

local prev_on = false
function Component:onInputsTriggered()
	rate = self:GetInputValue(1)
	on = (rate ~= 0)
	
	if(self:IsInputTriggered(2)) then
		min = self:GetInputValue(2)
		self:DoSend(pcnt)
	end
	if(self:IsInputTriggered(3)) then
		max = self:GetInputValue(3)
		self:DoSend(pcnt)
	end
	
	if(self:GetInputValue(4) ~= 0) then
		pcnt = 0
		finished = false
		self:TriggerOutput(1, min)
		self:TriggerOutput(2, 0)
	end
	
	if(prev_on ~= on) then
		if(on) then
			self:SetToolTip("Interpolation Timer\nTiming...")
		else
			self:SetToolTip("Interpolation Timer")
		end
		prev_on = on
	end
end
