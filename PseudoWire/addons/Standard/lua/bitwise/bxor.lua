function Component:onSpawn()
	self:SetImage("bit_xor.png")
	self:SetToolTip("Bitwise Exclusive Or\nA ^ B")
	
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
	self:TriggerOutput(1, BIT_XOR(a, b))
end
