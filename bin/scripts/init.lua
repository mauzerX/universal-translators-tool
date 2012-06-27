print "Starting Lua initiate..."

package.path = package.path..";./scripts/?.lua"
require 'common'


-- list of module names, there are no auto searching new module in UTT
-- new module must be added to this list
ModuleFolders = { 'example', 'privateer2' }

-- table with references to modules
UTTModules = {}

function initModules()
	for i = 1, #ModuleFolders do
		local fileName = ModuleFolders[i]..'/initmodule'
		require( fileName )
	end
end

initModules()

CurrentModule = UTTModules['example']

function getCurrentModule()
	return CurrentModule
end

function getExtensions()
	if CurrentModule ~= nil then
		return CurrentModule.getExtensions()
	end
end

function openFile( fileName )
	if CurrentModule ~= nil then
		return CurrentModule.openFile( fileName )
	end
end

function selectModule()
	-- we will allow user to select only succesfully loaded module
	local loadedModules = {}
	for k in pairs(UTTModules) do
		table.insert( loadedModules, k )
	end
	table.sort( loadedModules )
	
	local name = selectModuleDialog( loadedModules )
	if not name or string.len(name) == 0 then
		return
	end
	print (name..' module selected.')
	CurrentModule = UTTModules[ name ]
end

r = RGBA:new()
if CurrentModule == nil then
	selectModule()
end

print "Scripts initiated. Press '~' to show Lua console. Call reboot() to restart scripts."