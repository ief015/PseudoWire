local txt = Text()
local cursor_shp = nil
local buffer = ""

local buffer_columns = 16
local buffer_rows = 14
local buffer_size = 8

local insert_mode = false
local cursor = 0

function UpdateText()
	local buf = ""
	
	for row = 0, buffer_rows-1 do
		
		local addstr = buffer:sub(row*buffer_columns + 1 , row*buffer_columns + buffer_columns)
		if(addstr == nil) then
			break
		end
		
		buf = buf .. addstr
		
		if(row ~= buffer_rows-1) then
			buf = buf.."\n"
		end
	end
	
	txt:SetText(buf)
end

function Component:onSpawn()
	self:SetImage("display_console.png")
	self:SetInputs({
		{"Clk", TYPE_NUMBER, 3, 12},
		{"Char", TYPE_NUMBER, 3, 20},
		
		{"Backspace", TYPE_NUMBER, 3, 36},
		
		{"Clear", TYPE_NUMBER, 3, 52}
	})
	
	txt:SetFont("DlxFont.ttf")
	txt:SetSize(buffer_size)
	txt:SetColor(Color(196,196,196,205))
	txt:SetPos(Vector(7,7))
	
	cursor_shp = game.create_rect(7, 7+buffer_size-2, buffer_size-2, 2, Color(196,196,196))
end

function Component:onDraw()
	self:Draw(txt)
	self:Draw(cursor_shp)
end

local updatetxt = false
local inputs = nil
function Component:onInputsTriggered()
	inputs = self:GetInputsTable()
	
	if(self:IsInputTriggered(1) and inputs[1] ~= 0) then
		if(CharIsPrintable(inputs[2])) then
			buffer = string.insert(buffer, string.char(inputs[2]), cursor)
			updatetxt = true
			
			if(#buffer >= buffer_columns*buffer_rows) then
				buffer = string.sub(buffer, #buffer-(buffer_columns*buffer_rows)+buffer_columns+1, #buffer)
				cursor = cursor - buffer_columns+1
				cursor_shp:Move(-buffer_size*(buffer_columns-1), 0)
			else
				cursor = cursor + 1
				cursor_shp:Move(buffer_size, 0)
				
				if(cursor % buffer_columns == 0) then
					cursor_shp:Move(0, buffer_size+1)
					cursor_shp:X(0)
				end
			end
		end
	end
	
	if(self:IsInputTriggered(3) and inputs[3] ~= 0) then
		if(#buffer ~= 0) then
			buffer = string.remove(buffer, cursor-1, cursor)
			updatetxt = true
			
			cursor = cursor - 1
			cursor_shp:Move(-buffer_size, 0)
			
			if((cursor+1) % buffer_columns == 0) then
				cursor_shp:Move(0, -(buffer_size+1))
				cursor_shp:X(buffer_size * (buffer_columns-1))
			end
		end
	end
	
	if(self:IsInputTriggered(4) and inputs[4] ~= 0) then
		buffer = ""
		updatetxt = true
		cursor = 0
		cursor_shp:SetPos(Vector(0, 0))
	end
	
	if(updatetxt) then
		UpdateText()
	end
end
