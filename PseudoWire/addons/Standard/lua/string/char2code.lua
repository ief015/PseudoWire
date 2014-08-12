function Component:onSpawn()
	self:SetImage("str_char2code.png")
	self:SetToolTip("Char. -> Code")
	
	self:SetInputs({
		{"A", TYPE_STRING, 2, 12}
	})
	
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12}
	})
end

local out = nil
local inputs = nil
function Component:onInputsTriggered()
	inputs = self:GetInputsTable()
	
	out = string.byte(inputs[1]:sub(1,1))
	if(out ~= nil) then
		self:TriggerOutput( 1, out )
	else
		self:TriggerOutput(1, 0)
	end
end
