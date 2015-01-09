local potential_paths = {
'../bin/windows/vs2013/x32/Release/?.dll',
'../bin/windows/vs2013/x32/Debug/?.dll',
'../bin/windows/vs2013/x64/Release/?.dll',
'../bin/windows/vs2013/x64/Debug/?.dll',
'../bin/windows/vs2013/native/Release/?.dll',
'../bin/windows/vs2013/native/Debug/?.dll',
'../bin/linux/gmake/x32/Release/?.so',
'../bin/linux/gmake/x32/Debug/?.so',
'../bin/linux/gmake/x64/Release/?.so',
'../bin/linux/gmake/x64/Debug/?.so'
}

local extra_cpath = table.concat(potential_paths, ";");
package.cpath = extra_cpath..';'..package.cpath

assert(require 'xerceslua')

local parser=xerces.XercesDOMParser()
parser:loadGrammar("sample.xsd",xerces.GrammarType.SchemaGrammarType)
parser:setValidationScheme(xerces.ValSchemes.Val_Always)
local log=parser:parse("sample.xml")
print('parse ok: ',log.Ok)
if not log.Ok then
    print(log.Count)
    for i=0,log.Count-1 do
        local err=log:GetLogEntry(i)
        print(err.SystemId..', l:'..err.LineNumber..', c:'..err.ColumnNumber..', e:'..err.Message,err.LogType)
    end
end
