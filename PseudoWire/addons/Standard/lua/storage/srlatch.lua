function Component:onSpawn()
	self:SetImage("ram_sr.png")
	self:SetToolTip("Set-Reset Latch")
	
	self:SetInputs({
		{"Set", TYPE_NUMBER, 2, 12},
		{"Reset", TYPE_NUMBER, 2, 20}
	})
	
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12}
	})
end

local inputs = nil
function Component:onInputsTriggered()
	inputs = self:GetInputsTable()
	
	if(self:IsInputTriggered(2) and inputs[2] ~= 0) then
		--reset
		self:TriggerOutput(1, 0)
	elseif(self:IsInputTriggered(1) and inputs[1] ~= 0) then
		--set
		self:TriggerOutput(1, inputs[1])
	end
end
