local bar = nil
local bar_width = 0
local bar_height = 0
local val = 0.0
local min = 0.0
local max = 1.0

function Clamp(val, mini, maxi)
	if(val > maxi) then return maxi
	elseif(val < mini) then return mini
	else return val end
end

function Component:onSpawn()
	self:SetImage("graphics_progressdisp.png")
	self:SetToolTip("Progress Bar")
	
	bar_width = self:GetWidth()-8
	bar_height = self:GetHeight()-6
	
	bar = game.create_rect( 0,0 , bar_width,bar_height , Color(64,115,155,128) )
	
	self:SetInputs({
		{"A", TYPE_NUMBER, 2, 6},
		{"Min", TYPE_NUMBER, 2, 14},
		{"Max", TYPE_NUMBER, 2, 22}
	})
end

local inputs = nil
function Component:onInputsTriggered()
	inputs = self:GetInputsTable()
	
	val = inputs[1]
	
	if(self:IsConnected(2)) then
		min = inputs[2]
	else
		min = 0.0
	end
	if(self:IsConnected(3)) then
		max = inputs[3]
	else
		max = 1.0
	end
	
	local percent = Clamp((val-min)/(max-min), 0, 1)
	bar:SetScale(Vector( percent, 1 ))
	bar:SetPos(Vector(5,3))
	
	self:SetToolTip("Progress Bar\n" .. math.floor(percent*100) .. "%")
end

function Component:onDraw()
	if(val > min and self:IsConnected(1)) then
		self:Draw(bar)
	end
end
