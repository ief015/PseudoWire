function Component:onSpawn()
	self:SetImage("str_replace.png")
	self:SetToolTip("Replace")
	
	self:SetInputs({
		{"Str", TYPE_STRING, 2, 12},
		{"Pattern", TYPE_STRING, 2, 20},
		{"ReplaceStr", TYPE_STRING, 2, 28}
	})
	
	self:SetOutputs({
		{"Out", TYPE_STRING, self:GetWidth()-2, 12}
	})
end

local out = nil
local inputs = nil
function Component:onInputsTriggered()
	inputs = self:GetInputsTable()
	
	out = string.gsub(inputs[1], inputs[2], inputs[3])
	if(out ~= nil) then
		self:TriggerOutput(1, out)
	else
		self:TriggerOutput(1, "")
	end
end
