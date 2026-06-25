MongoDB c driver got compiled and its in the c root folder: C:\mongo-c-driver\install

compile this to avoid gcc headers conflicting with the msvc
& "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"


compile it like: cl test.c /I C:\mongo-c-driver\install\include\mongoc-2.4.0 /I C:\mongo-c-driver\install\include\bson-2.4.0 /link /LIBPATH:C:\mongo-c-driver\install\lib mongoc2.dll.lib bson2.dll.lib

run it all by going into cmd and typing test.exe