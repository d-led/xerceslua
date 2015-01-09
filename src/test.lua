local parser=xerces.XercesDOMParser()
parser:loadGrammar("Employee.dtd",xerces.GrammarType.DTDGrammarType)
parser:setValidationScheme(xerces.ValSchemes.Val_Auto)
local log=parser:parse("Employeexy.xml")
print(log.Ok)
if not log.Ok then
	print(log.Count)
	for i=0,log.Count-1 do
		local err=log:GetLogEntry(i)
		print(err.SystemId..', l:'..err.LineNumber..', c:'..err.ColumnNumber..', e:'..err.Message,err.LogType)
	end
end