xerceslua
=========

A minimal lua wrapper of Xerces-C++ in order to be able to validate xml files

Usage
-----

````lua
assert(require 'xerceslua')

local parser=xerces.XercesDOMParser()
parser:loadGrammar("sample.dtd",xerces.GrammarType.DTDGrammarType)
parser:setValidationScheme(xerces.ValSchemes.Val_Always)
local log=parser:parse("sample.xml")
print(log.Ok)
if not log.Ok then
	print(log.Count)
	for i=0,log.Count-1 do
		local err=log:GetLogEntry(i)
		print(err.SystemId..', l:'..err.LineNumber..', c:'..err.ColumnNumber..', e:'..err.Message,err.LogType)
	end
end
````

Demo
----

Windows:
```
cd test
.\test.bat
```

Dependencies
------------

 * [Xerces-C++](http://xerces.apache.org/xerces-c/) the original library, unpack the source into the xerceslua directory
 * [lua](http://www.lua.org/) the language, Lua 5.1.4 from [luaforwindows](http://code.google.com/p/luaforwindows/), but it should work for every proper lua which is supported by LuaBridge
 * [LuaBridge](https://github.com/vinniefalco/LuaBridge) for the declarative bindings to Lua

License
-------

This library is distributed under the MIT License:

Copyright (c) 2012-2014 Dmitry Ledentsov

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
