function Component:onSpawn()
	self:SetImage("math_inc.png")
	self:SetInputs({
		{"Clk", TYPE_NUMBER, 2, 12},
		{"A", TYPE_NUMBER, 2, 20},
		{"Reset", TYPE_NUMBER, 2, 36}
	})
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12}
	})
	self:SetToolTip("Increment\nOut += A on Clk")
end

local rate = 1
local out = 0

local clk,a,reset
function Component:onInputsTriggered()
	clk,a,reset = self:GetAllInputValues()
	
	if(self:IsConnected(2)) then
		rate = a
	else
		rate = 0
	end
	
	if(clk ~= 0 and self:IsInputTriggered(1)) then
		out = out + rate
		self:TriggerOutput(1, out)
	end
	
	if(reset ~= 0 and self:IsInputTriggered(3)) then
		out = 0
		self:TriggerOutput(1, out)
	end
end
