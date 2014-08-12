function Component:onSpawn()
	self:SetImage("trig_d2r.png")
	self:SetToolTip("Degrees to Radians")
	
	self:SetInputs({
		{ "Degrees", TYPE_NUMBER, 2, 20 }
	})
	
	self:SetOutputs({
		{ "Radians", TYPE_NUMBER, self:GetWidth()-2, 20 }
	})
end

function Component:onInputsTriggered()
	self:TriggerOutput(1, math.rad(self:GetInputValue(1)))
end
