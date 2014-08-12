function Component:onSpawn()
	self:SetImage("trig_adtan2.png")
	self:SetToolTip("Arc Tangent 2 (Deg)\natan2(rad(y, x))")
	
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
	self:TriggerOutput(1, math.atan2(math.rad(a, b)))
end
