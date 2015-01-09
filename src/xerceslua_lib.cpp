#include "xerceslua_lib.h"

#include <lua.hpp>
#include <LuaBridge.h>
#include <RefCountedPtr.h>

#include <iostream>
#include <sstream>
#include <vector>

#ifdef _MSC_VER
#include <xercesc/util/Xerces_autoconf_config.msvc.hpp>
#endif

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>

XERCES_CPP_NAMESPACE_USE

    namespace xerces {
        namespace lua {

            static char const* version = "0.0.2";

            /////////////////
            class ErrorData {
            public:
                enum Type {
                    Warning=0,
                    Error,
                    FatalError
                };
            private:
                std::string SystemId;
                std::string PublicId;
                XMLFileLoc LineNumber;
                XMLFileLoc ColumnNumber;
                std::string Message;
                int LogType;
            public:
                std::string getSystemId() const { return SystemId; }
                std::string getPublicId() const { return PublicId; }
                int getLineNumber() const { return (int)LineNumber; }
                int getColumnNumber() const { return (int)ColumnNumber; }
                std::string getMessage() const { return Message; }
                int getLogType() const { return (int)LogType; }
            public:
                ErrorData(){}
                ErrorData(std::string s,std::string p,XMLFileLoc l,XMLFileLoc c,std::string m,Type t):
                    SystemId(s),
                    PublicId(p),
                    LineNumber(l),
                    ColumnNumber(c),
                    Message(m),
                    LogType(t)
                {}
            };


            //////////////////////
            class ErrorLog {

            public:
                void AddLogEntry(
                    std::string S,
                    std::string P,
                    XMLFileLoc L,
                    XMLFileLoc C,
                    std::string M,
                    ErrorData::Type T) {
                        ErrorData Log(S,P,L,C,M,T);
                    entries.push_back(Log);
                }

                int Count() const {
                    return entries.size();
                }

                bool Ok() const {
                    return entries.size()==0;
                }

                RefCountedPtr<ErrorData> GetLogEntry(size_t pos) const {
                    if (pos>=0 && pos<entries.size())
                        return RefCountedPtr<ErrorData>(new ErrorData(entries[pos]));
                    else
                        return RefCountedPtr<ErrorData>(new ErrorData);
                }
            private:
                std::vector<ErrorData> entries;
            };

            /////////////////////////////////////////////
            class CustomErrorHandler : public HandlerBase
            {
            public:
                CustomErrorHandler() {}

                void error(const SAXParseException& e)
                {
                    handler(e,ErrorData::Error);
                }

                void fatalError(const SAXParseException& e)
                {
                    handler(e,ErrorData::FatalError);
                }
                void warning(const SAXParseException& e)
                {
                    handler(e,ErrorData::Warning);
                }

                ErrorLog const& Get() const {
                    return Log;
                }
            private:
                void handler(const SAXParseException& e,const ErrorData::Type t)
                {
                    char* s=XMLString::transcode(e.getSystemId());
                    char* p=XMLString::transcode(e.getPublicId());
                    char* m=XMLString::transcode(e.getMessage());
                    std::string S; if (s) S=s;
                    std::string P; if (p) P=p;
                    std::string M; if (m) M=m;
                    Log.AddLogEntry(
                        S,
                        P,
                        e.getLineNumber(),
                        e.getColumnNumber(),
                        M,
                        t
                        );
                    XMLString::release(&s);
                    XMLString::release(&p);
                    XMLString::release(&m);

                    char* message = XMLString::transcode(e.getMessage());
#ifdef DEBUG
                    std::cerr << "line " << e.getLineNumber()
                        << ", column " << e.getColumnNumber()
                        << " -> " << message << "\n\n";
#endif
                    XMLString::release(&message);
                }
                ErrorLog Log;
            };

            ////////// GrammarType
            static int DTDGrammarType=(int)Grammar::DTDGrammarType;
            static int SchemaGrammarType=(int)Grammar::SchemaGrammarType;
            static int UnKnown=(int)Grammar::UnKnown;

            ////////// ValSchemes
            static int Val_Never=(int)AbstractDOMParser::Val_Never;
            static int Val_Always=(int)AbstractDOMParser::Val_Always;
            static int Val_Auto=(int)AbstractDOMParser::Val_Auto;

            //////////////////////
            class lXercesDOMParser {
            public:
                void loadGrammar(const char* filename,int grammartype) {
                    try {
                        parser.setDoSchema(true);
                        parser.setDoNamespaces(true);
                        parser.loadGrammar(filename,(Grammar::GrammarType)grammartype);
                    } catch (SAXException const& e) {
                        std::cerr << "loadGrammar: " << e.getMessage() << std::endl;
                    } catch (XMLException const& e) {
                        std::cerr << "loadGrammar: " << e.getMessage() << std::endl;
                    } catch (...) {
                        std::cerr<<"An error ocurred in loadGrammar"<<std::endl;
                    }
                }

                void setValidationScheme(int scheme) {
                    parser.setValidationScheme((XercesDOMParser::ValSchemes)scheme);
                }

                RefCountedPtr<ErrorLog> parse(const char* filename) {
                    CustomErrorHandler eh;
                    parser.setErrorHandler(&eh);
                    parser.parse(filename);
                    return RefCountedPtr<ErrorLog>(new ErrorLog(eh.Get()));
                }
            private:
                XercesDOMParser parser;
            };
        }
}

void register_xerceslua (lua_State* L) {
    using namespace xerces::lua;

    try {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& toCatch) {
        std::cerr<<toCatch.getMessage()<<std::endl;
    }

#ifdef DEBUG
    std::cout<<"loaded xerceslua "<<version<<std::endl;
#endif

    luabridge::getGlobalNamespace(L)
        .beginNamespace("xerces")

        .addVariable("version",&version,false)

        .beginNamespace("GrammarType")
            .addVariable("DTDGrammarType",&DTDGrammarType,false)
            .addVariable("SchemaGrammarType",&SchemaGrammarType,false)
            .addVariable("UnKnown",&UnKnown,false)
        .endNamespace()

        .beginNamespace("ValSchemes")
            .addVariable("Val_Never",&Val_Never,false)
            .addVariable("Val_Always",&Val_Always,false)
            .addVariable("Val_Auto",&Val_Auto,false)
        .endNamespace()

        .beginClass<ErrorLog>("ErrorLog")
            .addConstructor<void (*)()>()
            .addProperty("Count",&ErrorLog::Count)
            .addProperty("Ok",&ErrorLog::Ok)
            .addFunction("GetLogEntry",&ErrorLog::GetLogEntry)
        .endClass()

        .beginClass<ErrorData>("ErrorData")
            .addConstructor<void (*)()>()
            .addProperty("SystemId",&ErrorData::getSystemId)
            .addProperty("PublicId",&ErrorData::getPublicId)
            .addProperty("LineNumber",&ErrorData::getLineNumber)
            .addProperty("ColumnNumber",&ErrorData::getColumnNumber)
            .addProperty("Message",&ErrorData::getMessage)
            .addProperty("LogType",&ErrorData::getLogType)
        .endClass()

        .beginClass<lXercesDOMParser>("XercesDOMParser")
            .addConstructor<void (*)()>()
            .addFunction("loadGrammar",&lXercesDOMParser::loadGrammar)
            .addFunction("setValidationScheme",&lXercesDOMParser::setValidationScheme)
            .addFunction("parse",&lXercesDOMParser::parse)
        .endClass()

        .endNamespace()
        ;

}