function Component:onSpawn()
	self:SetImage("io_msg.png")
	self:SetInputs({
		{"Send", TYPE_NUMBER, 2, 12},
		{"Message", TYPE_ANY, 2, 18}
	})
end

local inputs = nil
function Component:onInputsTriggered()
	inputs = self:GetInputsTable()
	
	if(self:IsInputTriggered(1) and inputs[1] ~= 0) then
		if(inputs[2] == nil) then
			Msg("")
		else
			Msg(inputs[2])
		end
	end
end