local out = 0

function Component:onSpawn()
	self:SetImage("ram_t.png")
	self:SetToolTip("Toggle Latch")
	
	self:SetInputs({
		{"T", TYPE_NUMBER, 2, 12}
	})
	
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12}
	})
end

local val = 0
function Component:onInputsTriggered()
	val = self:GetInputValue(1)
	
	if(self:IsInputTriggered(1) and val ~= 0) then
		--toggle
		if(out == 0) then out = 1 else out = 0 end
		
		self:TriggerOutput(1, out)
	end
end
