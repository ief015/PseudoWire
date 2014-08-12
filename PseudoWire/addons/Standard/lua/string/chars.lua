function Component:onSpawn()
	self:SetImage("str_charat.png")
	self:SetToolTip("Char At (String)")
	
	self:SetInputs({
		{"A", TYPE_STRING, 2, 12},
		{"B", TYPE_NUMBER, 2, 20}
	})
	
	self:SetOutputs({
		{"Out", TYPE_STRING, self:GetWidth()-2, 12}
	})
end

local inputs = nil
function Component:onInputsTriggered()
	inputs = self:GetInputsTable()
	
	if(inputs[2] > 0 and inputs[2] <= #inputs[1]) then
		self:TriggerOutput( 1, inputs[1]:sub(inputs[2],inputs[2]) )
	else
		self:TriggerOutput(1, "")
	end
end
