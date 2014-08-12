function Component:onSpawn()
	self:SetImage("ram_d.png")
	self:SetToolTip("Data Latch")
	
	self:SetInputs({
		{"D", TYPE_NUMBER, 2, 12}
	})
	
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12}
	})
end

local val = 0
function Component:onInputsTriggered()
	val = self:GetInputValue(1)
	
	if(self:IsInputTriggered(1) and val ~= 0) then
		self:TriggerOutput(1, val)
	end
end
