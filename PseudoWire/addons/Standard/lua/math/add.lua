-- Adding Gate
-- Outputs the sum of input values

function Component:onSpawn()
	self:SetImage("math_add.png")
	self:SetToolTip("Add\nOut = A + B + C + D")
	
	self:SetInputs({
		{"A", TYPE_NUMBER, 2, 12}, --input 1
		{"B", TYPE_NUMBER, 2, 20}, --input 2
		{"C", TYPE_NUMBER, 2, 28}, --input 3
		{"D", TYPE_NUMBER, 2, 36}  --input 4
	})
	
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth() - 2, 12} --output 1
	})
end

local a,b,c,d
function Component:onInputsTriggered()
	a,b,c,d = self:GetAllInputValues()
	self:TriggerOutput(1, a + b + c + d) --send the sum of all inputs to output 1
end
