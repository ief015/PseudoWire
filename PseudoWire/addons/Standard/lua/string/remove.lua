function Component:onSpawn()
	self:SetImage("str_remove.png")
	self:SetToolTip("Remove")
	
	self:SetInputs({
		{"Str", TYPE_STRING, 2, 12},
		{"Start", TYPE_NUMBER, 2, 20},
		{"End", TYPE_NUMBER, 2, 28}
	})
	
	self:SetOutputs({
		{"Out", TYPE_STRING, self:GetWidth()-2, 12}
	})
end

local out = nil
local inputs = nil
function Component:onInputsTriggered()
	inputs = self:GetInputsTable()
	
	out = string.remove(inputs[1], inputs[2], inputs[3])
	if(out ~= nil) then
		self:TriggerOutput(1, out)
	else
		self:TriggerOutput(1, "")
	end
end
