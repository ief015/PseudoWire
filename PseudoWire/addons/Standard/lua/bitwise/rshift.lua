function Component:onSpawn()
	self:SetImage("bit_rshift.png")
	self:SetToolTip("Bitwise Right Shift\nA >> B")
	
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
	a,b = self:GetAllInputValues()
	self:TriggerOutput(1, BIT_RSHIFT(a, b))
end
