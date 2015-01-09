include 'premake'

make_solution 'xerceslua'

platforms { 'native' }

lua = assert(dofile 'premake/recipes/lua.lua')

OS = os.get()

includedirs {
    './deps/LuaBridge',
    lua.includedirs[OS]
}

configuration 'windows'
	includedirs {
	    './deps/xerces-c-helper',
	    './deps/xerces-c/src'
	}
configuration '*'

make_shared_lib('xerceslua', {
	'./src/*.*'
})

libdirs {
	lua.libdirs[OS],
	'./bin' -- xerces pre-build file location
}

links {
	lua.links[OS]
}

configuration { 'Debug', 'windows' }
	links 'xerces-c_3D'
configuration { 'Release', 'windows' }
	links 'xerces-c_3'
configuration 'not windows'
	links 'xerces-c'
configuration 'macosx'
	targetextension '.so'
configuration '*'

targetprefix ""
