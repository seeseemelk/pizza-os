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
	local success = false

	local testCount = 0
	local testIndex = 0
	local test = {
		name = nil,
		log = {}
	}


	while line ~= nil do
		--state = check_state(state, line)

		if starts_with(line, "?TEST_COUNT") then
			testCount = tonumber(line:match("(%d+)$"))
			--print("TEST COUNT: " .. testCount)
		elseif starts_with(line, "?TEST_GOOD") then
		elseif starts_with(line, "?RUN_TEST") then
			test.name = line:match("^?RUN_TEST (.+)$")
			test.log = {}
			testIndex = testIndex + 1
			print(string.format(" => RUNNING %s (%d/%d %d%%)",
					test.name, testIndex, testCount, testIndex / testCount * 100))
		elseif starts_with(line, "?RUN_GROUP") then
			local groupName = line:match("(%S+)$")
			print("GROUP " .. groupName)
		elseif line == "?DONE" then
			success = true
		elseif starts_with(line, "!") then
			local expected, got, message
			if starts_with(line, "!<<<") then
				expected, got = line:match("<<<(.-)§(.-)>>>")
			end
			message = line:match(">>>(.-)$")
			
			print()
			print("=== TEST LOG ===")
			for _, line in ipairs(test.log) do
				print(line)
			end

			print()
			print("=== FAILURE INFORMATION ===")
			print(string.format("Test %s failed.", test.name))
			print(string.format("Message: %s", message))
			if expected and got then
				print(string.format("Expected %s, but got %s.", expected, got))
			end
		elseif starts_with(line, "?") then
			error("Unknown command: " .. line)
		else
			test.log[#test.log + 1] = line
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
		print()
		print("All tests ran successfully")
	end
	return success
end

local function main(arg)
	print("Executing unit tests")
	if execute_tests() then
		return 0
	else
		return 1
	end
end

os.exit(main({...}))
