function Component:onSpawn()
	self:SetImage("bit_lshift.png")
	self:SetToolTip("Bitwise Left Shift\nA << B")
	
	self:SetInputs({
		{ "A", TYPE_NUMBER, 2, 12 },
		{ "B", TYPE_NUMBER, 2, 28 }
	})
	
	self:SetOutputs({
		{ "Out", TYPE_NUMBER, self:GetWidth()-2, 12 }
	})
end

local a,b
function Component:onInputsTriggered()
	a,b = self:GetAllInputValue()
	self:TriggerOutput(1, BIT_LSHIFT(a, b))
end
