
assembler_objects = ast.tab.o lex.yy.o Command.o Hardware.o Registers.o Utils.o AssemblerGen.o
disassembler_objects = Command.o Utils.o Registers.o Disassembler.o Hardware.o
debugger_objects = Command_dbg.o Debugger.o Utils_dbg.o Hardware_dbg.o Registers.o
runtime_objects = Command.o Utils.o Hardware.o Runtime.o Registers.o

all_objects = Command_dbg.o Command.o Debugger.o Utils.o Utils_dbg.o Hardware.o Hardware_dbg.o Runtime.o Registers.o Disassembler.o AssemblerGen.o ast.tab.o lex.yy.o

java_home = D:\Program Files\Java\jdk-12.0.1
#java_home = %JAVA_HOME%

compiler = x86_64-w64-mingw32-g++

all: debugger runtime disassembler assembler

assembler: $(assembler_objects)
	$(compiler) -o assembler.exe $(assembler_objects)

disassembler: $(disassembler_objects)
	$(compiler) -o disassembler.exe $(disassembler_objects)

debugger: $(debugger_objects)
	$(compiler) -shared -DDEBUGGER -o jniDB.dll $(debugger_objects) -static-libstdc++ -static-libgcc -Wl,--add-stdcall-alias -m64

runtime: $(runtime_objects)
	$(compiler) -o runtime.exe $(runtime_objects)

Command.o: Hardware.h Registers.o
	$(compiler) -c -o Command.o Command.cpp
	
Command_dbg.o: Hardware_dbg.o Registers.o
	$(compiler) -DDEBUGGER -c -I"$(java_home)\include" -I"$(java_home)\include\win32" -o Command_dbg.o Command.cpp Hardware_dbg.o

Debugger.o: Utils.o Hardware.o
	$(compiler) -DDEBUGGER -c -I"$(java_home)\include" -I"$(java_home)\include\win32" -o Debugger.o Debugger.cpp  Utils.o Hardware.o

Utils.o: Command.h
	$(compiler) -c -o Utils.o Utils.cpp

Utils_dbg.o: Command.h
	$(compiler) -DDEBUGGER -c -I"$(java_home)\include" -I"$(java_home)\include\win32" -o Utils_dbg.o Utils.cpp

Hardware.o:
	$(compiler) -c -o Hardware.o Hardware.cpp
	
Hardware_dbg.o:
	$(compiler) -DDEBUGGER -c -o Hardware_dbg.o Hardware.cpp

Runtime.o: Command.h Utils.h
	$(compiler) -c -o Runtime.o Runtime.cpp

Registers.o:
	$(compiler) -c -o Registers.o Registers.cpp

AssemblerGen.o: Registers.o Hardware.o
	$(compiler) -c AssemblerGen.cpp

ast.tab.o : ast.tab.c AssemblerGen.o
	$(compiler) -c ast.tab.c

lex.yy.o : lex.yy.c ast.tab.h AssemblerGen.o
	$(compiler) -c lex.yy.c

ast.tab.c: ast.y
	bison -d ast.y

ast.tab.h : ast.y
	bison -d ast.y
	
lex.yy.c : ast.lex
	flex ast.lex

clean:
	rm $(all_objects) lex.yy.c ast.tab.c ast.tab.h runtime.exe jniDB.dll disassembler.exe assembler.exe
