function clamp(a, min, max)
	if(a > max) then
		return max
	elseif(a < min) then
		return min
	else
		return a
	end
end

function Component:onSpawn()
	self:SetImage("math_clamp.png")
	self:SetToolTip("Power\nOut = Min < A < Max")
	self:SetInputs({
		{"A", TYPE_NUMBER, 2, 12},
		{"Min", TYPE_NUMBER, 2, 20},
		{"Max", TYPE_NUMBER, 2, 28}
	})
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12}
	})
end

local a,min,max
function Component:onInputsTriggered()
	a,min,max = self:GetAllInputValues()
	self:TriggerOutput(1, clamp(a,min,max))
end