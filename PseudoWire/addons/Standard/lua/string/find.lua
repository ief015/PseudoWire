function Component:onSpawn()
	self:SetImage("str_find.png")
	self:SetToolTip("Find")
	
	self:SetInputs({
		{"Str", TYPE_STRING, 2, 12},
		{"Pattern", TYPE_STRING, 2, 20}
	})
	
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12}
	})
end

local out = nil
local inputs = nil
function Component:onInputsTriggered()
	inputs = self:GetInputsTable()
	
	out = string.find(inputs[1],inputs[2])
	if(out ~= nil) then
		self:TriggerOutput(1, out)
	else
		self:TriggerOutput(1, "")
	end
end
