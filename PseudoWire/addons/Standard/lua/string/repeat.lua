function Component:onSpawn()
	self:SetImage("str_rep.png")
	self:SetToolTip("Repeat")
	
	self:SetInputs({
		{"Str", TYPE_STRING, 2, 12},
		{"Repeats", TYPE_NUMBER, 2, 20}
	})
	
	self:SetOutputs({
		{"Out", TYPE_STRING, self:GetWidth()-2, 12}
	})
end

local out = nil
local inputs = nil
function Component:onInputsTriggered()
	inputs = self:GetInputsTable()
	
	out = string.rep(inputs[1],inputs[2])
	if(out ~= nil) then
		self:TriggerOutput(1, out)
	else
		self:TriggerOutput(1, "")
	end
end
