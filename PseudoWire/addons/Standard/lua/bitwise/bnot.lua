function Component:onSpawn()
	self:SetImage("bit_not.png")
	self:SetToolTip("Bitwise Not\n~A")
	
	self:SetInputs({
		{ "A", TYPE_NUMBER, 2, 12 }
	})
	
	self:SetOutputs({
		{ "Out", TYPE_NUMBER, self:GetWidth()-2, 12 }
	})
end

local a = 0
function Component:onInputsTriggered()
	a = self:GetInputValue(1)
	self:TriggerOutput(1, BIT_NOT(a))
end
