function Component:onSpawn()
	self:SetImage("ram_d_clk.png")
	self:SetToolTip("Clocked Data Latch")
	
	self:SetInputs({
		{"Clk", TYPE_NUMBER, 2, 12},
		{"D", TYPE_NUMBER, 2, 20}
	})
	
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12}
	})
end

function Component:onInputsTriggered()
	if(self:IsInputTriggered(1) and self:GetInputValue(1) ~= 0) then
		self:TriggerOutput(1, self:GetInputValue(2))
	end
end
