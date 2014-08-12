function Component:onSpawn()
	self:SetInputs({
		{"A", TYPE_NUMBER, 2, self:GetHeight()/2}
	})
end

local v
function Component:onInputsTriggered()
	v = self:GetInputValue(1)
	if(v ~= 0) then
		print("")
	end
end
