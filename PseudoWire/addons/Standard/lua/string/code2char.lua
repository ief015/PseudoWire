function Component:onSpawn()
	self:SetImage("str_code2char.png")
	self:SetToolTip("Code -> Char.")
	
	self:SetInputs({
		{"A", TYPE_NUMBER, 2, 12}
	})
	
	self:SetOutputs({
		{"Out", TYPE_STRING, self:GetWidth()-2, 12}
	})
end

local out = nil
local inputs = nil
function Component:onInputsTriggered()
	inputs = self:GetInputsTable()
	
	out = string.char(inputs[1])
	if(out ~= nil) then
		self:TriggerOutput( 1, out )
	else
		self:TriggerOutput(1, "")
	end
end
