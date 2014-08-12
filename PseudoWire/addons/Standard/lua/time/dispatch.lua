local on = false
local rate = 1
local out = false
local t = 0

local pulse = false

function Component:onSpawn()
	self:SetImage("time_osc.png")
	self:SetToolTip("Oscillating Dispatcher")
	self:SetInputs({
		{"On", TYPE_NUMBER, 2, 12},
		{"Reset", TYPE_NUMBER, 2, 20},
		{"Rate", TYPE_NUMBER, 2, 36}
	})
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12},
		{"Time", TYPE_NUMBER, self:GetWidth()-2, 20}
	})
end

function Component:onBuildEdit()
	self:AddUIRadio("osc", "Oscillate", "grp_op", not pulse)
	self:AddUIRadio("pls", "Pulse", "grp_op", pulse)
	return true
end

function Component:onEditSubmit(settings)
	pulse = settings["pls"]
	
	if(pulse) then
		self:SetToolTip("Oscillating Dispatcher")
	else
		self:SetToolTip("Pulsing Dispatcher")
	end
end

function Component:onThink(dt)
	if(pulse and out) then
		out = false
		self:TriggerOutput(1, IFELSE(out, 1, 0))
	end
	
	if(on) then
		t = t + (dt / 1000)
		if(t >= rate) then
			if(pulse) then
				out = true
			else
				out = not out
			end
			t = t - rate
			self:TriggerOutput(1, IFELSE(out, 1, 0))
		end
		self:TriggerOutput(2, t)
	end
end

local prev_on = false
function Component:onInputsTriggered()
	on = (self:GetInputValue(1) ~= 0)
	
	if(self:GetInputValue(2) ~= 0) then
		t = 0
		out = false
		self:TriggerOutput(1, IFELSE(out, 1, 0))
		self:TriggerOutput(2, t)
	end
	
	if(self:IsConnected(3)) then
		rate = self:GetInputValue(3)
	end
	
	if(prev_on ~= on) then
		if(on) then
			if(pulse) then
				self:SetToolTip("Oscillating Dispatcher\nTiming...")
			else
				self:SetToolTip("Pulsing Dispatcher\nTiming...")
			end
		else
			if(pulse) then
				self:SetToolTip("Oscillating Dispatcher")
			else
				self:SetToolTip("Pulsing Dispatcher")
			end
		end
		prev_on = on
	end
end
