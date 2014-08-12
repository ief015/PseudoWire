function Component:onSpawn()
	self:SetImage("str_append.png")
	self:SetToolTip("Append")
	
	self:SetInputs({
		{"A", TYPE_STRING, 2, 12},
		{"B", TYPE_STRING, 2, 28}
	})
	
	self:SetOutputs({
		{"Out", TYPE_STRING, self:GetWidth()-2, 12}
	})
end

local inputs = nil
function Component:onInputsTriggered()
	inputs = self:GetInputsTable()
	self:TriggerOutput(1, inputs[1]..inputs[2])
end
