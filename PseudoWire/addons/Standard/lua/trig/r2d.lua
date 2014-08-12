function Component:onSpawn()
	self:SetImage("trig_r2d.png")
	self:SetToolTip("Radians to Degrees")
	
	self:SetInputs({
		{ "Radians", TYPE_NUMBER, 2, 20 }
	})
	
	self:SetOutputs({
		{ "Degrees", TYPE_NUMBER, self:GetWidth()-2, 20 }
	})
end

function Component:onInputsTriggered()
	self:TriggerOutput(1, math.deg(self:GetInputValue(1)))
end
