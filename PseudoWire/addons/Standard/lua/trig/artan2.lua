function Component:onSpawn()
	self:SetImage("trig_artan2.png")
	self:SetToolTip("Arc Tangent 2 (Rads)\natan2(y, x)")
	
	self:SetInputs({
		{ "Y", TYPE_NUMBER, 2, 12 },
		{ "X", TYPE_NUMBER, 2, 20 }
	})
	
	self:SetOutputs({
		{ "Out", TYPE_NUMBER, self:GetWidth()-2, 20 }
	})
end

local a,b
function Component:onInputsTriggered()
	a,b = self:GetAllInputValues()
	self:TriggerOutput(1, math.atan2(a, b))
end
