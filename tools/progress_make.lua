#!/bin/lua
local function read_all_lines(file)
	local lines = {}
	local count = 0
	for line in io.lines(file) do
		if #line > 0 and lines[line] ~= true then
			lines[line] = true
			count = count + 1
		end
	end
	return lines, count
end

local buffer = {}
local function write(str)
	buffer[#buffer + 1] = str
end
local function flush()
	io.write(table.concat(buffer))
	io.flush()
	buffer = {}
end

local function print_bar(value, max, width)
	if not width then
		width = max
	else
		width = width - 10
	end
	local scale = width / max
	local filled_scaled = math.floor(value * scale)
	local empty_scaled = math.floor((max - value) * scale)
	write('\r')
	local str = table.concat({string.format(" %3.0f%%", value / max * 100), " [", ("#"):rep(filled_scaled), ("."):rep(empty_scaled), "]"})
	write(str)
	write('\r')
	flush()
end

local function clear_bar(max, width)
	if not width then
		width = max
	end
	write("\r")
	write((" "):rep(width))
	write("\r")
end

local function main(progress_file, width)
	local all_lines, line_count = read_all_lines(progress_file)

	local done = 0
	local line = io.read()
	
	while line ~= null do
		if all_lines[line] == true then
			all_lines[line] = false
			done = done + 1
			print_bar(done, line_count, width)
		elseif all_lines[line] ~= false then
			clear_bar(line_count, width)
			write(line)
			write("\n")
			print_bar(done, line_count, width)
		end
		line = io.read("*l")
	end
	
	clear_bar(line_count, width)
	flush()
end

main(...)
