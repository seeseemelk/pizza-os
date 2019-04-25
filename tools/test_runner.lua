#!/usr/bin/luajit

local command = "qemu-system-i386 -cdrom pizzaos.test.iso -m 16M -gdb tcp::1234 -serial stdio -display none -no-reboot"

--[[
local function check_state(state, line)
	if state == "print" then
		--if line:sub(1,1) == "?" then
	end
end
--]]

local function starts_with(line, str)
	return #line >= #str and line:sub(1, #str) == str
end

local function execute_tests()
	local ph = io.popen(command)

	local state = "print"
	local line = ph:read("*l")
	local testName = ""
	local success = false
	while line ~= nil do
		--state = check_state(state, line)

		if starts_with(line, "?RUN_TEST") then
			testName = line:match("(%S+)$")
			print("RUN " .. testName)
		elseif line == "?DONE" then
			success = true
		end

		--[[
		if state == "print" then
			print(line)
		end
		--]]
		line = ph:read("*l")
	end

	ph:close()

	if success then
		print("All tests ran successfully")
	else
		print("Some tests fail")
	end
end

local function main(arg)
	print("Executing unit tests")
	execute_tests()
end

main({...})
