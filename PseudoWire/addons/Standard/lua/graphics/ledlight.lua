local image_on = "io_led_on.png"
local image_off = "io_led_off.png"

local on = false

function Component:onSpawn()
	self:SetImage(image_off)
	self:SetToolTip("LED\nOff")
	
	self:SetInputs({
		{"A", TYPE_NUMBER, self:GetWidth()/2, self:GetHeight()-2}
	})
end

local prev_on = false
function Component:onInputsTriggered()
	prev_on = on
	on = (self:GetInputValue(1) ~= 0)
	
	-- if state didn't change, don't bother setting the image. (optimization)
	if(on == prev_on) then return end
	
	if( on ) then
		self:SetImage(image_on)
		self:SetToolTip("LED\nOn")
	else
		self:SetImage(image_off)
		self:SetToolTip("LED\nOff")
	end
end
